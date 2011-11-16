#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include "config.h"
#include "data_manager.h"
#include <unordered_map>

namespace ygo {

class ImageManager {

public:
	bool Initial();
	void SetDevice(irr::IrrlichtDevice* dev);
	void ClearTexture();
	void RemoveTexture(int code);
	irr::video::ITexture* GetTexture(int code);
	irr::video::ITexture* GetTextureThumb(int code);
	
	std::unordered_map<int, irr::video::ITexture*> tMap;
	std::unordered_map<int, irr::video::ITexture*> tThumb;
	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::video::ITexture* tCover;
	irr::video::ITexture* tAct;
	irr::video::ITexture* tAttack;
	irr::video::ITexture* tNegated;
	irr::video::ITexture* tChain;
	irr::video::ITexture* tNumber;
	irr::video::ITexture* tLPFrame;
	irr::video::ITexture* tLPBar;
	irr::video::ITexture* tMask;
	irr::video::ITexture* tEquip;
	irr::video::ITexture* tLim;
	irr::video::ITexture* tBackGround;
};

}

#endif // IMAGEMANAGER_H
