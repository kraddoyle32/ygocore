/*
 * field.cpp
 *
 *  Created on: 2010-7-21
 *      Author: Argon
 */

#include "field.h"
#include <iostream>

int32 field::field_used_count[32] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5};

field::field(duel* pduel) {
	this->pduel = pduel;
	infos.effect_id = 1;
	infos.copy_id = 1;
	infos.turn_player = 0;
	infos.turn_id = 0;
	infos.field_id = 1;
	infos.card_id = 1;
	for (int i = 0; i < 2; ++i) {
		cost[i].count = 0;
		cost[i].amount = 0;
		cost[i].lpstack[0] = 0;
		player[i].lp = 8000;
		player[i].start_count = 5;
		player[i].draw_count = 1;
		player[i].disabled_location = 0;
		player[i].used_location = 0;
		for (int j = 0; j < 5; ++j)
			player[i].list_mzone.push_back(0);
		for (int j = 0; j < 6; ++j)
			player[i].list_szone.push_back(0);
	}
	for (int i = 0; i < 5; ++i)
		core.pre_field[i] = 0;
	core.summoning_card = 0;
	core.spsummoning_card = 0;
	core.summon_depth = 0;
	core.chain_limit = 0;
	core.chain_limit_p = 0;
	core.win_player = 5;
	core.win_reason = 0;
	core.reason_effect = 0;
	core.reason_player = PLAYER_NONE;
	core.selfdes_disabled = FALSE;
	core.flip_delayed = FALSE;
	core.overdraw[0] = FALSE;
	core.overdraw[1] = FALSE;
	core.check_level = 0;
	core.limit_tuner = 0;
	core.duel_options = 0;
	core.attacker = 0;
	core.attack_target = 0;
	nil_event.event_code = 0;
	nil_event.event_cards = 0;
	nil_event.event_player = PLAYER_NONE;
	nil_event.event_value = 0;
	nil_event.reason = 0;
	nil_event.reason_effect = 0;
	nil_event.reason_player = PLAYER_NONE;
}
field::~field() {

}
void field::add_card(uint8 playerid, card* pcard, uint8 location, uint8 sequence) {
	if (pcard->current.location != 0)
		return;
	if (!is_location_useable(playerid, location, sequence))
		return;
	if ((pcard->data.type & (TYPE_FUSION | TYPE_SYNCHRO | TYPE_XYZ)) && (location == LOCATION_HAND || location == LOCATION_DECK))
		location = LOCATION_EXTRA;
	pcard->current.controler = playerid;
	pcard->current.location = location;
	switch (location) {
	case LOCATION_MZONE:
		player[playerid].list_mzone[sequence] = pcard;
		pcard->current.sequence = sequence;
		break;
	case LOCATION_SZONE:
		player[playerid].list_szone[sequence] = pcard;
		pcard->current.sequence = sequence;
		break;
	case LOCATION_DECK:
		if (sequence == 0) {		//deck top
			player[playerid].list_main.push_back(pcard);
			pcard->iter = --(player[playerid].list_main.end());
			pcard->current.sequence = player[playerid].list_main.size() - 1;
		} else if (sequence == 1) {		//deck button
			player[playerid].list_main.push_front(pcard);
			pcard->iter = player[playerid].list_main.begin();
			reset_sequence(playerid, LOCATION_DECK);
		} else {		//deck top & shuffle
			player[playerid].list_main.push_back(pcard);
			pcard->iter = --(player[playerid].list_main.end());
			pcard->current.sequence = player[playerid].list_main.size() - 1;
			if(!core.shuffle_check_disabled)
				core.shuffle_deck_check[playerid] = TRUE;
		}
		break;
	case LOCATION_HAND:
		player[playerid].list_hand.push_back(pcard);
		pcard->iter = --(player[playerid].list_hand.end());
		pcard->current.sequence = player[playerid].list_hand.size() - 1;
		if(!(pcard->current.reason & REASON_DRAW) && !core.shuffle_check_disabled)
			core.shuffle_hand_check[playerid] = TRUE;
		break;
	case LOCATION_GRAVE:
		player[playerid].list_grave.push_back(pcard);
		pcard->iter = --(player[playerid].list_grave.end());
		pcard->current.sequence = player[playerid].list_grave.size() - 1;
		break;
	case LOCATION_REMOVED:
		player[playerid].list_remove.push_back(pcard);
		pcard->iter = --(player[playerid].list_remove.end());
		pcard->current.sequence = player[playerid].list_remove.size() - 1;
		break;
	case LOCATION_EXTRA:
		player[playerid].list_extra.push_back(pcard);
		pcard->iter = --(player[playerid].list_extra.end());
		pcard->current.sequence = player[playerid].list_extra.size() - 1;
		break;
	}
	pcard->apply_field_effect();
	pcard->fieldid = infos.field_id++;
	pcard->turnid = infos.turn_id;
	if (location == LOCATION_MZONE)
		player[playerid].used_location |= 1 << sequence;
	if (location == LOCATION_SZONE)
		player[playerid].used_location |= 256 << sequence;
}
void field::remove_card(card* pcard) {
	if (pcard->current.controler == PLAYER_NONE || pcard->current.location == 0)
		return;
	uint8 playerid = pcard->current.controler;
	card_list::iterator cit;
	switch (pcard->current.location) {
	case LOCATION_MZONE:
		player[playerid].list_mzone[pcard->current.sequence] = 0;
		break;
	case LOCATION_SZONE:
		player[playerid].list_szone[pcard->current.sequence] = 0;
		break;
	case LOCATION_DECK:
		player[playerid].list_main.erase(pcard->iter);
		reset_sequence(playerid, LOCATION_DECK);
		if(!core.shuffle_check_disabled)
			core.shuffle_deck_check[playerid] = TRUE;
		break;
	case LOCATION_HAND:
		player[playerid].list_hand.erase(pcard->iter);
		reset_sequence(playerid, LOCATION_HAND);
		break;
	case LOCATION_GRAVE:
		player[playerid].list_grave.erase(pcard->iter);
		reset_sequence(playerid, LOCATION_GRAVE);
		break;
	case LOCATION_REMOVED:
		player[playerid].list_remove.erase(pcard->iter);
		reset_sequence(playerid, LOCATION_REMOVED);
		break;
	case LOCATION_EXTRA:
		player[playerid].list_extra.erase(pcard->iter);
		reset_sequence(playerid, LOCATION_EXTRA);
		break;
	}
	pcard->cancel_field_effect();
	if (pcard->current.location == LOCATION_MZONE)
		player[playerid].used_location &= ~(1 << pcard->current.sequence);
	if (pcard->current.location == LOCATION_SZONE)
		player[playerid].used_location &= ~(256 << pcard->current.sequence);
	pcard->previous.controler = pcard->current.controler;
	pcard->previous.location = pcard->current.location;
	pcard->previous.sequence = pcard->current.sequence;
	pcard->previous.position = pcard->current.position;
	pcard->current.controler = PLAYER_NONE;
	pcard->current.location = 0;
	pcard->current.sequence = 0;
}
void field::move_card(uint8 playerid, card* pcard, uint8 location, uint8 sequence) {
	if (!is_location_useable(playerid, location, sequence))
		return;
	uint8 preplayer = pcard->current.controler;
	uint8 presequence = pcard->current.sequence;
	if (pcard->current.location) {
		if (pcard->current.location == location) {
			if (pcard->current.location == LOCATION_DECK) {
				if(preplayer == playerid) {
					pduel->write_buffer8(MSG_MOVE);
					pduel->write_buffer32(pcard->data.code);
					pduel->write_buffer32(pcard->get_info_location());
					player[preplayer].list_main.remove(pcard);
					if (sequence == 0) {		//deck top
						player[playerid].list_main.push_back(pcard);
						pcard->iter = --(player[playerid].list_main.end());
						pcard->current.sequence = player[playerid].list_main.size() - 1;
					} else if (sequence == 1) {
						player[playerid].list_main.push_front(pcard);
						pcard->iter = player[playerid].list_main.begin();
						reset_sequence(playerid, LOCATION_DECK);
					} else {
						player[playerid].list_main.push_back(pcard);
						pcard->iter = --(player[playerid].list_main.end());
						pcard->current.sequence = player[playerid].list_main.size() - 1;
						if(!core.shuffle_check_disabled)
							core.shuffle_deck_check[playerid] = true;
					}
					pcard->previous.controler = preplayer;
					pcard->current.controler = playerid;
					pduel->write_buffer32(pcard->get_info_location());
					return;
				} else
					remove_card(pcard);
			} else if(location & LOCATION_ONFIELD) {
				if (playerid == preplayer && sequence == presequence)
					return;
				if((location == LOCATION_MZONE && (sequence < 0 || sequence > 4 || player[playerid].list_mzone[sequence]))
				        || (location == LOCATION_SZONE && (sequence < 0 || sequence > 4 || player[playerid].list_szone[sequence])))
					return;
				if(preplayer == playerid) {
					pduel->write_buffer8(MSG_MOVE);
					pduel->write_buffer32(pcard->data.code);
					pduel->write_buffer32(pcard->get_info_location());
				}
				pcard->previous.controler = pcard->current.controler;
				pcard->previous.location = pcard->current.location;
				pcard->previous.sequence = pcard->current.sequence;
				pcard->previous.position = pcard->current.position;
				if (location == LOCATION_MZONE) {
					player[preplayer].list_mzone[presequence] = 0;
					player[preplayer].used_location &= ~(1 << presequence);
					player[playerid].list_mzone[sequence] = pcard;
					player[playerid].used_location |= 1 << sequence;
					pcard->current.controler = playerid;
					pcard->current.sequence = sequence;
				} else {
					player[preplayer].list_szone[presequence] = 0;
					player[preplayer].used_location &= ~(256 << presequence);
					player[playerid].list_szone[sequence] = pcard;
					player[playerid].used_location |= 256 << sequence;
					pcard->current.controler = playerid;
					pcard->current.sequence = sequence;
				}
				if(preplayer == playerid) {
					pduel->write_buffer32(pcard->get_info_location());
				}
				return;
			} else {
				pduel->write_buffer8(MSG_MOVE);
				pduel->write_buffer32(pcard->data.code);
				pduel->write_buffer32(pcard->get_info_location());
				if(location == LOCATION_GRAVE) {
					player[pcard->current.controler].list_grave.erase(pcard->iter);
					player[pcard->current.controler].list_grave.push_back(pcard);
					pcard->iter = --(player[pcard->current.controler].list_grave.end());
					reset_sequence(pcard->current.controler, LOCATION_GRAVE);
				} else if(location == LOCATION_REMOVED) {
					player[pcard->current.controler].list_remove.erase(pcard->iter);
					player[pcard->current.controler].list_remove.push_back(pcard);
					pcard->iter = --(player[pcard->current.controler].list_remove.end());
					reset_sequence(pcard->current.controler, LOCATION_REMOVED);
				} else {
					player[pcard->current.controler].list_extra.erase(pcard->iter);
					player[pcard->current.controler].list_extra.push_back(pcard);
					pcard->iter = --(player[pcard->current.controler].list_extra.end());
					reset_sequence(pcard->current.controler, LOCATION_EXTRA);
				}
				pduel->write_buffer32(pcard->get_info_location());
				return;
			}
		} else
			remove_card(pcard);
	}
	add_card(playerid, pcard, location, sequence);
}
void field::set_control(card* pcard, uint8 playerid, uint8 reset_phase, uint8 reset_count) {
	if(pcard->refresh_control_status() == playerid)
		return;
	effect* peffect = pduel->new_effect();
	if(core.reason_effect)
		peffect->owner = core.reason_effect->handler;
	else
		peffect->owner = pcard;
	peffect->handler = pcard;
	peffect->type = EFFECT_TYPE_SINGLE;
	peffect->code = EFFECT_SET_CONTROL;
	peffect->value = playerid;
	peffect->flag = EFFECT_FLAG_CANNOT_DISABLE;
	peffect->reset_flag = RESET_EVENT | 0xec0000;
	if(reset_count) {
		peffect->reset_flag |= RESET_PHASE | reset_phase;
		if(!(peffect->reset_flag & (RESET_SELF_TURN | RESET_OPPO_TURN)))
			peffect->reset_flag |= (RESET_SELF_TURN | RESET_OPPO_TURN);
		peffect->reset_count |= reset_count & 0xff;
	}
	pcard->add_effect(peffect);
	pcard->current.controler = playerid;
}

