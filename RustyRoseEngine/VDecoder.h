extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
}


class VDecoder
{
public:
	VDecoder();
	void setPath(char* path);
	bool start();
	bool decodeFrame();
	uint8_t** getFrameData();
	int getWidthFrame();
	int getHeightFrame();
	int* getLinesize();
	void freeDecoder();
	

private:
	char* _path;
	AVFormatContext* _formatContext;
	int _videoStreamIndex;
	AVCodecContext* _codecContext;
	const AVCodec* _codec;
	AVPacket* _packet;
	AVFrame* _frame;
};

