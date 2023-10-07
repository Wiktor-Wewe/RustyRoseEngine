#include "VDecoder.h"

VDecoder::VDecoder(SDL_Renderer* renderer)
{
	avformat_network_init();
	this->_renderer = renderer;
	this->_frameWidth = 800;
	this->_frameHeight = 452;
	this->_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, this->_frameWidth, this->_frameHeight); // <- get size of frame from file
}

void VDecoder::setPath(std::string path)
{
	this->_path = path;
	this->_frame = NULL;
}

bool VDecoder::start()
{
	this->_formatContext = avformat_alloc_context();

	if (avformat_open_input(&this->_formatContext, this->_path.c_str(), NULL, NULL) != 0) {
		// Error while opening the file
		return false;
	}

	if (avformat_find_stream_info(this->_formatContext, NULL) < 0) {
		// Error while taking info about streams
		return false;
	}

	// Try to find index of video
	this->_videoStreamIndex = -1;
	for (int i = 0; i < this->_formatContext->nb_streams; i++) {
		if (this->_formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			this->_videoStreamIndex = i;
			break;
		}
	}

	if (this->_videoStreamIndex == -1) {
		// Cant find video stream
		return false;
	}

	this->_codecContext = avcodec_alloc_context3(NULL);

	// Setting decoder's context from parameters in video stream
	avcodec_parameters_to_context(this->_codecContext, this->_formatContext->streams[this->_videoStreamIndex]->codecpar);

	this->_codec = avcodec_find_decoder(this->_codecContext->codec_id);

	if (this->_codec == NULL) {
		// Cant find decoder
		return false;
	}

	if (avcodec_open2(this->_codecContext, this->_codec, NULL) < 0) {
		// Error while opening decoder
		return false;
	}

	this->_packet = av_packet_alloc();

	return true;
}

bool VDecoder::decodeFrame()
{
	while (av_read_frame(this->_formatContext, this->_packet) >= 0) {
		if (this->_packet->stream_index == this->_videoStreamIndex) {
			if (this->_frame != NULL) {
				av_frame_free(&this->_frame);
			}
			this->_frame = av_frame_alloc();
			int frameFinished;
			avcodec_send_packet(this->_codecContext, this->_packet);
			frameFinished = avcodec_receive_frame(this->_codecContext, this->_frame);
			if (frameFinished == 0) {
				return true;
			}
			else if (frameFinished == AVERROR(EAGAIN)) {
				continue;
			}
			else {
				break;
			}
		}
	}
	if (this->_frame != NULL) {
		av_frame_free(&this->_frame);
	}
	av_packet_unref(this->_packet);
	return false;
}

void VDecoder::free()
{
	if (this->_frame != NULL) {
		av_frame_free(&this->_frame);
		this->_frame = NULL;
	}

	if (this->_packet != NULL) {
		av_packet_unref(this->_packet);
		this->_packet = NULL;
	}

	avcodec_close(this->_codecContext);
	this->_codecContext = NULL;
	avformat_close_input(&this->_formatContext);
	this->_formatContext = NULL;
}

SDL_Texture* VDecoder::getFrame()
{
	if (this->_frame == nullptr)
	{
		return nullptr;
	}

	// Prepare RGB data buffer for the frame
	uint8_t* rgbData = new uint8_t[this->_frame->width * this->_frame->height * 3];

	// Set up FFmpeg's SwsContext for YUV to RGB conversion
	int srcWidth = this->_frame->width;
	int srcHeight = this->_frame->height;
	AVPixelFormat srcFormat = AV_PIX_FMT_YUV420P; // Assuming the input is YUV420P
	int dstWidth = srcWidth;
	int dstHeight = srcHeight;
	AVPixelFormat dstFormat = AV_PIX_FMT_RGB24; // Convert to RGB24

	SwsContext* swsContext = sws_getContext(srcWidth, srcHeight, srcFormat,
		dstWidth, dstHeight, dstFormat,
		SWS_BILINEAR, nullptr, nullptr, nullptr);


	// Perform YUV to RGB conversion
	uint8_t* srcData[AV_NUM_DATA_POINTERS] = { this->_frame->data[0], this->_frame->data[1], this->_frame->data[2] };
	int srcLinesize[AV_NUM_DATA_POINTERS] = { this->_frame->linesize[0], this->_frame->linesize[1], this->_frame->linesize[2] };
	uint8_t* dstData[AV_NUM_DATA_POINTERS] = { rgbData, nullptr, nullptr };
	int dstLinesize[AV_NUM_DATA_POINTERS] = { this->_frame->width * 3, 0, 0 };
	sws_scale(swsContext, srcData, srcLinesize, 0, srcHeight, dstData, dstLinesize);

	// Clean up the SwsContext
	sws_freeContext(swsContext);

	// Lock the texture to update its data.
	void* texturePixels;
	int texturePitch;
	SDL_LockTexture(this->_texture, nullptr, &texturePixels, &texturePitch);

	// Copy the converted RGB data to the texture
	memcpy(texturePixels, rgbData, dstWidth * dstHeight * 3);

	// Unlock the texture
	SDL_UnlockTexture(this->_texture);

	// Clean up the RGB data buffer
	delete[] rgbData;

	return this->_texture;
}

VDecoder::~VDecoder()
{
	if (this->_formatContext) {
		avformat_close_input(&this->_formatContext);
	}

	if (this->_codecContext) {
		avcodec_close(this->_codecContext);
	}

	if (this->_frame) {
		av_frame_free(&this->_frame);
	}

	if (this->_packet) {
		av_packet_unref(this->_packet);
	}

	if (this->_codecContext) {
		avcodec_close(this->_codecContext);
	}

	if (this->_formatContext) {
		avformat_close_input(&this->_formatContext);
	}

	if (this->_texture) {
		SDL_DestroyTexture(this->_texture);
	}
}

int VDecoder::_getWidthFrame()
{
	if (this->_frame != NULL) {
		return this->_frame->width;
	}
	return 0;
}

int VDecoder::_getHeightFrame()
{
	if (this->_frame != NULL) {
		return this->_frame->height;
	}
	return 0;
}

int* VDecoder::_getLinesize()
{
	if (this->_frame != NULL) {
		return this->_frame->linesize;
	}
	return NULL;
}

uint8_t** VDecoder::_getFrameData()
{
	return this->_frame->data;
}
