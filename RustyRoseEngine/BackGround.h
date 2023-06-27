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

	BackGround(SDL_Renderer* renderer, std::string path, std::vector<std::string> fileNames);
	SDL_Texture* getTexture();

private:
	SDL_Renderer* _renderer;

	std::vector<std::string> _fileNames;
	std::string _path;
	SDL_Texture* _texture;
	std::vector<TalkAnimation*> animations;
	void _loadImage();
	void _loadAnimation();

	std::vector<std::string> _findAnimations(std::string path);
	SDL_Texture* _getSpriteForAnimaton(std::string path);
	std::string _getName(std::string path);
};

