#include <vector>
#include "SysImg.h"
#include "SoundEffect.h"

class System
{
public:
	System(SDL_Renderer* renderer); //, SDL_Font
	void setSystem(std::string path);
	bool loadSystem();
	void freeSystem();
	
	SoundEffect* getSoundEffect(std::string path);
	SysImg* getSystemImage(std::string path);
	//SDL_Font* getFont();

private:
	SDL_Renderer* _renderer;
	// add SDL_Font _font
	std::vector<std::string> _list;

	std::vector<SoundEffect*> _soundEffects;
	std::vector<SysImg*> _systemImages;

};
