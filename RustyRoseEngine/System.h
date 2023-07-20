#include <vector>
#include "SysImg.h"
#include "SoundEffect.h"
#include <fstream>
#include <SDL_ttf.h>

class System
{
public:
	System(SDL_Renderer* renderer);
	void setSystem(std::string path);
	void loadSystem();
	void freeSystem();
	
	SoundEffect* getSoundEffect(std::string path);
	SysImg* getSystemImage(std::string path);
	TTF_Font* getFont();

private:
	SDL_Renderer* _renderer;
	TTF_Font* _font;
	std::vector<std::string> _list;

	std::vector<SoundEffect*> _soundEffects;
	std::vector<SysImg*> _systemImages;

	void _setFont(std::string path);
	std::string _getFileName(std::string path);
	std::string _getFileFormat(std::string path);

};
