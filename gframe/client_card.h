#ifndef CLIENT_CARD_H
#define CLIENT_CARD_H

#include "config.h"
#include <vector>
#include <set>
#include <unordered_map>

namespace ygo {

struct CardData {
	unsigned int code;
	unsigned int alias;
	unsigned int setcode;
	unsigned int type;
	unsigned int level;
	unsigned int attribute;
	unsigned int race;
	int attack;
	int defence;
};
struct CardDataC {
	unsigned int code;
	unsigned int alias;
	unsigned int setcode;
	unsigned int type;
	unsigned int level;
	unsigned int attribute;
	unsigned int race;
	int attack;
	int defence;
	unsigned int category;
};
struct CardString {
	wchar_t* name;
	wchar_t* text;
	wchar_t* desc[16];
};
typedef std::unordered_map<unsigned int, CardDataC>::iterator code_pointer;

struct ClientCard {
public:
	irr::video::ITexture* curTexture;
	irr::core::matrix4 mTransform;
	irr::core::vector3df curPos;
	irr::core::vector3df curRot;
	irr::core::vector3df dPos;
	irr::core::vector3df dRot;
	u32 curAlpha;
	u32 dAlpha;
	u32 aniFrame;
	bool is_moving;
	bool is_fading;
	bool is_hovered;
	bool is_selectable;
	bool is_selected;
	bool is_showequip;
	bool is_showtarget;
	bool is_highlighting;
	u32 code;
	u32 alias;
	u32 type;
	u32 level;
	u32 rank;
	u32 attribute;
	u32 race;
	s32 attack;
	s32 defence;
	s32 base_attack;
	s32 base_defence;
	u32 reason;
	u32 select_seq;
	u8 owner;
	u8 controler;
	u8 location;
	u8 sequence;
	u8 position;
	u8 is_disabled;
	u8 is_public;
	u32 turnCounter;
	u32 opParam;
	u32 symbol;
	u32 cmdFlag;
	ClientCard* overlayTarget;
	std::vector<ClientCard*> overlayed;
	ClientCard* equipTarget;
	std::set<ClientCard*> equipped;
	std::map<int, int> counters;
	wchar_t atkstring[16];
	wchar_t defstring[16];
	wchar_t lvstring[4];

	ClientCard();
	void SetCode(int code);
	void UpdateInfo(char* buf, int flag);
	static bool client_card_sort(ClientCard* c1, ClientCard* c2);
	static bool deck_sort_atk(code_pointer p1, code_pointer p2);
	static bool deck_sort_def(code_pointer def, code_pointer p2);
	static bool deck_sort_lv(code_pointer lv, code_pointer p2);
};

}

#endif //CLIENT_CARD_H
