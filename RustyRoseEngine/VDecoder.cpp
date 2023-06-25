#include "VDecoder.h"

VDecoder::VDecoder()
{
	avformat_network_init();
}

void VDecoder::setPath(char* path)
{
	this->_path = path;
	this->_frame = NULL;
}

bool VDecoder::start()
{
	this->_formatContext = avformat_alloc_context();

	if (avformat_open_input(&this->_formatContext, this->_path, NULL, NULL) != 0) {
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

uint8_t** VDecoder::getFrameData()
{
	return this->_frame->data;
}

int VDecoder::getWidthFrame()
{
	if (this->_frame != NULL) {
		return this->_frame->width;
	}
	return 0;
}

int VDecoder::getHeightFrame()
{
	if (this->_frame != NULL) {
		return this->_frame->height;
	}
	return 0;
}

int* VDecoder::getLinesize()
{
	if (this->_frame != NULL) {
		return this->_frame->linesize;
	}
	return NULL;
}

void VDecoder::freeDecoder()
{
	if (this->_frame != NULL) {
		av_frame_free(&this->_frame);
	}

	if (this->_packet != NULL) {
		av_packet_unref(this->_packet);
	}

	avcodec_close(this->_codecContext);
	avformat_close_input(&this->_formatContext);
}

