#include "VDecoder.h"

VDecoder::VDecoder(SDL_Renderer* renderer)
{
	avformat_network_init();
	this->_renderer = renderer;
	this->_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 800, 452); // <- get size of frame from file
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

void VDecoder::freeDecoder()
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

	//SDL_DestroyTexture(this->_texture);
	//this->_texture = NULL;
}

SDL_Texture* VDecoder::getFrame()
{
	uint8_t* yPlane = this->_frame->data[0];
	uint8_t* uPlane = this->_frame->data[1];
	uint8_t* vPlane = this->_frame->data[2];

	int yPitch = this->_frame->linesize[0];
	int uPitch = this->_frame->linesize[1];
	int vPitch = this->_frame->linesize[2];

	uint8_t* rgbData = new uint8_t[800 * 452 * 3];
	int rgbIndex = 0;

	for (int y = 0; y < 452; y++)
	{
		for (int x = 0; x < 800; x++)
		{
			int uvIndex = (x >> 1) + (y >> 1) * (uPitch);
			int yIndex = x + y * yPitch;

			uint8_t Y = yPlane[yIndex];
			uint8_t U = uPlane[uvIndex];
			uint8_t V = vPlane[uvIndex];

			int C = Y - 16;
			int D = U - 128;
			int E = V - 128;

			uint8_t R = (298 * C + 409 * E + 128) >> 8;
			uint8_t G = (298 * C - 100 * D - 208 * E + 128) >> 8;
			uint8_t B = (298 * C + 516 * D + 128) >> 8;

			rgbData[rgbIndex] = R;
			rgbData[rgbIndex + 1] = G;
			rgbData[rgbIndex + 2] = B;

			rgbIndex += 3;
		}
	}

	void* texturePixels;
	int texturePitch;
	SDL_LockTexture(this->_texture, nullptr, &texturePixels, &texturePitch);
	memcpy(texturePixels, rgbData, 800 * 452 * 3);
	SDL_UnlockTexture(this->_texture);

	delete[] rgbData;

	return this->_texture;
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
