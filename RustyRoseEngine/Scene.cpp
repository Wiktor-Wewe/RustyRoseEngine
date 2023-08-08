#include "Scene.h"

Scene::Scene(SDL_Renderer* renderer)
{
	this->_renderer = renderer;
	this->_shortName = std::string();
	this->_textMutex = SDL_CreateMutex();
	this->_selectedOptionIndex = -1;
}

void Scene::setFont(TTF_Font* font)
{
	this->_font = font;
}

void Scene::draw()
{
	SDL_SetRenderTarget(this->_renderer, NULL);
	SDL_RenderClear(this->_renderer);

	// layer 0
	for (int i = 0; i < this->_backGround0.size(); i++) {
		SDL_LockMutex(this->_backGround0[i]->getMutex());
		SDL_RenderCopy(this->_renderer, this->_backGround0[i]->getTexture(), NULL, NULL);
		SDL_UnlockMutex(this->_backGround0[i]->getMutex());
	}

	if (!this->_shortName.empty()) {
		if (!this->_backGround0.empty()) {
			SDL_LockMutex(this->_backGround0[this->_backGround0.size() - 1]->getMutex());
			SDL_Texture* animation = this->_backGround0[this->_backGround0.size() - 1]->getNextAnimationTexture(this->_shortName);
			if (animation) {
				SDL_RenderCopy(this->_renderer, animation, NULL, NULL);
			}
			SDL_UnlockMutex(this->_backGround0[this->_backGround0.size() - 1]->getMutex());
		}
	}

	for (int i = 0; i < this->_sysImg0.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_sysImg0[i]->getTexture(), NULL, NULL);
	}

	SDL_RenderCopy(this->_renderer, this->_videoFrame, NULL, NULL);

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
	SDL_LockMutex(this->_textMutex);
	for (int i = 0; i < this->_text.size(); i++) {
		//printf("text [i=%i] | size = %i | %s\n", i, this->_text.size(), this->_text[i].c_str());
		SDL_RenderCopy(this->_renderer, this->_textTexture[i], NULL, this->_textRect[i]);
	}
	SDL_UnlockMutex(this->_textMutex);

	// select option
	for (int i = 0; i < this->_pathOptions.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_pathOptionsTexture[i], NULL, this->_pathOptionsRect[i]);
	}

	for (int i = 0; i < this->_sysImg2.size(); i++) {
		SDL_RenderCopy(this->_renderer, this->_sysImg2[i]->getTexture(), NULL, NULL);
	}

	SDL_RenderPresent(this->_renderer);
}

void Scene::setAnimationShortName(std::string shortName)
{
	this->_shortName = shortName;
;}

void Scene::setAnimationShortNameToDefalut()
{
	this->_shortName = std::string();
}

void Scene::setAnimationShortNameToDefaultIfName(std::string shortName)
{
	if (this->_shortName == shortName) {
		this->_shortName = std::string();
	}
}

BackGround* Scene::getLastBackGround(int layer)
{
	if (layer == 0) {
		if (this->_backGround0.empty()) {
			return nullptr;
		}
		return this->_backGround0[this->_backGround0.size() - 1];
	}
	else if (layer == 1) {
		if (this->_backGround1.empty()) {
			return nullptr;
		}
		return this->_backGround1[this->_backGround1.size() - 1];
	}
	else if (layer == 2) {
		if (this->_backGround2.empty()) {
			return nullptr;
		}
		return this->_backGround2[this->_backGround2.size() - 1];
	}
	else {
		printf("unable to gerlast background from scene - wrong layer\n");
	}
	return nullptr;
}