card* field::get_field_card(uint8 playerid, uint8 location, uint8 sequence) {
	if (location != LOCATION_MZONE && location != LOCATION_SZONE)
		return 0;
	if (location == LOCATION_MZONE)
		return player[playerid].list_mzone[sequence];
	else
		return player[playerid].list_szone[sequence];
}
int32 field::is_location_useable(uint8 playerid, uint8 location, uint8 sequence) {
	if (location != LOCATION_MZONE && location != LOCATION_SZONE)
		return TRUE;
	if (!(location == LOCATION_SZONE && sequence == 5) && get_useable_count(playerid, location) == 0)
		return FALSE;
	int32 flag = player[playerid].disabled_location | player[playerid].used_location;
	if (location == LOCATION_MZONE && flag & (1 << sequence))
		return FALSE;
	if (location == LOCATION_SZONE && flag & (256 << sequence))
		return FALSE;
	return TRUE;
}
int32 field::get_useable_count(uint8 playerid, uint8 location, uint32* list) {
	if (location != LOCATION_MZONE && location != LOCATION_SZONE)
		return 0;
	uint32 flag = player[playerid].disabled_location | player[playerid].used_location;
	if (location == LOCATION_MZONE)
		flag = (flag & 0x1f);
	else
		flag = (flag & 0x1f00) >> 8;
	if(list)
		*list = flag;
	return 5 - field_used_count[flag];
}
void field::shuffle(uint8 playerid, uint8 location) {
	if(!(location & (LOCATION_HAND + LOCATION_DECK)))
		return;
	card* tcard[100];
	card* t;
	card_list& l = (location == LOCATION_HAND) ? player[playerid].list_hand : player[playerid].list_main;
	if(l.size() == 0)
		return;
	if(l.size() > 1) {
		card_list::iterator cit;
		uint32 i = 0, j, r;
		for(cit = l.begin(); cit != l.end(); ++cit)
			tcard[i++] = *cit;
		for(j = 0; j < i; ++j) {
			r = pduel->get_next_integer(0, i - 1);
			t = tcard[j];
			tcard[j] = tcard[r];
			tcard[r] = t;
		}
		l.clear();
		for(j = 0; j < i; ++j) {
			l.push_back(tcard[j]);
			tcard[j]->current.sequence = j;
			tcard[j]->iter = --l.end();
		}
		reset_sequence(playerid, location);
	}
	if(location == LOCATION_HAND) {
		pduel->write_buffer8(MSG_SHUFFLE_HAND);
		core.shuffle_hand_check[playerid] = FALSE;
	} else {
		pduel->write_buffer8(MSG_SHUFFLE_DECK);
		core.shuffle_deck_check[playerid] = FALSE;
	}
	pduel->write_buffer8(playerid);
}
void field::reset_sequence(uint8 playerid, uint8 location) {
	if(location & (LOCATION_ONFIELD))
		return;
	card_list::iterator cit;
	uint32 i = 0;
	switch(location) {
	case LOCATION_DECK:
		for(cit = player[playerid].list_main.begin(), i = 0; cit != player[playerid].list_main.end(); ++cit, ++i)
			(*cit)->current.sequence = i;
		break;
	case LOCATION_HAND:
		for(cit = player[playerid].list_hand.begin(), i = 0; cit != player[playerid].list_hand.end(); ++cit, ++i)
			(*cit)->current.sequence = i;
		break;
	case LOCATION_EXTRA:
		for(cit = player[playerid].list_extra.begin(), i = 0; cit != player[playerid].list_extra.end(); ++cit, ++i)
			(*cit)->current.sequence = i;
		break;
	case LOCATION_GRAVE:
		for(cit = player[playerid].list_grave.begin(), i = 0; cit != player[playerid].list_grave.end(); ++cit, ++i)
			(*cit)->current.sequence = i;
		break;
	case LOCATION_REMOVED:
		for(cit = player[playerid].list_remove.begin(), i = 0; cit != player[playerid].list_remove.end(); ++cit, ++i)
			(*cit)->current.sequence = i;
		break;
	}
}
void field::swap_deck_and_grave(uint8 playerid) {
	card_list::iterator clit;
	for(clit = player[playerid].list_grave.begin(); clit != player[playerid].list_grave.end(); ++clit) {
		(*clit)->previous.location = LOCATION_GRAVE;
		(*clit)->previous.sequence = (*clit)->current.sequence;
		(*clit)->enable_field_effect(false);
		(*clit)->cancel_field_effect();
	}
	for(clit = player[playerid].list_main.begin(); clit != player[playerid].list_main.end(); ++clit) {
		(*clit)->previous.location = LOCATION_DECK;
		(*clit)->previous.sequence = (*clit)->current.sequence;
		(*clit)->enable_field_effect(false);
		(*clit)->cancel_field_effect();
	}
	card_list cl = player[playerid].list_grave;
	player[playerid].list_grave = player[playerid].list_main;
	player[playerid].list_main = cl;
	for(clit = player[playerid].list_grave.begin(); clit != player[playerid].list_grave.end(); ++clit) {
		(*clit)->current.location = LOCATION_GRAVE;
		(*clit)->iter = clit;
		(*clit)->current.reason = REASON_EFFECT;
		(*clit)->current.reason_effect = core.reason_effect;
		(*clit)->current.reason_player = core.reason_player;
		(*clit)->apply_field_effect();
		(*clit)->enable_field_effect(true);
	}
	for(clit = player[playerid].list_main.begin(); clit != player[playerid].list_main.end(); ++clit) {
		(*clit)->current.location = LOCATION_DECK;
		(*clit)->iter = clit;
		(*clit)->current.reason = REASON_EFFECT;
		(*clit)->current.reason_effect = core.reason_effect;
		(*clit)->current.reason_player = core.reason_player;
		(*clit)->apply_field_effect();
		(*clit)->enable_field_effect(true);
	}
	reset_sequence(playerid, LOCATION_GRAVE);
	pduel->write_buffer8(MSG_SWAP_GRAVE_DECK);
	pduel->write_buffer8(playerid);
	shuffle(playerid, LOCATION_DECK);
}
void field::add_effect(effect* peffect, uint8 owner_player) {
	if (!peffect->handler) {
		peffect->flag |= EFFECT_FLAG_FIELD_ONLY;
		peffect->handler = peffect->owner;
		peffect->effect_owner = owner_player;
		peffect->id = infos.effect_id++;
	}
	effect_container::iterator it;
	if (!(peffect->type & EFFECT_TYPE_ACTIONS))
		it = effects.aura_effect.insert(make_pair(peffect->code, peffect));
	else {
		if (peffect->type & EFFECT_TYPE_IGNITION)
			it = effects.startup_effect.insert(make_pair(peffect->code, peffect));
		else if (peffect->type & EFFECT_TYPE_ACTIVATE)
			it = effects.activate_effect.insert(make_pair(peffect->code, peffect));
		else if (peffect->type & EFFECT_TYPE_TRIGGER_O && peffect->type & EFFECT_TYPE_FIELD)
			it = effects.trigger_o_effect.insert(make_pair(peffect->code, peffect));
		else if (peffect->type & EFFECT_TYPE_TRIGGER_F && peffect->type & EFFECT_TYPE_FIELD)
			it = effects.trigger_f_effect.insert(make_pair(peffect->code, peffect));
		else if (peffect->type & EFFECT_TYPE_QUICK_O)
			it = effects.instant_o_effect.insert(make_pair(peffect->code, peffect));
		else if (peffect->type & EFFECT_TYPE_QUICK_F)
			it = effects.instant_f_effect.insert(make_pair(peffect->code, peffect));
		else if (peffect->type & EFFECT_TYPE_CONTINUOUS)
			it = effects.continuous_effect.insert(make_pair(peffect->code, peffect));
	}
	effects.indexer.insert(make_pair(peffect, it));
	if((peffect->flag & EFFECT_FLAG_FIELD_ONLY)) {
		if(peffect->flag & EFFECT_FLAG_OATH)
			effects.oath.insert(make_pair(peffect, core.reason_effect));
		if(peffect->reset_flag & RESET_PHASE)
			effects.pheff.insert(peffect);
		if(peffect->reset_flag & RESET_CHAIN)
			effects.cheff.insert(peffect);
		if(peffect->flag & EFFECT_FLAG_COUNT_LIMIT)
			effects.rechargeable.insert(peffect);
	}
}
void field::remove_effect(effect* peffect) {
	effect_container::iterator it;
	if (effects.indexer.find(peffect) == effects.indexer.end())
		return;
	it = effects.indexer[peffect];
	if (!(peffect->type & EFFECT_TYPE_ACTIONS))
		effects.aura_effect.erase(it);
	else {
		if (peffect->type & EFFECT_TYPE_IGNITION)
			effects.startup_effect.erase(it);
		else if (peffect->type & EFFECT_TYPE_ACTIVATE)
			effects.activate_effect.erase(it);
		else if (peffect->type & EFFECT_TYPE_TRIGGER_O)
			effects.trigger_o_effect.erase(it);
		else if (peffect->type & EFFECT_TYPE_TRIGGER_F)
			effects.trigger_f_effect.erase(it);
		else if (peffect->type & EFFECT_TYPE_QUICK_O)
			effects.instant_o_effect.erase(it);
		else if (peffect->type & EFFECT_TYPE_QUICK_F)
			effects.instant_f_effect.erase(it);
		else if (peffect->type & EFFECT_TYPE_CONTINUOUS)
			effects.continuous_effect.erase(it);
	}
	effects.indexer.erase(peffect);
	if((peffect->flag & EFFECT_FLAG_FIELD_ONLY)) {
		if(peffect->flag & EFFECT_FLAG_OATH)
			effects.oath.erase(peffect);
		if(peffect->reset_flag & RESET_PHASE)
			effects.pheff.erase(peffect);
		if(peffect->reset_flag & RESET_CHAIN)
			effects.cheff.erase(peffect);
		if(peffect->flag & EFFECT_FLAG_COUNT_LIMIT)
			effects.rechargeable.erase(peffect);
		core.reseted_effects.insert(peffect);
	}
}
void field::remove_oath_effect(effect* reason_effect) {
	oath_effects::iterator oeit, rm;
	for(oeit = effects.oath.begin(); oeit != effects.oath.end();) {
		rm = oeit++;
		if(rm->second == reason_effect) {
			effects.oath.erase(rm);
			if(rm->first->flag & EFFECT_FLAG_FIELD_ONLY)
				remove_effect(rm->first);
			else
				rm->first->handler->remove_effect(rm->first);
		}
	}
}
void field::reset_effect(uint32 id, uint32 reset_type) {
	effect_indexer::iterator it, rm;
	effect_container::iterator pit;
	effect* peffect;
	int32 result;
	for (it = effects.indexer.begin(); it != effects.indexer.end(); ++it) {
		rm = it++;
		peffect = rm->first;
		pit = rm->second;
		if (!(peffect->flag & EFFECT_FLAG_FIELD_ONLY))
			continue;
		result = peffect->reset(id, reset_type);
		if (result) {
			if (!(peffect->type & EFFECT_TYPE_ACTIONS)) {
				if (peffect->is_disable_related())
					update_disable_check_list(peffect);
				effects.aura_effect.erase(pit);
			} else {
				if (peffect->type & EFFECT_TYPE_IGNITION)
					effects.startup_effect.erase(pit);
				else if (peffect->type & EFFECT_TYPE_ACTIVATE)
					effects.activate_effect.erase(pit);
				else if (peffect->type & EFFECT_TYPE_TRIGGER_O)
					effects.trigger_o_effect.erase(pit);
				else if (peffect->type & EFFECT_TYPE_TRIGGER_F)
					effects.trigger_f_effect.erase(pit);
				else if (peffect->type & EFFECT_TYPE_QUICK_O)
					effects.instant_o_effect.erase(pit);
				else if (peffect->type & EFFECT_TYPE_QUICK_F)
					effects.instant_f_effect.erase(pit);
				else if (peffect->type & EFFECT_TYPE_CONTINUOUS)
					effects.continuous_effect.erase(pit);
			}
			effects.indexer.erase(peffect);
			pduel->delete_effect(peffect);
		}
	}
}
void field::reset_phase(uint32 phase) {
	field_effect::effect_collection::iterator eit, rm;
	for(eit = effects.pheff.begin(); eit != effects.pheff.end();) {
		rm = eit++;
		if((*rm)->reset(phase, RESET_PHASE)) {
			if((*rm)->flag & EFFECT_FLAG_FIELD_ONLY)
				remove_effect((*rm));
			else
				(*rm)->handler->remove_effect((*rm));
		}
	}
}
void field::reset_chain() {
	field_effect::effect_collection::iterator eit, rm;
	for(eit = effects.cheff.begin(); eit != effects.cheff.end();) {
		rm = eit++;
		if((*rm)->flag & EFFECT_FLAG_FIELD_ONLY)
			remove_effect((*rm));
		else
			(*rm)->handler->remove_effect((*rm));
	}
}
void field::filter_field_effect(uint32 code, effect_set* eset, uint8 sort) {
	effect* peffect;
	pair<effect_container::iterator, effect_container::iterator> rg;
	rg = effects.aura_effect.equal_range(code);
	for (; rg.first != rg.second; ++rg.first) {
		peffect = rg.first->second;
		if (peffect->is_available())
			eset->add_item(peffect);
	}
	if(sort)
		eset->sort();
}
void field::filter_affected_cards(effect* peffect, card_set* cset) {
	if ((peffect->type & EFFECT_TYPE_ACTIONS) || !(peffect->type & EFFECT_TYPE_FIELD) || (peffect->flag & EFFECT_FLAG_PLAYER_TARGET))
		return;
	uint8 self = peffect->get_handler_player();
	if(self == PLAYER_NONE)
		return;
	card* pcard;
	card_list::iterator it;
	uint16 range = peffect->s_range;
	for(uint32 p = 0; p < 2; ++p) {
		if (range & LOCATION_MZONE) {
			for (int i = 0; i < 5; ++i) {
				pcard = player[self].list_mzone[i];
				if (pcard && peffect->is_target(pcard))
					cset->insert(pcard);
			}
		}
		if (range & LOCATION_SZONE) {
			for (int i = 0; i < 6; ++i) {
				pcard = player[self].list_szone[i];
				if (pcard && peffect->is_target(pcard))
					cset->insert(pcard);
			}
		}
		if (range & LOCATION_GRAVE) {
			for (it = player[self].list_grave.begin(); it != player[self].list_grave.end(); ++it) {
				pcard = *it;
				if (peffect->is_target(pcard))
					cset->insert(pcard);
			}
		}
		if (range & LOCATION_REMOVED) {
			for (it = player[self].list_remove.begin(); it != player[self].list_remove.end(); ++it) {
				pcard = *it;
				if (peffect->is_target(pcard))
					cset->insert(pcard);
			}
		}
		if (range & LOCATION_HAND) {
			for (it = player[self].list_hand.begin(); it != player[self].list_hand.end(); ++it) {
				pcard = *it;
				if (peffect->is_target(pcard))
					cset->insert(pcard);
			}
		}
		range = peffect->o_range;
		self = 1 - self;
	}
}
void field::filter_player_effect(uint8 playerid, uint32 code, effect_set* eset, uint8 sort) {
	effect* peffect;
	pair<effect_container::iterator, effect_container::iterator> rg;
	rg = effects.aura_effect.equal_range(code);
	for (; rg.first != rg.second; ++rg.first) {
		peffect = rg.first->second;
		if (peffect->is_target_player(playerid) && peffect->is_available())
			eset->add_item(peffect);
	}
	if(sort)
		eset->sort();
}
int32 field::filter_matching_card(int32 findex, uint8 self, uint32 location1, uint32 location2, group* pgroup, card* pexception, uint32 extraargs, card** pret, int32 fcount, int32 is_target) {
	if(self != 0 && self != 1)
		return FALSE;
	card* pcard;
	int32 count = 0;
	uint32 location = location1;
	for(uint32 p = 0; p < 2; ++p) {
		if(location & LOCATION_MZONE) {
			for(uint32 i = 0; i < 5; ++i) {
				pcard = player[self].list_mzone[i];
				if(pcard && !pcard->is_status(STATUS_SUMMONING) && !pcard->is_status(STATUS_SUMMON_DISABLED) && pcard != pexception
				        && pduel->lua->check_matching(pcard, findex, extraargs) && (!is_target || pcard->is_capable_be_effect_target(core.reason_effect, core.reason_player))) {
					if(pret) {
						*pret = pcard;
						return TRUE;
					}
					count ++;
					if(fcount && count >= fcount)
						return TRUE;
					if(pgroup) {
						pgroup->container.insert(pcard);
					}
				}
			}
		}
		if(location & LOCATION_SZONE) {
			for(uint32 i = 0; i < 6; ++i) {
				pcard = player[self].list_szone[i];
				if(pcard && pcard != pexception && pduel->lua->check_matching(pcard, findex, extraargs)
				        && (!is_target || pcard->is_capable_be_effect_target(core.reason_effect, core.reason_player))) {
					if(pret) {
						*pret = pcard;
						return TRUE;
					}
					count ++;
					if(fcount && count >= fcount)
						return TRUE;
					if(pgroup) {
						pgroup->container.insert(pcard);
					}
				}
			}
		}
		if(location & LOCATION_DECK) {
			for(card_list::reverse_iterator cit = player[self].list_main.rbegin(); cit != player[self].list_main.rend(); ++cit) {
				if(*cit != pexception && pduel->lua->check_matching(*cit, findex, extraargs)
				        && (!is_target || (*cit)->is_capable_be_effect_target(core.reason_effect, core.reason_player))) {
					if(pret) {
						*pret = *cit;
						return TRUE;
					}
					count ++;
					if(fcount && count >= fcount)
						return TRUE;
					if(pgroup) {
						pgroup->container.insert(*cit);
					}
				}
			}
		}
		if(location & LOCATION_EXTRA) {
			for(card_list::reverse_iterator cit = player[self].list_extra.rbegin(); cit != player[self].list_extra.rend(); ++cit) {
				if(*cit != pexception && pduel->lua->check_matching(*cit, findex, extraargs)
				        && (!is_target || (*cit)->is_capable_be_effect_target(core.reason_effect, core.reason_player))) {
					if(pret) {
						*pret = *cit;
						return TRUE;
					}
					count ++;
					if(fcount && count >= fcount)
						return TRUE;
					if(pgroup) {
						pgroup->container.insert(*cit);
					}
				}
			}
		}
		if(location & LOCATION_HAND) {
			for(card_list::iterator cit = player[self].list_hand.begin(); cit != player[self].list_hand.end(); ++cit) {
				if(*cit != pexception && pduel->lua->check_matching(*cit, findex, extraargs)
				        && (!is_target || (*cit)->is_capable_be_effect_target(core.reason_effect, core.reason_player))) {
					if(pret) {
						*pret = *cit;
						return TRUE;
					}
					count ++;
					if(fcount && count >= fcount)
						return TRUE;
					if(pgroup) {
						pgroup->container.insert(*cit);
					}
				}
			}
		}
		if(location & LOCATION_GRAVE) {
			for(card_list::reverse_iterator cit = player[self].list_grave.rbegin(); cit != player[self].list_grave.rend(); ++cit) {
				if(*cit != pexception && pduel->lua->check_matching(*cit, findex, extraargs)
				        && (!is_target || (*cit)->is_capable_be_effect_target(core.reason_effect, core.reason_player))) {
					if(pret) {
						*pret = *cit;
						return TRUE;
					}
					count ++;
					if(fcount && count >= fcount)
						return TRUE;
					if(pgroup) {
						pgroup->container.insert(*cit);
					}
				}
			}
		}
		if(location & LOCATION_REMOVED) {
			for(card_list::reverse_iterator cit = player[self].list_remove.rbegin(); cit != player[self].list_remove.rend(); ++cit) {
				if(*cit != pexception && pduel->lua->check_matching(*cit, findex, extraargs)
				        && (!is_target || (*cit)->is_capable_be_effect_target(core.reason_effect, core.reason_player))) {
					if(pret) {
						*pret = *cit;
						return TRUE;
					}
					count ++;
					if(fcount && count >= fcount)
						return TRUE;
					if(pgroup) {
						pgroup->container.insert(*cit);
					}
				}
			}
		}
		location = location2;
		self = 1 - self;
	}
	return FALSE;
}
int32 field::filter_field_card(uint8 self, uint32 location1, uint32 location2, group* pgroup) {
	if(self != 0 && self != 1)
		return 0;
	uint32 location = location1;
	uint32 count = 0;
	card* pcard;
	for(uint32 p = 0; p < 2; ++p) {
		if(location & LOCATION_MZONE) {
			for(int i = 0; i < 5; ++i) {
				pcard = player[self].list_mzone[i];
				if(pcard && !pcard->is_status(STATUS_SUMMONING)) {
					if(pgroup)
						pgroup->container.insert(pcard);
					count++;
				}
			}
		}
		if(location & LOCATION_SZONE) {
			for(int i = 0; i < 6; ++i) {
				pcard = player[self].list_szone[i];
				if(pcard) {
					if(pgroup)
						pgroup->container.insert(pcard);
					count++;
				}
			}
		}
		if(location & LOCATION_HAND) {
			if(pgroup)
				for(card_list::iterator cit = player[self].list_hand.begin(); cit != player[self].list_hand.end(); ++cit)
					pgroup->container.insert(*cit);
			count += player[self].list_hand.size();
		}
		if(location & LOCATION_DECK) {
			if(pgroup)
				for(card_list::reverse_iterator cit = player[self].list_main.rbegin(); cit != player[self].list_main.rend(); ++cit)
					pgroup->container.insert(*cit);
			count += player[self].list_main.size();
		}
		if(location & LOCATION_EXTRA) {
			if(pgroup)
				for(card_list::reverse_iterator cit = player[self].list_extra.rbegin(); cit != player[self].list_extra.rend(); ++cit)
					pgroup->container.insert(*cit);
			count += player[self].list_extra.size();
		}
		if(location & LOCATION_GRAVE) {
			if(pgroup)
				for(card_list::reverse_iterator cit = player[self].list_grave.rbegin(); cit != player[self].list_grave.rend(); ++cit)
					pgroup->container.insert(*cit);
			count += player[self].list_grave.size();
		}
		if(location & LOCATION_REMOVED) {
			if(pgroup)
				for(card_list::reverse_iterator cit = player[self].list_remove.rbegin(); cit != player[self].list_remove.rend(); ++cit)
					pgroup->container.insert(*cit);
			count += player[self].list_remove.size();
		}
		location = location2;
		self = 1 - self;
	}
	return count;
}
int32 field::is_player_affected_by_effect(uint8 playerid, uint32 code) {
	effect* peffect;
	pair<effect_container::iterator, effect_container::iterator> rg;
	rg = effects.aura_effect.equal_range(code);
	for (; rg.first != rg.second; ++rg.first) {
		peffect = rg.first->second;
		if (peffect->is_target_player(playerid) && peffect->is_available())
			return TRUE;
	}
	return FALSE;
}
int32 field::get_release_list(uint8 playerid, card_set* release_list) {
	card* pcard;
	for(int i = 0; i < 5; ++i) {
		pcard = player[playerid].list_mzone[i];
		if(pcard && pcard->is_releaseable_by_nonsummon(playerid))
			release_list->insert(pcard);
	}
	for(int i = 0; i < 5; ++i) {
		pcard = player[1 - playerid].list_mzone[i];
		if(pcard && pcard->is_affected_by_effect(EFFECT_EXTRA_RELEASE) && pcard->is_releaseable_by_nonsummon(playerid))
			release_list->insert(pcard);
	}
	return release_list->size();
}
int32 field::get_summon_release_list(card* target, card_set* release_list) {
	uint8 p = target->current.controler;
	card* pcard;
	for(int i = 0; i < 5; ++i) {
		pcard = player[p].list_mzone[i];
		if(pcard && pcard->is_releaseable_by_summon(p, target))
			release_list->insert(pcard);
	}
	for(int i = 0; i < 5; ++i) {
		pcard = player[1 - p].list_mzone[i];
		if(pcard && pcard->is_affected_by_effect(EFFECT_EXTRA_RELEASE) && pcard->is_releaseable_by_summon(p, target))
			release_list->insert(pcard);
	}
	uint32 rcount = 0;
	card_set::iterator cit;
	for(cit = release_list->begin(); cit != release_list->end(); ++cit) {
		pcard = *cit;
		if(pcard->is_affected_by_effect(EFFECT_DOUBLE_TRIBUTE, target))
			pcard->operation_param = 2;
		else
			pcard->operation_param = 1;
		rcount += pcard->operation_param;
	}
	return rcount;
}
int32 field::get_summon_count_limit(uint8 playerid) {
	effect_set eset;
	filter_player_effect(playerid, EFFECT_SET_SUMMON_COUNT_LIMIT, &eset);
	int32 count = 1, c;
	for(int32 i = 0; i < eset.count; ++i) {
		c = eset[i]->get_value();
		if(c > count)
			count = c;
	}
	return count;
}
int32 field::get_draw_count(uint8 playerid) {
	effect_set eset;
	filter_player_effect(infos.turn_player, EFFECT_DRAW_COUNT, &eset);
	int32 count = player[playerid].draw_count;
	if(eset.count)
		count = eset.get_last()->get_value();
	return count;
}
void field::get_ritual_material(uint8 playerid, card_set* material) {
	card* pcard;
	for(int i = 0; i < 5; ++i) {
		pcard = player[playerid].list_mzone[i];
		if(pcard && pcard->get_level() && pcard->is_affect_by_effect(core.reason_effect)
		        && pcard->is_releaseable_by_nonsummon(playerid))
			material->insert(pcard);
	}
	for(int i = 0; i < 5; ++i) {
		pcard = player[1 - playerid].list_mzone[i];
		if(pcard && pcard->get_level() && pcard->is_affect_by_effect(core.reason_effect)
		        && pcard->is_affected_by_effect(EFFECT_EXTRA_RELEASE) && pcard->is_releaseable_by_nonsummon(playerid))
			material->insert(pcard);
	}
	card_list::iterator cit;
	for(cit = player[playerid].list_hand.begin(); cit != player[playerid].list_hand.end(); ++cit)
		if(((*cit)->data.type & TYPE_MONSTER) && (*cit)->is_releaseable_by_nonsummon(playerid))
			material->insert((*cit));
	for(cit = player[playerid].list_grave.begin(); cit != player[playerid].list_grave.end(); ++cit)
		if(((*cit)->data.type & TYPE_MONSTER) && (*cit)->is_affected_by_effect(EFFECT_EXTRA_RITUAL_MATERIAL) && (*cit)->is_removeable(playerid))
			material->insert((*cit));
}
void field::ritual_release(card_set* material) {
	card_set rel;
	card_set rem;
	card_set::iterator cit;
	for(cit = material->begin(); cit != material->end(); ++cit) {
		if((*cit)->current.location == LOCATION_GRAVE)
			rem.insert(*cit);
		else
			rel.insert(*cit);
	}
	release(&rel, core.reason_effect, REASON_RITUAL + REASON_EFFECT + REASON_MATERIAL, core.reason_player);
	send_to(&rem, core.reason_effect, REASON_RITUAL + REASON_EFFECT + REASON_MATERIAL, core.reason_player, PLAYER_NONE, LOCATION_REMOVED, 0, POS_FACEUP);
}
void field::get_exceed_material(card* scard, card_set* material) {
	card* pcard;
	int32 playerid = scard->current.controler;
	for(int i = 0; i < 5; ++i) {
		pcard = player[playerid].list_mzone[i];
		if(pcard && pcard->is_position(POS_FACEUP) && pcard->is_can_be_exceed_material(scard))
			material->insert(pcard);
	}
	for(int i = 0; i < 5; ++i) {
		pcard = player[1 - playerid].list_mzone[i];
		if(pcard && pcard->is_position(POS_FACEUP) && pcard->is_can_be_exceed_material(scard)
		        && pcard->is_affected_by_effect(EFFECT_EXCEED_MATERIAL))
			material->insert(pcard);
	}
}
void field::get_overlay_group(uint8 self, uint8 s, uint8 o, card_set* pset) {
	uint8 c = s;
	card* pcard;
	card_list::iterator clit;
	for(int p = 0; p < 2; ++p) {
		if(!c)
			continue;
		for(int i = 0; i < 5; ++i) {
			pcard = player[self].list_mzone[i];
			if(pcard && pcard->exceed_materials.size())
				for(clit = pcard->exceed_materials.begin(); clit != pcard->exceed_materials.end(); ++clit)
					pset->insert(*clit);
		}
		self = 1 - self;
		c = o;
	}
}
int32 field::get_overlay_count(uint8 self, uint8 s, uint8 o) {
	uint8 c = s;
	uint32 count = 0;
	for(int p = 0; p < 2; ++p) {
		if(!c)
			continue;
		for(int i = 0; i < 5; ++i) {
			if(player[self].list_mzone[i])
				count += player[self].list_mzone[i]->exceed_materials.size();
		}
		self = 1 - self;
		c = o;
	}
	return count;
}
void field::update_disable_check_list(effect* peffect) {
	card_set cset;
	card_set::iterator it;
	filter_affected_cards(peffect, &cset);
	for (it = cset.begin(); it != cset.end(); ++it)
		add_to_disable_check_list(*it);
}
void field::add_to_disable_check_list(card* pcard) {
	if (effects.disable_check_set.find(pcard) != effects.disable_check_set.end())
		return;
	effects.disable_check_set.insert(pcard);
	effects.disable_check_list.push_back(pcard);
}
void field::adjust_disable_check_list() {
	card* checking;
	int32 pre_disable, new_disable;
	if (!effects.disable_check_list.size())
		return;
	card_set checked;
	do {
		checked.clear();
		while (effects.disable_check_list.size()) {
			checking = effects.disable_check_list.front();
			effects.disable_check_list.pop_front();
			effects.disable_check_set.erase(checking);
			checked.insert(checking);
			if (checking->is_status(STATUS_TO_ENABLE + STATUS_TO_DISABLE))
				continue;
			pre_disable = checking->is_status(STATUS_DISABLED);
			checking->refresh_disable_status();
			new_disable = checking->is_status(STATUS_DISABLED);
			if (pre_disable != new_disable && checking->is_status(STATUS_EFFECT_ENABLED)) {
				checking->filter_disable_related_cards();
				if (pre_disable)
					checking->set_status(STATUS_TO_ENABLE, TRUE);
				else
					checking->set_status(STATUS_TO_DISABLE, TRUE);
			}
		}
		for (card_set::iterator it = checked.begin(); it != checked.end(); ++it) {
			if((*it)->is_status(STATUS_DISABLED) && (*it)->is_status(STATUS_TO_DISABLE) && !(*it)->is_status(STATUS_TO_ENABLE))
				(*it)->reset(RESET_DISABLE, RESET_EVENT);
			(*it)->set_status(STATUS_TO_ENABLE + STATUS_TO_DISABLE, FALSE);
		}
	} while(effects.disable_check_list.size());
}

