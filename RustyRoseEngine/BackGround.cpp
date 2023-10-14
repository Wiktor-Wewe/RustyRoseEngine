#include "BackGround.h"

BackGround::BackGround(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, std::string path) : Image(renderer, screenSize, path)
{
	this->_animationCounter = 0;
}

bool BackGround::tryLoadAnimation(std::string shortName)
{
	std::string animPath = this->_path.substr(0, this->_path.size() - 4);
	animPath = RRE_NormalizePath(animPath + shortName + ".A.PNG");

	SDL_Texture* buffTexture = this->_tryGetAnimationTexture(animPath);
	
	if (buffTexture == NULL) { // its possible that there is no animation for this background
		false;
	}

	// if animation .A. excist -> allocate animation -> add .A. -> load and add .B. -> load and add .C.
	TalkAnimation* animation = new TalkAnimation;
	animation->shortName = shortName;
	animation->sprites[0] = buffTexture;

	animPath[animPath.size() - 1 - 4] = 'B';
	buffTexture = this->_tryGetAnimationTexture(animPath);
	animation->sprites[1] = buffTexture;

	animPath[animPath.size() - 1 - 4] = 'C';
	buffTexture = this->_tryGetAnimationTexture(animPath);
	animation->sprites[2] = buffTexture;

	// add to animations
	this->_animations[shortName] = animation;
	return true;
}

void BackGround::resetAnimationCounter()
{
	this->_animationCounter = 0;
}

void BackGround::free()
{
	if (this->_texture) {
		SDL_DestroyTexture(this->_texture);
		this->_texture = NULL;
	}

	if (this->_textureToTrim) {
		SDL_DestroyTexture(this->_textureToTrim);
		this->_textureToTrim = NULL;
	}

	if (this->_trimedTexture) {
		SDL_DestroyTexture(this->_trimedTexture);
		this->_trimedTexture = NULL;
	}

	for (auto& animation : this->_animations) {
		delete animation.second;
	}
	this->_animations.clear();
}

SDL_Texture* BackGround::getAnimation(std::string shortName)
{
	auto animation = this->_animations[shortName];
	if (animation == nullptr) {
		return nullptr;
	}

	auto sprite = animation->sprites[this->_animationCounter];
	this->_animationCounter++;
	if (this->_animationCounter > 2) this->_animationCounter = 0;
	return sprite;
}

BackGround::~BackGround()
{
	for (auto& animation : this->_animations) {
		delete animation.second;
	}
	this->_animations.clear();
}

SDL_Texture* BackGround::_tryGetAnimationTexture(std::string path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL) { 
		return NULL; // its normal, there is posibility that file just not excist
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
	if (texture == NULL) {
		printf("unable to make texture from surface in background animation: %s\n", path.c_str());
		SDL_FreeSurface(surface);
		return NULL;
	}

	SDL_FreeSurface(surface);
	return texture;
}
