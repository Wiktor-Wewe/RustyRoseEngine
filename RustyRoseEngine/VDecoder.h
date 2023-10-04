extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
}
#include "SharedUtils.h"


class VDecoder
{
public:
	VDecoder(SDL_Renderer* renderer);
	void setPath(std::string path);
	bool start();
	bool decodeFrame();
	void freeDecoder();

	SDL_Texture* getFrame();

private:
	SDL_Renderer* _renderer;
	std::string _path;

	int _frameWidth;
	int _frameHeight;

	AVFormatContext* _formatContext;
	int _videoStreamIndex;
	AVCodecContext* _codecContext;
	const AVCodec* _codec;
	AVPacket* _packet;
	AVFrame* _frame;

	SDL_Texture* _texture;

	int _getWidthFrame();
	int _getHeightFrame();
	int* _getLinesize();
	uint8_t** _getFrameData();
};

