#pragma once
#include <SDL_image.h>
#include <stdio.h>
#include <vector>
#include <string>

class BackGround
{
public:
	struct TalkAnimation
	{
		int i;
		std::string shortName;
		SDL_Texture* sprites[3];
	};

	BackGround(SDL_Renderer* renderer, std::string path);
	void tryLoadAnimation(std::string shortName);
	SDL_Texture* getTexture();
	SDL_Texture* getNextAnimationTexture(std::string shortName);
	std::string getPath();
	void free();

private:
	SDL_Renderer* _renderer;

	int _animationCounter;
	std::string _path;
	SDL_Texture* _texture;
	std::vector<TalkAnimation*> animations;

	void _loadImage();
	SDL_Texture* _tryGetAnimationTexture(std::string path);
};

