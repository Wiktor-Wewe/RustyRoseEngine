#pragma once
#include "SharedUtils.h"
#include "Image.h"

class BackGround : public Image 
{
public:
	struct TalkAnimation
	{
		std::string shortName;
		SDL_Texture* sprites[3] = { NULL, NULL, NULL };

		~TalkAnimation()
		{
			for (auto& sprite : this->sprites) {
				if (sprite) {
					SDL_DestroyTexture(sprite);
				}
			}
		}
	};

	BackGround(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, std::string path);
	void tryLoadAnimation(std::string shortName);
	void resetAnimationCounter();
	void free();

	SDL_Texture* getAnimation(std::string shortName);

	~BackGround();

private:
	int _animationCounter;
	std::unordered_map<std::string, TalkAnimation*> _animations; //key = shortName

	SDL_Texture* _tryGetAnimationTexture(std::string path);
};

