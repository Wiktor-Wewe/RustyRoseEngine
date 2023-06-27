#include "BackGround.h"

BackGround::BackGround(SDL_Renderer* renderer, std::string path, std::vector<std::string> fileNames)
{
	this->_renderer = renderer;
	this->_path = path;
	this->_fileNames = fileNames;
	this->_loadImage();
	this->_loadAnimation();
}

SDL_Texture* BackGround::getTexture()
{
	if (this->_texture != NULL) {
		return this->_texture;
	}
	return nullptr;
}

void BackGround::_loadImage()
{
	SDL_Surface* surface = IMG_Load(this->_path.c_str());
	if (surface == NULL) {
		printf("unable to load backgroud image: %s\n", this->_path.c_str());
	}

	this->_texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
	if (this->_texture == NULL) {
		printf("unable to make texture from bg image: %s\n", this->_path.c_str());
	}

	SDL_FreeSurface(surface);
}

void BackGround::_loadAnimation()
{
	auto list = this->_findAnimations(this->_path);

	if (list.size() > 1) {
		for (int i = 1; i < list.size();) {
			TalkAnimation* ta = new TalkAnimation;
			ta->i = 0;
			ta->shortName = this->_getName(list[i]);
			ta->sprites[0] = this->_getSpriteForAnimaton(list[i]);
			i++;
			ta->sprites[1] = this->_getSpriteForAnimaton(list[i]);
			i++;
			ta->sprites[2] = this->_getSpriteForAnimaton(list[i]);
			i++;
			this->animations.push_back(ta);
		}
	}
}

std::vector<std::string> BackGround::_findAnimations(std::string path)
{
	std::vector<std::string> result;
	path = path.substr(0, path.size() - 4);


	for (const std::string& str : this->_fileNames) {
		if (str.find(path) != std::string::npos) {
			result.push_back(str);
		}
	}

	return result;
}

SDL_Texture* BackGround::_getSpriteForAnimaton(std::string path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL) {
		printf("unable to load animation image: %s\n", path.c_str());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
	if (texture == NULL) {
		printf("unable to make texture from animation image: %s\n", path.c_str());
	}

	SDL_FreeSurface(surface);
	return texture;
}

std::string BackGround::_getName(std::string path)
{
	return path.substr(path.size() - 9, 3);
}
