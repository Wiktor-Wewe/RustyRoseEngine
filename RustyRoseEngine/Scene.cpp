#include "Scene.h"

Scene::Scene(SDL_Renderer* renderer, TTF_Font* font)
{
	this->_renderer = renderer;
	this->_font = font;
	this->_texture = this->_texture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1280, 720);
	SDL_SetTextureBlendMode(this->_texture, SDL_BLENDMODE_BLEND);
}

void Scene::draw()
{
	SDL_RenderCopy(this->_renderer, this->_texture, NULL, NULL);
	SDL_RenderPresent(this->_renderer);
}

void Scene::clear(int layer)
{
	if (layer == -1) {
		this->_backGround0.clear();
		this->_backGround1.clear();
		this->_backGround2.clear();
		this->_sysImg0.clear();
		this->_sysImg1.clear();
		this->_sysImg2.clear();
	}
	else if (layer == 0) {
		this->_backGround0.clear();
		this->_sysImg0.clear();
	}
	else if (layer == 1) {
		this->_backGround1.clear();
		this->_sysImg1.clear();
	}
	else if (layer == 2) {
		this->_backGround2.clear();
		this->_sysImg2.clear();
	}
	else {
		printf("Cant clear layer with number: %i\n", layer);
		return;
	}
	
	
	SDL_SetRenderTarget(this->_renderer, this->_texture);
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->_renderer);
	SDL_SetRenderTarget(this->_renderer, NULL);
}

void Scene::addBackGround(BackGround* bg, int layer)
{
	if (layer == 0) {
		this->_backGround0.push_back(bg);
	}
	else if (layer == 1) {
		this->_backGround1.push_back(bg);
	}
	else if (layer == 2) {
		this->_backGround2.push_back(bg);
	}
	else {
		printf("There is no layer %i in scene->backgrounds\n", layer);
	}
}

void Scene::addSysImg(SysImg* sysimg, int layer)
{
	if (layer == 0) {
		this->_sysImg0.push_back(sysimg);
	}
	else if (layer == 1) {
		this->_sysImg1.push_back(sysimg);
	}
	else if (layer == 2) {
		this->_sysImg2.push_back(sysimg);
	}
	else {
		printf("There is no layer %i in scene->sysimg\n", layer);
	}
}

void Scene::makeTexture()
{
	SDL_SetRenderTarget(this->_renderer, this->_texture);

	// layer 0
	for (int i = 0; i < this->_backGround0.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_backGround0[i]->getTexture(), NULL, NULL);
	}
	for (int i = 0; i < this->_sysImg0.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_sysImg0[i]->getTexture(), NULL, NULL);
	}

	// layer 1
	for (int i = 0; i < this->_backGround1.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_backGround1[i]->getTexture(), NULL, NULL);
	}
	for (int i = 0; i < this->_sysImg1.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_sysImg1[i]->getTexture(), NULL, NULL);
	}

	// layer 2
	for (int i = 0; i < this->_backGround2.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_backGround2[i]->getTexture(), NULL, NULL);
	}
	for (int i = 0; i < this->_sysImg2.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_sysImg2[i]->getTexture(), NULL, NULL);
	}

	SDL_SetRenderTarget(this->_renderer, NULL);
}