void Scene::clear(int layer)
{
	if (layer == -3) {
		this->_videoFrame = NULL;
	}
	else if (layer == -2) {
		for (int i = 0; i < this->_text.size(); i++) {
			this->removeText(this->_text[i]);
		}
	}
	else if (layer == -1) {
		this->_backGround0.clear();
		this->_backGround1.clear();
		this->_backGround2.clear();
		this->_sysImg0.clear();
		this->_sysImg1.clear();
		this->_sysImg2.clear();

		this->_pathOptions.clear();
		this->_pathOptionsTexture.clear();
		this->_pathOptionsRect.clear();

		this->_videoFrame = NULL;
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

void Scene::addVideoFrame(SDL_Texture* frame)
{
	this->_videoFrame = frame;
}

void Scene::addText(std::string text)
{
	SDL_LockMutex(this->_textMutex);

	int w = 0, h = 0;
	SDL_Texture* texture = this->_makeText(text, w, h);
	SDL_Rect* rect = new SDL_Rect { (1280 / 2) - (w / 2), (720 - h - 5), w, h };
	
	if (!this->_textRect.empty() && this->_textRect[0]->y >= rect->y) {
		for (int i = 0; i < this->_text.size(); i++) {
			rect->y -= this->_textRect[i]->h - 5;
		}
	}

	this->_textTexture.push_back(texture);
	this->_textRect.push_back(rect);
	this->_text.push_back(text);

	SDL_UnlockMutex(this->_textMutex);
}

void Scene::addPathOption(std::string option)
{
	this->_pathOptions.push_back(option);

	int w = 0, h = 0;
	SDL_Texture* texture = this->_makeText(option, w, h, { 255, 0, 0, 255 });
	this->_pathOptionsTexture.push_back(texture);

	SDL_Rect* rect = new SDL_Rect;
	rect->w = w;
	rect->h = h;
	rect->x = (1280 / 2) - (w / 2);
	this->_pathOptionsRect.push_back(rect);

	// update positions
	int segment = 720 / (this->_pathOptions.size() + 1);
	for (int i = 0; i < this->_pathOptions.size(); i++) {
		this->_pathOptionsRect[i]->y = (segment + (segment * i)) - (h / 2);
	}
}

void Scene::setPathOptionByDirection(int dy)
{
	int min = 0;
	int max = this->_pathOptions.size() - 1;

	int newIndex = this->_selectedOptionIndex + dy;

	if (newIndex >= min && newIndex <= max) {
		this->_selectedOptionIndex += dy;
	}
}

void Scene::setPathOptionByIndex(int index)
{
	int min = 0;
	int max = this->_pathOptions.size() - 1;

	if (index >= min && index >= max) {
		this->_selectedOptionIndex = index;
	}
}

void Scene::clearPathOption()
{
	this->_selectedOptionIndex = -1;
	this->_pathOptions.clear();
	this->_pathOptionsTexture.clear();
	this->_pathOptionsRect.clear();
}

void Scene::removeText(std::string text)
{
	SDL_LockMutex(this->_textMutex);

	int position = -1;
	for (int i = 0; i < this->_text.size(); i++) {
		if (this->_text[i] == text) {
			position = i;
			break;
		}
	}

	if (position == -1) {
		printf("unable to remove string from scene->text: %s\n", text.c_str());
		return;
	}


	this->_text.erase(this->_text.begin() + position);

	delete(this->_textRect[position]);
	this->_textRect.erase(this->_textRect.begin() + position);
	
	SDL_DestroyTexture(this->_textTexture[position]);
	this->_textTexture.erase(this->_textTexture.begin() + position);

	SDL_UnlockMutex(this->_textMutex);
}

void Scene::removeBackGround(BackGround* backGround, int layer)
{
	if (layer == -1) {
		this->_tryRemoveBg(backGround, this->_backGround0);
		this->_tryRemoveBg(backGround, this->_backGround1);
		this->_tryRemoveBg(backGround, this->_backGround2);
	}
	else if (layer == 0) {
		this->_tryRemoveBg(backGround, this->_backGround0);
	}
	else if (layer == 1) {
		this->_tryRemoveBg(backGround, this->_backGround1);
	}
	else if (layer == 2) {
		this->_tryRemoveBg(backGround, this->_backGround2);
	}
	else {
		printf("unable to remove bg from scene - wrong layer\n");
	}
}

SDL_Texture* Scene::_makeText(std::string text, int& w, int& h, SDL_Color textColor)
{
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
		SDL_SetRenderTarget(this->_renderer, NULL);
		return finalTexture;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(this->_renderer, textSurface);
	if (textTexture == NULL) {
		printf("unable to make real text in text: %s\n", text.c_str());
		SDL_FreeSurface(outlineSurface);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(outlineTexture);
		SDL_SetRenderTarget(this->_renderer, NULL);
		return finalTexture;
	}

	SDL_RenderCopy(this->_renderer, textTexture, NULL, &rect);

	SDL_FreeSurface(outlineSurface);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(outlineTexture);
	SDL_DestroyTexture(textTexture);
	SDL_SetRenderTarget(this->_renderer, NULL);
	return finalTexture;
}

void Scene::_tryRemoveBg(BackGround* bg, std::vector<BackGround*>& list)
{
	auto it = std::find(list.begin(), list.end(), bg);
	if (it != list.end()) {
		list.erase(it);
	}
}