int32 field::check_lp_cost(uint8 playerid, uint32 lp) {
	effect_set eset;
	int32 val = lp;
	filter_player_effect(playerid, EFFECT_LPCOST_CHANGE, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		pduel->lua->add_param(core.reason_effect, PARAM_TYPE_EFFECT);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		pduel->lua->add_param(val, PARAM_TYPE_INT);
		val = eset[i]->get_value(3);
		if(val <= 0)
			return TRUE;
	}
	event e;
	e.event_cards = 0;
	e.event_player = playerid;
	e.event_value = lp;
	e.reason = 0;
	e.reason_effect = core.reason_effect;
	e.reason_player = playerid;
	if(effect_replace_check(EFFECT_LPCOST_REPLACE, e))
		return true;
	cost[playerid].amount += val;
	if(cost[playerid].amount < player[playerid].lp)
		return TRUE;
	return FALSE;
}
void field::save_lp_cost() {
	for(uint8 playerid = 0; playerid < 2; ++playerid) {
		if(cost[playerid].count < 8)
			cost[playerid].lpstack[cost[playerid].count] = cost[playerid].amount;
		cost[playerid].count++;
	}
}
void field::restore_lp_cost() {
	for(uint8 playerid = 0; playerid < 2; ++playerid) {
		cost[playerid].count--;
		if(cost[playerid].count < 8)
			cost[playerid].amount = cost[playerid].lpstack[cost[playerid].count];
	}
}
uint32 field::get_field_counter(uint8 self, uint8 s, uint8 o, uint16 countertype) {
	uint8 c = s;
	uint32 count = 0;
	for(int p = 0; p < 2; ++p) {
		if(c) {
			for(int i = 0; i < 5; ++i) {
				if(player[self].list_mzone[i])
					count += player[self].list_mzone[i]->get_counter(countertype);
			}
			for(int i = 0; i < 6; ++i) {
				if(player[self].list_szone[i])
					count += player[self].list_szone[i]->get_counter(countertype);
			}
		}
		self = 1 - self;
		c = o;
	}
	return count;
}
int32 field::effect_replace_check(uint32 code, event e) {
	pair<effect_container::iterator, effect_container::iterator> pr;
	pr = effects.continuous_effect.equal_range(code);
	effect* peffect;
	for (; pr.first != pr.second; ++pr.first) {
		peffect = pr.first->second;
		if(peffect->is_activateable(peffect->get_handler_player(), e))
			return TRUE;
	}
	return FALSE;
}
int32 field::get_attack_target(card* pcard, card_vector* v, uint8 chain_attack) {
	uint8 p = pcard->current.controler;
	effect* peffect;
	card* atarget;
	pcard->operation_param = 0;
	card_vector must_be_attack;
	card_vector* pv;
	card_vector::iterator cit;
	for(uint32 i = 0; i < 5; ++i) {
		atarget = player[1 - p].list_mzone[i];
		if(atarget && atarget->is_affected_by_effect(EFFECT_MUST_BE_ATTACKED))
			must_be_attack.push_back(atarget);
	}
	if((peffect = pcard->is_affected_by_effect(EFFECT_ATTACK_ALL))) {
		if(pcard->announced_cards.size()) {
			card::attacker_map::iterator ait;
			for(ait = pcard->announced_cards.begin(); ait != pcard->announced_cards.end(); ++ait) {
				if(ait->first == 0)
					continue;
				pduel->lua->add_param(ait->second, PARAM_TYPE_CARD);
				if(!peffect->check_value_condition(1))
					return 0;
			}
			if(must_be_attack.size())
				pv = &must_be_attack;
			else
				pv = &player[1 - p].list_mzone;
			for(cit = pv->begin(); cit != pv->end(); ++cit) {
				atarget = *cit;
				if(!atarget || pcard->announced_cards.count(atarget->fieldid))
					continue;
				if(atarget->is_affected_by_effect(EFFECT_IGNORE_BATTLE_TARGET))
					continue;
				if(atarget->is_affected_by_effect(EFFECT_CANNOT_BE_BATTLE_TARGET, pcard))
					continue;
				pduel->lua->add_param(atarget, PARAM_TYPE_CARD);
				if(!peffect->check_value_condition(1))
					continue;
				v->push_back(atarget);
			}
			return must_be_attack.size() ? TRUE : FALSE;
		}
	} else if(!chain_attack) {
		uint32 extrac = 0;
		if((peffect = pcard->is_affected_by_effect(EFFECT_EXTRA_ATTACK)))
			extrac = peffect->get_value(pcard);
		if(pcard->announce_count >= extrac + 1)
			return FALSE;
	}
	uint32 mcount = 0;
	if(must_be_attack.size())
		pv = &must_be_attack;
	else
		pv = &player[1 - p].list_mzone;
	for(cit = pv->begin(); cit != pv->end(); ++cit) {
		atarget = *cit;
		if(!atarget)
			continue;
		if(atarget->is_affected_by_effect(EFFECT_IGNORE_BATTLE_TARGET))
			continue;
		mcount++;
		if(atarget->is_affected_by_effect(EFFECT_CANNOT_BE_BATTLE_TARGET, pcard))
			continue;
		v->push_back(atarget);
	}
	if(must_be_attack.size())
		return TRUE;
	if((mcount == 0 || pcard->is_affected_by_effect(EFFECT_DIRECT_ATTACK)) && !pcard->is_affected_by_effect(EFFECT_CANNOT_DIRECT_ATTACK))
		pcard->operation_param = 1;
	return must_be_attack.size() ? TRUE : FALSE;
}
int32 field::check_synchro_material(card* pcard, int32 findex1, int32 findex2, int32 min) {
	card* tuner;
	for(uint8 p = 0; p < 2; ++p) {
		for(int32 i = 0; i < 5; ++i) {
			tuner = player[p].list_mzone[i];
			if(check_tuner_material(pcard, tuner, findex1, findex2, min))
				return TRUE;
		}
	}
	return FALSE;
}
int32 field::check_tuner_material(card* pcard, card* tuner, int32 findex1, int32 findex2, int32 min) {
	effect* peffect;
	if(tuner && tuner->is_position(POS_FACEUP) && (tuner->get_type()&TYPE_TUNER) && pduel->lua->check_matching(tuner, findex1, 0)
	        && tuner->is_can_be_synchro_material(pcard)) {
		if((peffect = tuner->is_affected_by_effect(EFFECT_SYNCHRO_MATERIAL_CUSTOM, pcard))) {
			if(!peffect->target)
				return FALSE;
			pduel->lua->add_param(peffect, PARAM_TYPE_EFFECT);
			pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
			pduel->lua->add_param(findex2, PARAM_TYPE_INDEX);
			pduel->lua->add_param(min, PARAM_TYPE_INT);
			if(pduel->lua->check_condition(peffect->target, 4))
				return TRUE;
		} else {
			card_vector nsyn;
			card* pm;
			for(uint8 p = 0; p < 2; ++p) {
				for(int32 i = 0; i < 5; ++i) {
					pm = player[p].list_mzone[i];
					if(pm && pm != tuner && pm->is_position(POS_FACEUP) && pduel->lua->check_matching(pm, findex2, 0)
					        && pm->is_can_be_synchro_material(pcard)) {
						nsyn.push_back(pm);
						pm->operation_param = pm->get_synchro_level(pcard);
					}
				}
			}
			int32 l = tuner->get_synchro_level(pcard);
			int32 l1 = l & 0xffff;
			//int32 l2 = l >> 16;
			int32 lv = pcard->get_level();
			if(lv == l1)
				return FALSE;
			if(check_with_sum_limit(&nsyn, lv - l1, 0, 1, min))
				return TRUE;
		}
	}
	return FALSE;
}
int32 field::check_with_sum_limit(card_vector* mats, int32 acc, int32 index, int32 count, int32 min) {
	if((uint32)index >= mats->size())
		return FALSE;
	int32 op1 = mats->at(index)->operation_param & 0xffff;
	int32 op2 = (mats->at(index)->operation_param >> 16) & 0xffff;
	if((op1 == acc || op2 == acc) && count >= min)
		return TRUE;
	return (acc > op1 && check_with_sum_limit(mats, acc - op1, index + 1, count + 1, min))
	       || (op2 && acc > op2 && check_with_sum_limit(mats, acc - op2, index + 1, count + 1, min))
	       || check_with_sum_limit(mats, acc, index + 1, count, min);
}
int32 field::is_player_can_draw(uint8 playerid) {
	return !is_player_affected_by_effect(playerid, EFFECT_CANNOT_DRAW);
}
int32 field::is_player_can_discard_deck(uint8 playerid) {
	return !is_player_affected_by_effect(playerid, EFFECT_CANNOT_DISCARD_DECK);
}
int32 field::is_player_can_discard_deck_as_cost(uint8 playerid, int32 count) {
	if(player[playerid].list_main.size() < (uint32)count)
		return FALSE;
	if(is_player_affected_by_effect(playerid, EFFECT_CANNOT_DISCARD_DECK))
		return FALSE;
	effect_set eset;
	filter_field_effect(EFFECT_TO_GRAVE_REDIRECT, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		uint8 p = eset[i]->get_handler_player();
		if((eset[i]->flag & EFFECT_FLAG_IGNORE_RANGE) || (p == playerid && eset[i]->s_range & LOCATION_DECK) || (p != playerid && eset[i]->o_range & LOCATION_DECK))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_discard_hand(uint8 playerid, card* pcard, effect* peffect, uint32 reason) {
	if(pcard->current.location != LOCATION_HAND)
		return FALSE;
	effect_set eset;
	filter_player_effect(playerid, EFFECT_CANNOT_DISCARD_HAND, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(peffect, PARAM_TYPE_EFFECT);
		pduel->lua->add_param(reason, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 4))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_summon(uint32 sumtype, uint8 playerid, card * pcard) {
	effect_set eset;
	sumtype |= SUMMON_TYPE_NORMAL;
	filter_player_effect(playerid, EFFECT_CANNOT_SUMMON, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		pduel->lua->add_param(sumtype, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 4))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_mset(uint32 sumtype, uint8 playerid, card* pcard) {
	effect_set eset;
	sumtype |= SUMMON_TYPE_NORMAL;
	filter_player_effect(playerid, EFFECT_CANNOT_MSET, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		pduel->lua->add_param(sumtype, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 4))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_sset(uint8 playerid, card* pcard) {
	effect_set eset;
	filter_player_effect(playerid, EFFECT_CANNOT_SSET, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 3))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_spsummon(effect* peffect, uint32 sumtype, uint8 sumpos, uint8 playerid, uint8 toplayer, card * pcard) {
	effect_set eset;
	sumtype |= SUMMON_TYPE_SPECIAL;
	if(sumpos & POS_FACEDOWN && is_player_affected_by_effect(playerid, EFFECT_DEVINE_LIGHT))
		sumpos = (sumpos & POS_FACEUP) | (sumpos >> 1);
	filter_player_effect(playerid, EFFECT_CANNOT_SPECIAL_SUMMON, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		pduel->lua->add_param(sumtype, PARAM_TYPE_INT);
		pduel->lua->add_param(sumpos, PARAM_TYPE_INT);
		pduel->lua->add_param(toplayer, PARAM_TYPE_INT);
		pduel->lua->add_param(peffect, PARAM_TYPE_EFFECT);
		if (pduel->lua->check_condition(eset[i]->target, 7))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_flipsummon(uint8 playerid, card * pcard) {
	effect_set eset;
	filter_player_effect(playerid, EFFECT_CANNOT_FLIP_SUMMON, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 3))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_spsummon_token(uint8 playerid, uint8 toplayer, uint8 sumpos, card_data * pdata) {
	temp_card->data = *pdata;
	if(!is_player_can_spsummon(core.reason_effect, SUMMON_TYPE_SPECIAL, sumpos, playerid, toplayer, temp_card))
		return FALSE;
	return temp_card->is_affected_by_effect(EFFECT_CANNOT_SPECIAL_SUMMON) ? FALSE : TRUE;
}
int32 field::is_player_can_release(uint8 playerid, card * pcard) {
	effect_set eset;
	filter_player_effect(playerid, EFFECT_CANNOT_RELEASE, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 3))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_remove_counter(uint8 playerid, card * pcard, uint8 s, uint8 o, uint16 countertype, uint16 count, uint32 reason) {
	if((pcard && pcard->get_counter(countertype) >= count) || (!pcard && get_field_counter(playerid, s, o, countertype) >= count))
		return TRUE;
	pair<effect_container::iterator, effect_container::iterator> pr;
	pr = effects.continuous_effect.equal_range(EFFECT_RCOUNTER_REPLACE + countertype);
	effect* peffect;
	event e;
	e.event_cards = 0;
	e.event_player = playerid;
	e.event_value = count;
	e.reason = reason;
	e.reason_effect = core.reason_effect;
	e.reason_player = playerid;
	for (; pr.first != pr.second; ++pr.first) {
		peffect = pr.first->second;
		if(peffect->is_activateable(peffect->get_handler_player(), e))
			return TRUE;
	}
	return FALSE;
}
int32 field::is_player_can_send_to_grave(uint8 playerid, card * pcard) {
	effect_set eset;
	filter_player_effect(playerid, EFFECT_CANNOT_TO_GRAVE, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 3))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_send_to_hand(uint8 playerid, card * pcard) {
	effect_set eset;
	filter_player_effect(playerid, EFFECT_CANNOT_TO_HAND, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 3))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_send_to_deck(uint8 playerid, card * pcard) {
	effect_set eset;
	filter_player_effect(playerid, EFFECT_CANNOT_TO_DECK, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 3))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_player_can_remove(uint8 playerid, card * pcard) {
	effect_set eset;
	filter_player_effect(playerid, EFFECT_CANNOT_REMOVE, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		if(!eset[i]->target)
			return FALSE;
		pduel->lua->add_param(eset[i], PARAM_TYPE_EFFECT);
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		pduel->lua->add_param(playerid, PARAM_TYPE_INT);
		if (pduel->lua->check_condition(eset[i]->target, 3))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_chain_inactivatable(uint8 chaincount) {
	effect_set eset;
	if(chaincount < 0 && chaincount > core.current_chain.size())
		return FALSE;
	effect* peffect;
	if(chaincount == 0)
		peffect = core.current_chain.back().triggering_effect;
	else
		peffect = core.current_chain[chaincount - 1].triggering_effect;
	if(peffect->flag & EFFECT_FLAG_CANNOT_DISABLE)
		return FALSE;
	filter_field_effect(EFFECT_CANNOT_INACTIVATE, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		pduel->lua->add_param(chaincount, PARAM_TYPE_INT);
		if(eset[i]->check_value_condition(1))
			return FALSE;
	}
	return TRUE;
}
int32 field::is_chain_disablable(uint8 chaincount) {
	effect_set eset;
	if(chaincount < 0 && chaincount > core.current_chain.size())
		return FALSE;
	effect* peffect;
	if(chaincount == 0)
		peffect = core.current_chain.back().triggering_effect;
	else
		peffect = core.current_chain[chaincount - 1].triggering_effect;
	if(peffect->flag & EFFECT_FLAG_CANNOT_DISABLE)
		return FALSE;
	filter_field_effect(EFFECT_CANNOT_DISEFFECT, &eset);
	for(int32 i = 0; i < eset.count; ++i) {
		pduel->lua->add_param(chaincount, PARAM_TYPE_INT);
		if(eset[i]->check_value_condition(1))
			return FALSE;
	}
	return TRUE;
}
int32 field::check_chain_target(uint8 chaincount, card* pcard) {
	if(chaincount < 0 && chaincount > core.current_chain.size())
		return FALSE;
	chain* pchain;
	if(chaincount == 0)
		pchain = &core.current_chain.back();
	else
		pchain = &core.current_chain[chaincount - 1];
	effect* peffect = pchain->triggering_effect;
	uint8 tp = pchain->triggering_player;
	if(!(peffect->flag & EFFECT_FLAG_CARD_TARGET) || !peffect->target)
		return FALSE;
	if(!pcard->is_capable_be_effect_target(peffect, tp))
		return false;
	pduel->lua->add_param(peffect, PARAM_TYPE_EFFECT);
	pduel->lua->add_param(tp, PARAM_TYPE_INT);
	pduel->lua->add_param(pchain->evt.event_cards , PARAM_TYPE_GROUP);
	pduel->lua->add_param(pchain->evt.event_player, PARAM_TYPE_INT);
	pduel->lua->add_param(pchain->evt.event_value, PARAM_TYPE_INT);
	pduel->lua->add_param(pchain->evt.reason_effect , PARAM_TYPE_EFFECT);
	pduel->lua->add_param(pchain->evt.reason, PARAM_TYPE_INT);
	pduel->lua->add_param(pchain->evt.reason_player, PARAM_TYPE_INT);
	pduel->lua->add_param((ptr)0, PARAM_TYPE_INT);
	pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
	return pduel->lua->check_condition(peffect->target, 10);
}
