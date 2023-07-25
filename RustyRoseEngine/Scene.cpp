#include "Scene.h"

Scene::Scene(SDL_Renderer* renderer)
{
	this->_renderer = renderer;
	this->_texture = this->_texture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1280, 720);
	SDL_SetTextureBlendMode(this->_texture, SDL_BLENDMODE_BLEND);
}

void Scene::setFont(TTF_Font* font)
{
	this->_font = font;
}

void Scene::draw()
{
	SDL_RenderCopy(this->_renderer, this->_texture, NULL, NULL);
	SDL_RenderPresent(this->_renderer);
}

void Scene::clear(int layer)
{
	if (layer == -2) {
		this->_text.clear();
	}
	else if (layer == -1) {
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
	
	this->_text.clear();
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

void Scene::addText(std::string text)
{
	this->_text.push_back(text);
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

	// subtitles
	for (int i = 0; i < this->_text.size(); i++) {
		int w = 0, h = 0;
		SDL_Texture* text = this->_makeText(this->_text[i], w, h);
		SDL_Rect rect = {(1280 / 2) - (w / 2), h<120 ? 600 : 500, w, h}; // <- scaling
		SDL_RenderCopy(this->_renderer, text, NULL, &rect);
		SDL_DestroyTexture(text);
	}

	for (int i = 0; i < this->_sysImg2.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_sysImg2[i]->getTexture(), NULL, NULL);
	}

	SDL_SetRenderTarget(this->_renderer, NULL);
}

SDL_Texture* Scene::_makeText(std::string text, int& w, int& h)
{
	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Color outlineColor = { 0, 0, 0, 255 };

	SDL_Surface* outlineSurface = TTF_RenderText_Blended_Wrapped(this->_font, text.c_str(), outlineColor, 1100);
	if (outlineSurface == NULL) {
		printf("unable to make outline surface in text: %s\n", text.c_str());
		return nullptr;
	}

	SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(this->_renderer, outlineSurface);
	if (outlineTexture == NULL) {
		printf("unable to make texture from outline surface in text: %s\n", text.c_str());
		SDL_FreeSurface(outlineSurface);
		return nullptr;
	}

	SDL_Texture* finalTexture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, outlineSurface->w, outlineSurface->h);
	if (finalTexture == NULL) {
		printf("unable to make final texture in text: %s\n", text.c_str());
		SDL_FreeSurface(outlineSurface);
		return outlineTexture;
	}
	SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

	SDL_Rect rect = { 0, 0, outlineSurface->w, outlineSurface->h };
	SDL_SetRenderTarget(this->_renderer, finalTexture);

	w = rect.w;
	h = rect.h;

	rect.x = -3;
	SDL_RenderCopy(this->_renderer, outlineTexture, NULL, &rect);
	rect.x = 3;
	SDL_RenderCopy(this->_renderer, outlineTexture, NULL, &rect);
	rect.x = 0;
	rect.y = -3;
	SDL_RenderCopy(this->_renderer, outlineTexture, NULL, &rect);
	rect.y = 3;
	SDL_RenderCopy(this->_renderer, outlineTexture, NULL, &rect);
	rect.y = 0;

	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(this->_font, text.c_str(), textColor, 1100);
	if (textSurface == NULL) {
		printf("unable to make text surface in text: %s\n", text.c_str());
		SDL_FreeSurface(outlineSurface);
		SDL_DestroyTexture(outlineTexture);
		SDL_SetRenderTarget(this->_renderer, this->_texture);
		return finalTexture;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(this->_renderer, textSurface);
	if (textTexture == NULL) {
		printf("unable to make real text in text: %s\n", text.c_str());
		SDL_FreeSurface(outlineSurface);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(outlineTexture);
		SDL_SetRenderTarget(this->_renderer, this->_texture);
		return finalTexture;
	}

	SDL_RenderCopy(this->_renderer, textTexture, NULL, &rect);

	SDL_FreeSurface(outlineSurface);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(outlineTexture);
	SDL_DestroyTexture(textTexture);
	SDL_SetRenderTarget(this->_renderer, this->_texture);
	return finalTexture;
}
