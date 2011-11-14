#ifndef CLIENT_FIELD_H
#define CLIENT_FIELD_H

#include "config.h"
#include "client_card.h"
#include <vector>

namespace ygo {

struct ChainInfo{
	irr::core::vector3df chain_pos;
	ClientCard* chain_card;
	int code;
	int desc;
	int controler;
	int location;
	int sequence;
	bool solved;
};

class ClientField: public irr::IEventReceiver {
public:
	std::vector<ClientCard*> deck[2];
	std::vector<ClientCard*> hand[2];
	std::vector<ClientCard*> mzone[2];
	std::vector<ClientCard*> szone[2];
	std::vector<ClientCard*> grave[2];
	std::vector<ClientCard*> remove[2];
	std::vector<ClientCard*> extra[2];
	std::set<ClientCard*> overlay_cards;
	std::vector<ClientCard*> summonable_cards;
	std::vector<ClientCard*> spsummonable_cards;
	std::vector<ClientCard*> msetable_cards;
	std::vector<ClientCard*> ssetable_cards;
	std::vector<ClientCard*> reposable_cards;
	std::vector<ClientCard*> activatable_cards;
	std::vector<ClientCard*> attackable_cards;
	std::vector<int> activatable_descs;
	std::vector<int> select_options;
	std::vector<ChainInfo> chains;
	
	bool is_replaying;
	
	int selected_option;
	ClientCard* attacker;
	ClientCard* attack_target;
	int disabled_field;
	int selectable_field;
	int selected_field;
	int select_min;
	int select_max;
	int select_cancelable;
	int select_mode;
	bool select_ready;
	int announce_count;
	int select_counter_count;
	int select_counter_type;
	std::vector<ClientCard*> selectable_cards;
	std::vector<ClientCard*> selected_cards;
	std::set<ClientCard*> selectsum_cards;
	std::vector<ClientCard*> selectsum_all;
	std::vector<int> sort_list;
	bool grave_act;
	bool remove_act;
	bool deck_act;
	bool extra_act;
	ChainInfo current_chain;
	bool last_chain;
	
	ClientField();
	void Clear();
	void Initial(int player, int deckc, int extrac);
	ClientCard* GetCard(int controler, int location, int sequence);
	void AddCard(ClientCard* pcard, int controler, int location, int sequence);
	ClientCard* RemoveCard(int controler, int location, int sequence);
	void UpdateCard(int controler, int location, int sequence, char* data, int flag);
	void UpdateFieldCard(int controler, int location, char* data, int flag);
	void ClearCommandFlag();
	void ClearSelect();
	void ClearChainSelect();
	void ShowSelectCard(bool buttonok = false);

	void GetChainLocation(int controler, int location, int sequence, irr::core::vector3df* t);
	void GetCardLocation(ClientCard* pcard, irr::core::vector3df* t, irr::core::vector3df* r);
	void MoveCard(ClientCard* pcard, int frame);
	void FadeCard(ClientCard* pcard, int alpha, int frame);
	bool CheckSelectSum();
	bool check_min(std::set<ClientCard*>& left, std::set<ClientCard*>::iterator index, int min, int max);
	bool check_sel_sum_s(std::set<ClientCard*>& left, int index, int acc);
	void check_sel_sum_t(std::set<ClientCard*>& left, int acc);
	bool check_sum(std::set<ClientCard*>& testlist, std::set<ClientCard*>::iterator index, int acc, int count);
	
	irr::IrrlichtDevice* device;
	irr::gui::IGUIElement* panel;
	int hovered_controler;
	int hovered_location;
	int hovered_sequence;
	int command_controler;
	int command_location;
	int command_sequence;
	ClientCard* hovered_card;
	ClientCard* clicked_card;
	ClientCard* command_card;
	int list_command;
	wchar_t formatBuffer[256];

	virtual bool OnEvent(const irr::SEvent& event);
	void GetHoverField(int x, int y);
	void ShowMenu(int flag, int x, int y);
};

}

#endif //CLIENT_FIELD_H
