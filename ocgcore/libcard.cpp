/*
 * libcard.cpp
 *
 *  Created on: 2010-5-6
 *      Author: Argon
 */

#include "scriptlib.h"
#include <iostream>

int32 scriptlib::card_get_code(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_code());
	return 1;
}
int32 scriptlib::card_get_origin_code(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->data.code);
	return 1;
}
int32 scriptlib::card_is_set_card(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 set_code = lua_tointeger(L, 2);
	lua_pushboolean(L, pcard->is_set_card(set_code));
	return 1;
}
int32 scriptlib::card_get_type(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_type());
	return 1;
}
int32 scriptlib::card_get_origin_type(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->data.type);
	return 1;
}
int32 scriptlib::card_get_level(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_level());
	return 1;
}
int32 scriptlib::card_get_synchro_level(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* scard = *(card**) lua_touserdata(L, 2);
	lua_pushinteger(L, pcard->get_synchro_level(scard));
	return 1;
}
int32 scriptlib::card_get_ritual_level(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* scard = *(card**) lua_touserdata(L, 2);
	lua_pushinteger(L, pcard->get_ritual_level(scard));
	return 1;
}
int32 scriptlib::card_get_origin_level(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if(pcard->data.type & TYPE_XYZ)
		lua_pushinteger(L, 0);
	else
		lua_pushinteger(L, pcard->data.level);
	return 1;
}
int32 scriptlib::card_get_attribute(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_attribute());
	return 1;
}
int32 scriptlib::card_get_origin_attribute(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->data.attribute);
	return 1;
}
int32 scriptlib::card_get_race(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_race());
	return 1;
}
int32 scriptlib::card_get_origin_race(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->data.race);
	return 1;
}
int32 scriptlib::card_get_attack(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_attack());
	return 1;
}
int32 scriptlib::card_get_origin_attack(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_base_attack());
	return 1;
}
int32 scriptlib::card_get_text_attack(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->data.attack);
	return 1;
}
int32 scriptlib::card_get_defence(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_defence());
	return 1;
}
int32 scriptlib::card_get_origin_defence(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_base_defence());
	return 1;
}
int32 scriptlib::card_get_text_defence(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->data.defence);
	return 1;
}
int32 scriptlib::card_get_owner(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->owner);
	return 1;
}
int32 scriptlib::card_get_controler(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->current.controler);
	return 1;
}
int32 scriptlib::card_get_previous_controler(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->previous.controler);
	return 1;
}
int32 scriptlib::card_get_reason(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->current.reason);
	return 1;
}
int32 scriptlib::card_get_reason_card(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	interpreter::card2value(L, pcard->current.reason_card);
	return 1;
}
int32 scriptlib::card_get_reason_player(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->current.reason_player);
	return 1;
}
int32 scriptlib::card_get_reason_effect(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	interpreter::effect2value(L, pcard->current.reason_effect);
	return 1;
}
int32 scriptlib::card_get_position(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->current.position);
	return 1;
}
int32 scriptlib::card_get_previous_position(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->previous.position);
	return 1;
}
int32 scriptlib::card_get_battle_position(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->temp.position);
	return 1;
}
int32 scriptlib::card_get_location(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if(pcard->is_status(STATUS_SUMMONING) || pcard->is_status(STATUS_SUMMON_DISABLED))
		lua_pushinteger(L, 0);
	else
		lua_pushinteger(L, pcard->current.location);
	return 1;
}
int32 scriptlib::card_get_previous_location(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->previous.location);
	return 1;
}
int32 scriptlib::card_get_sequence(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->current.sequence);
	return 1;
}
int32 scriptlib::card_get_previous_sequence(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->previous.sequence);
	return 1;
}
int32 scriptlib::card_get_summon_type(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->summon_type);
	return 1;
}
int32 scriptlib::card_get_summon_player(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->summon_player);
	return 1;
}
int32 scriptlib::card_get_destination(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, (pcard->operation_param >> 8) & 0xff);
	return 1;
}
int32 scriptlib::card_get_turnid(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->turnid);
	return 1;
}
int32 scriptlib::card_get_fieldid(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->fieldid);
	return 1;
}
int32 scriptlib::card_is_code(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 tcode = lua_tointeger(L, 2);
	if(pcard->get_code() == tcode)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_type(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 ttype = lua_tointeger(L, 2);
	if(pcard->get_type()&ttype)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_race(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 trace = lua_tointeger(L, 2);
	if(pcard->get_race()&trace)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_attribute(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 tattrib = lua_tointeger(L, 2);
	if(pcard->get_attribute()&tattrib)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_reason(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 treason = lua_tointeger(L, 2);
	if(pcard->current.reason & treason)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_status(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 tstatus = lua_tointeger(L, 2);
	if(pcard->is_status(tstatus))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_not_tuner(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 type = pcard->get_type();
	if(!(type & TYPE_TUNER) || pcard->is_affected_by_effect(EFFECT_NONTUNER))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_set_status(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 tstatus = lua_tointeger(L, 2);
	int32 enable = lua_toboolean(L, 3);
	pcard->set_status(tstatus, enable);
	return 0;
}
int32 scriptlib::card_is_dual_state(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, (ptr)pcard->is_affected_by_effect(EFFECT_DUAL_STATUS));
	return 1;
}
int32 scriptlib::card_enable_dual_state(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	duel* pduel = pcard->pduel;
	effect* deffect = pduel->new_effect();
	deffect->owner = pcard;
	deffect->code = EFFECT_DUAL_STATUS;
	deffect->type = EFFECT_TYPE_SINGLE;
	deffect->flag = EFFECT_FLAG_CANNOT_DISABLE;
	deffect->reset_flag = RESET_EVENT + 0x1fe0000;
	pcard->add_effect(deffect);
	return 0;
}
int32 scriptlib::card_set_turn_counter(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 ct = lua_tointeger(L, 2);
	pcard->count_turn(ct);
	return 0;
}
int32 scriptlib::card_get_turn_counter(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->turn_counter);
	return 1;
}
int32 scriptlib::card_set_material(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_GROUP, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = *(group**) lua_touserdata(L, 2);
	pcard->set_material(&pgroup->container);
	return 0;
}
int32 scriptlib::card_get_material(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = pcard->pduel->new_group();
	card::card_set::iterator cit;
	for(cit = pcard->material_cards.begin(); cit != pcard->material_cards.end(); ++cit)
		pgroup->container.insert(*cit);
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::card_get_material_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->material_cards.size());
	return 1;
}
int32 scriptlib::card_get_equip_group(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = pcard->pduel->new_group();
	card::card_set::iterator cit;
	for(cit = pcard->equiping_cards.begin(); cit != pcard->equiping_cards.end(); ++cit)
		pgroup->container.insert(*cit);
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::card_get_equip_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->equiping_cards.size());
	return 1;
}
int32 scriptlib::card_get_equip_target(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	interpreter::card2value(L, pcard->equiping_target);
	return 1;
}
int32 scriptlib::card_get_pre_equip_target(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	interpreter::card2value(L, pcard->pre_equip_target);
	return 1;
}
int32 scriptlib::card_check_equip_target(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* target = *(card**) lua_touserdata(L, 2);
	if(pcard->is_affected_by_effect(EFFECT_EQUIP_LIMIT, target)
	        && (!pcard->is_status(STATUS_UNION) || target->get_union_count() == 0))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_get_union_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->get_union_count());
	return 1;
}
int32 scriptlib::card_get_overlay_group(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = pcard->pduel->new_group();
	for(auto cit = pcard->exceed_materials.begin(); cit != pcard->exceed_materials.end(); ++cit)
		pgroup->container.insert(*cit);
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::card_get_overlay_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->exceed_materials.size());
	return 1;
}
int32 scriptlib::card_check_remove_overlay_card(lua_State *L) {
	check_param_count(L, 4);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 playerid = lua_tointeger(L, 2);
	if(playerid != 0 && playerid != 1)
		return 0;
	int32 count = lua_tointeger(L, 3);
	int32 reason = lua_tointeger(L, 4);
	duel* pduel = pcard->pduel;
	lua_pushboolean(L, pduel->game_field->is_player_can_remove_overlay_card(playerid, pcard, 0, 0, count, reason));
	return 1;
}
int32 scriptlib::card_remove_overlay_card(lua_State *L) {
	check_action_permission(L);
	check_param_count(L, 5);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 playerid = lua_tointeger(L, 2);
	if(playerid != 0 && playerid != 1)
		return 0;
	int32 min = lua_tointeger(L, 3);
	int32 max = lua_tointeger(L, 4);
	int32 reason = lua_tointeger(L, 5);
	duel* pduel = pcard->pduel;
	pduel->game_field->remove_overlay_card(reason, pcard, playerid, 0, 0, min, max);
	return lua_yield(L, 0);
}
int32 scriptlib::card_get_attacked_group(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = pcard->pduel->new_group();
	card::attacker_map::iterator cit;
	for(cit = pcard->attacked_cards.begin(); cit != pcard->attacked_cards.end(); ++cit) {
		if(cit->second)
			pgroup->container.insert(cit->second);
	}
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::card_get_attacked_group_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->attacked_cards.size());
	return 1;
}
int32 scriptlib::card_get_attacked_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->attacked_count);
	return 1;
}
int32 scriptlib::card_get_battled_group(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = pcard->pduel->new_group();
	card::attacker_map::iterator cit;
	for(cit = pcard->battled_cards.begin(); cit != pcard->battled_cards.end(); ++cit) {
		if(cit->second)
			pgroup->container.insert(cit->second);
	}
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::card_get_battled_group_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->battled_cards.size());
	return 1;
}
int32 scriptlib::card_get_attack_announced_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->announce_count);
	return 1;
}
int32 scriptlib::card_is_direct_attacked(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	bool ret = false;
	for(auto cit = pcard->attacked_cards.begin(); cit != pcard->attacked_cards.end(); ++cit)
		if(cit->first == 0)
			ret = true;
	lua_pushboolean(L, ret);
	return 1;
}
int32 scriptlib::card_set_card_target(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* ocard = *(card**) lua_touserdata(L, 2);
	pcard->effect_target_cards.insert(ocard);
	ocard->effect_target_owner.insert(pcard);
	return 0;
}
int32 scriptlib::card_get_card_target(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = pcard->pduel->new_group();
	pgroup->container = pcard->effect_target_cards;
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::card_get_card_target_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->effect_target_cards.size());
	return 1;
}
int32 scriptlib::card_is_has_card_target(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* rcard = *(card**) lua_touserdata(L, 2);
	lua_pushboolean(L, pcard->effect_target_cards.count(rcard));
	return 1;
}
int32 scriptlib::card_get_owner_target(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = pcard->pduel->new_group();
	pgroup->container = pcard->effect_target_owner;
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::card_get_owner_target_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushinteger(L, pcard->effect_target_owner.size());
	return 1;
}
int32 scriptlib::card_get_activate_effect(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card::effect_container::iterator eit;
	int32 count = 0;
	for(eit = pcard->field_effect.begin(); eit != pcard->field_effect.end(); ++eit) {
		if(eit->second->type & EFFECT_TYPE_ACTIVATE) {
			interpreter::effect2value(L, eit->second);
			count++;
		}
	}
	return count;
}
int32 scriptlib::card_register_effect(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_EFFECT, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	effect* peffect = *(effect**) lua_touserdata(L, 2);
	int32 forced = lua_toboolean(L, 3);
	duel* pduel = pcard->pduel;
	if(!forced && pduel->game_field->core.reason_effect && !pcard->is_affect_by_effect(pduel->game_field->core.reason_effect))
		return 0;
	int32 id;
	if (peffect->handler)
		id = -1;
	else
		id = pcard->add_effect(peffect);
	lua_pushinteger(L, id);
	return 1;
}
int32 scriptlib::card_is_has_effect(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 code = lua_tointeger(L, 2);
	if(pcard && pcard->is_affected_by_effect(code))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_get_effect_count(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 code = lua_tointeger(L, 2);
	effect_set eset;
	pcard->filter_effect(code, &eset);
	lua_pushinteger(L, eset.count);
	return 1;
}
int32 scriptlib::card_register_flag_effect(lua_State *L) {
	check_param_count(L, 5);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 code = (lua_tointeger(L, 2) & 0xfffffff) | 0x10000000;
	int32 reset = lua_tointeger(L, 3);
	int32 flag = lua_tointeger(L, 4);
	int32 count = lua_tointeger(L, 5);
	if(count == 0)
		count = 1;
	if(reset & (RESET_PHASE) && !(reset & (RESET_SELF_TURN | RESET_OPPO_TURN)))
		reset |= (RESET_SELF_TURN | RESET_OPPO_TURN);
	duel* pduel = pcard->pduel;
	effect* peffect = pduel->new_effect();
	peffect->owner = pcard;
	peffect->handler = 0;
	peffect->type = EFFECT_TYPE_SINGLE;
	peffect->code = code;
	peffect->reset_flag = reset;
	peffect->flag = flag | EFFECT_FLAG_CANNOT_DISABLE;
	peffect->reset_count |= count & 0xff;
	pcard->add_effect(peffect);
	interpreter::effect2value(L, peffect);
	return 1;
}
int32 scriptlib::card_get_flag_effect(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 code = (lua_tointeger(L, 2) & 0xfffffff) | 0x10000000;
	lua_pushinteger(L, pcard->single_effect.count(code));
	return 1;
}
int32 scriptlib::card_reset_flag_effect(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 code = (lua_tointeger(L, 2) & 0xfffffff) | 0x10000000;
	pcard->reset(code, RESET_CODE);
	return 0;
}
int32 scriptlib::card_create_relation(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* rcard = *(card**) lua_touserdata(L, 2);
	uint32 reset = lua_tointeger(L, 3);
	pcard->create_relation(rcard, reset);
	return 0;
}
int32 scriptlib::card_release_relation(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* rcard = *(card**) lua_touserdata(L, 2);
	pcard->release_relation(rcard);
	return 0;
}
int32 scriptlib::card_create_effect_relation(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_EFFECT, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	effect* peffect = *(effect**) lua_touserdata(L, 2);
	pcard->create_relation(peffect);
	return 0;
}
int32 scriptlib::card_is_relate_to_effect(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_EFFECT, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	effect* peffect = *(effect**) lua_touserdata(L, 2);
	if(pcard && pcard->is_has_relation(peffect))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_relate_to_card(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* rcard = *(card**) lua_touserdata(L, 2);
	if(pcard && pcard->is_has_relation(rcard))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_relate_to_battle(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	duel* pduel = pcard->pduel;
	if(pcard->fieldid == pduel->game_field->core.pre_field[0] || pcard->fieldid == pduel->game_field->core.pre_field[1])
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_copy_effect(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 code = lua_tointeger(L, 2);
	uint32 reset = lua_tointeger(L, 3);
	uint32 count = lua_tointeger(L, 4);
	if(count == 0)
		count = 1;
	if(reset & RESET_PHASE && !(reset & (RESET_SELF_TURN | RESET_OPPO_TURN)))
		reset |= (RESET_SELF_TURN | RESET_OPPO_TURN);
	lua_pushinteger(L, pcard->copy_effect(code, reset, count));
	return 1;
}
int32 scriptlib::card_enable_revive_limit(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	pcard->set_status(STATUS_REVIVE_LIMIT, TRUE);
	return 0;
}
int32 scriptlib::card_complete_procedure(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	pcard->set_status(STATUS_PROC_COMPLETE, TRUE);
	return 0;
}
int32 scriptlib::card_is_disabled(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_status(STATUS_DISABLED));
	return 1;
}
int32 scriptlib::card_is_destructable(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if(pcard->is_destructable())
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_summonable(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card * pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_summonable());
	return 1;
}
int32 scriptlib::card_is_msetable(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	uint32 ign = lua_toboolean(L, 2);
	effect* peffect = 0;
	if(!lua_isnil(L, 3)) {
		check_param(L, PARAM_TYPE_EFFECT, 3);
		peffect = *(effect**)lua_touserdata(L, 3);
	}
	lua_pushboolean(L, pcard->is_setable_mzone(p, ign, peffect));
	return 1;
}
int32 scriptlib::card_is_ssetable(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	lua_pushboolean(L, pcard->is_setable_szone(p));
	return 1;
}
int32 scriptlib::card_is_special_summonable(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	lua_pushboolean(L, pcard->is_special_summonable(p));
	return 1;
}
int32 scriptlib::card_is_synchro_summonable(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if(!(pcard->get_type()&TYPE_SYNCHRO))
		return 0;
	card* tuner = 0;
	if(!lua_isnil(L, 2)) {
		check_param(L, PARAM_TYPE_CARD, 2);
		tuner = *(card**) lua_touserdata(L, 2);
	}
	uint32 p = pcard->pduel->game_field->core.reason_player;
	pcard->pduel->game_field->core.limit_tuner = tuner;
	lua_pushboolean(L, pcard->is_special_summonable(p));
	return 1;
}
int32 scriptlib::card_is_can_be_summoned(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	uint32 ign = lua_toboolean(L, 2);
	effect* peffect = 0;
	if(!lua_isnil(L, 3)) {
		check_param(L, PARAM_TYPE_EFFECT, 3);
		peffect = *(effect**)lua_touserdata(L, 3);
	}
	lua_pushboolean(L, pcard->is_can_be_summoned(p, ign, peffect));
	return 1;
}
int32 scriptlib::card_is_can_be_special_summoned(lua_State *L) {
	check_param_count(L, 6);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_EFFECT, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	effect* peffect = *(effect**) lua_touserdata(L, 2);
	uint32 sumtype = lua_tointeger(L, 3);
	uint32 sumplayer = lua_tointeger(L, 4);
	uint32 nocheck = lua_toboolean(L, 5);
	uint32 nolimit = lua_toboolean(L, 6);
	uint32 sumpos = POS_FACEUP;
	uint32 toplayer = sumplayer;
	if(lua_gettop(L) > 6)
		sumpos = lua_tointeger(L, 7);
	if(lua_gettop(L) > 7)
		toplayer = lua_tointeger(L, 8);
	if(pcard->is_can_be_special_summoned(peffect, sumtype, sumpos, sumplayer, toplayer, nocheck, nolimit))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_hand(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_capable_send_to_hand(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_grave(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_capable_send_to_grave(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_deck(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_capable_send_to_deck(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_extra(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_capable_send_to_extra(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_remove(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_removeable(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_hand_as_cost(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_capable_cost_to_hand(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_grave_as_cost(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_capable_cost_to_grave(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_deck_as_cost(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_capable_cost_to_deck(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_extra_as_cost(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_capable_cost_to_extra(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_remove_as_cost(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_removeable_as_cost(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_releaseable(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	if(pcard->is_releaseable_by_nonsummon(p))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_discardable(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 p = pcard->pduel->game_field->core.reason_player;
	effect* pe = pcard->pduel->game_field->core.reason_effect;
	uint32 reason = REASON_COST;
	if(lua_gettop(L) > 1)
		reason = lua_tointeger(L, 2);
	if((reason != REASON_COST || !pcard->is_affected_by_effect(EFFECT_CANNOT_USE_AS_COST))
	        && pcard->pduel->game_field->is_player_can_discard_hand(p, pcard, pe, reason))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_attackable(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_capable_attack());
	return 1;
}
int32 scriptlib::card_is_chain_attackable(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	duel* pduel = pcard->pduel;
	card* attacker = pduel->game_field->core.attacker;
	if(attacker->is_status(STATUS_BATTLE_DESTROYED) || attacker->fieldid != pduel->game_field->core.pre_field[0]
	        || !attacker->is_capable_attack_announce(pduel->game_field->infos.turn_player)
	        || attacker->announce_count > 1) {
		lua_pushboolean(L, 0);
		return 1;
	}
	pduel->game_field->core.select_cards.clear();
	pduel->game_field->get_attack_target(attacker, &pduel->game_field->core.select_cards, TRUE);
	if(pduel->game_field->core.select_cards.size() == 0 && attacker->operation_param == 0)
		lua_pushboolean(L, 0);
	else
		lua_pushboolean(L, 1);
	return 1;
}
int32 scriptlib::card_is_faceup(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_position(POS_FACEUP));
	return 1;
}
int32 scriptlib::card_is_attack(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_position(POS_ATTACK));
	return 1;
}
int32 scriptlib::card_is_facedown(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_position(POS_FACEDOWN));
	return 1;
}
int32 scriptlib::card_is_defence(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_position(POS_DEFENCE));
	return 1;
}
int32 scriptlib::card_is_position(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 pos = lua_tointeger(L, 2);
	lua_pushboolean(L, pcard->is_position(pos));
	return 1;
}
int32 scriptlib::card_is_pre_position(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 pos = lua_tointeger(L, 2);
	lua_pushboolean(L, pcard->previous.position & pos);
	return 1;
}
int32 scriptlib::card_is_controler(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 con = lua_tointeger(L, 2);
	if(pcard->current.controler == con)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_onfield(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if((pcard->current.location & LOCATION_ONFIELD) && !pcard->is_status(STATUS_SUMMONING) && !pcard->is_status(STATUS_SUMMON_DISABLED))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_location(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 loc = lua_tointeger(L, 2);
	if(pcard->current.location == LOCATION_MZONE) {
		if((loc & LOCATION_MZONE) && !pcard->is_status(STATUS_SUMMONING) && !pcard->is_status(STATUS_SUMMON_DISABLED))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	} else
		lua_pushboolean(L, pcard->current.location & loc);
	return 1;
}
int32 scriptlib::card_is_pre_location(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 loc = lua_tointeger(L, 2);
	lua_pushboolean(L, pcard->previous.location & loc);
	return 1;
}
int32 scriptlib::card_is_level_below(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 lvl = lua_tointeger(L, 2);
	if((pcard->data.type & TYPE_XYZ) || (!(pcard->data.type & TYPE_MONSTER) && !(pcard->current.location & LOCATION_MZONE)))
		lua_pushboolean(L, 0);
	else
		lua_pushboolean(L, pcard->get_level() <= lvl);
	return 1;
}
int32 scriptlib::card_is_level_above(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 lvl = lua_tointeger(L, 2);
	if((pcard->data.type & TYPE_XYZ) || (!(pcard->data.type & TYPE_MONSTER) && !(pcard->current.location & LOCATION_MZONE)))
		lua_pushboolean(L, 0);
	else
		lua_pushboolean(L, pcard->get_level() >= lvl);
	return 1;
}
int32 scriptlib::card_is_attack_below(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 atk = lua_tointeger(L, 2);
	if(!(pcard->data.type & TYPE_MONSTER) && !(pcard->current.location & LOCATION_MZONE))
		lua_pushboolean(L, 0);
	else {
		int _atk = pcard->get_attack();
		lua_pushboolean(L, _atk >= 0 && _atk <= atk);
	}
	return 1;
}
int32 scriptlib::card_is_attack_above(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 atk = lua_tointeger(L, 2);
	if(!(pcard->data.type & TYPE_MONSTER) && !(pcard->current.location & LOCATION_MZONE))
		lua_pushboolean(L, 0);
	else {
		int _atk = pcard->get_attack();
		lua_pushboolean(L, _atk >= atk);
	}
	return 1;
}
int32 scriptlib::card_is_defence_below(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 def = lua_tointeger(L, 2);
	if(!(pcard->data.type & TYPE_MONSTER) && !(pcard->current.location & LOCATION_MZONE))
		lua_pushboolean(L, 0);
	else {
		int _def = pcard->get_defence();
		lua_pushboolean(L, _def >= 0 && _def <= def);
	}
	return 1;
}
int32 scriptlib::card_is_defence_above(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 def = lua_tointeger(L, 2);
	if(!(pcard->data.type & TYPE_MONSTER) && !(pcard->current.location & LOCATION_MZONE))
		lua_pushboolean(L, 0);
	else {
		int _def = pcard->get_defence();
		lua_pushboolean(L, _def >= def);
	}
	return 1;
}
int32 scriptlib::card_is_public(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if(pcard->is_affected_by_effect(EFFECT_PUBLIC))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_forbidden(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if(pcard->is_affected_by_effect(EFFECT_FORBIDDEN))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_able_to_change_controler(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if(pcard->is_capable_change_control())
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_is_controler_can_be_changed(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	if(pcard->is_control_can_be_changed())
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::card_add_counter(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 countertype = lua_tointeger(L, 2);
	uint32 count = lua_tointeger(L, 3);
	lua_pushboolean(L, pcard->add_counter(countertype, count));
	return 1;
}
int32 scriptlib::card_remove_counter(lua_State *L) {
	check_action_permission(L);
	check_param_count(L, 5);
	check_param(L, PARAM_TYPE_CARD, 1);
	card * pcard = *(card**) lua_touserdata(L, 1);
	uint32 rplayer = lua_tointeger(L, 2);
	uint32 countertype = lua_tointeger(L, 3);
	uint32 count = lua_tointeger(L, 4);
	uint32 reason = lua_tointeger(L, 5);
	pcard->pduel->game_field->remove_counter(reason, pcard, rplayer, 0, 0, countertype, count);
	return lua_yield(L, 0);
}
int32 scriptlib::card_get_counter(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 countertype = lua_tointeger(L, 2);
	lua_pushinteger(L, pcard->get_counter(countertype));
	return 1;
}
int32 scriptlib::card_enable_counter_permit(lua_State *L) {
	check_param_count(L, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 countertype = lua_tointeger(L, 2);
	effect* peffect = pcard->pduel->new_effect();
	peffect->type = EFFECT_TYPE_SINGLE;
	peffect->code = EFFECT_COUNTER_PERMIT | countertype;
	peffect->flag = EFFECT_FLAG_SINGLE_RANGE;
	if(pcard->data.type & TYPE_MONSTER)
		peffect->range = LOCATION_MZONE;
	else
		peffect->range = LOCATION_SZONE;
	pcard->add_effect(peffect);
	return 0;
}
int32 scriptlib::card_set_counter_limit(lua_State *L) {
	check_param_count(L, 3);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 countertype = lua_tointeger(L, 2);
	int32 limit = lua_tointeger(L, 3);
	effect* peffect = pcard->pduel->new_effect();
	peffect->type = EFFECT_TYPE_SINGLE;
	peffect->code = EFFECT_COUNTER_LIMIT | countertype;
	peffect->value = limit;
	pcard->add_effect(peffect);
	return 0;
}
int32 scriptlib::card_is_can_turn_set(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_capable_turn_set(pcard->pduel->game_field->core.reason_player));
	return 1;
}
int32 scriptlib::card_is_can_add_counter(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 countertype = lua_tointeger(L, 2);
	uint32 count = lua_tointeger(L, 3);
	lua_pushboolean(L, pcard->is_can_add_counter(countertype, count));
	return 1;
}
int32 scriptlib::card_is_can_remove_counter(lua_State *L) {
	check_param_count(L, 5);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 playerid = lua_tointeger(L, 2);
	if(playerid != 0 && playerid != 1)
		return 0;
	uint32 countertype = lua_tointeger(L, 3);
	uint32 count = lua_tointeger(L, 4);
	uint32 reason = lua_tointeger(L, 5);
	lua_pushboolean(L, pcard->pduel->game_field->is_player_can_remove_counter(playerid, pcard, 0, 0, countertype, count, reason));
	return 1;
}
int32 scriptlib::card_is_can_be_fusion_material(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	lua_pushboolean(L, pcard->is_can_be_fusion_material());
	return 1;
}
int32 scriptlib::card_is_can_be_synchro_material(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* scard = 0;
	if(!lua_isnil(L, 2)) {
		check_param(L, PARAM_TYPE_CARD, 2);
		scard = *(card**) lua_touserdata(L, 2);
	}
	lua_pushboolean(L, pcard->is_can_be_synchro_material(scard));
	return 1;
}
int32 scriptlib::card_is_can_be_xyz_material(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* scard = 0;
	if(!lua_isnil(L, 2)) {
		check_param(L, PARAM_TYPE_CARD, 2);
		scard = *(card**) lua_touserdata(L, 2);
	}
	lua_pushboolean(L, pcard->is_can_be_exceed_material(scard));
	return 1;
}
int32 scriptlib::card_check_fusion_material(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	group* pgroup = 0;
	if(lua_gettop(L) > 1) {
		check_param(L, PARAM_TYPE_GROUP, 2);
		pgroup = *(group**) lua_touserdata(L, 2);
	}
	card* cg = 0;
	if(lua_gettop(L) > 2) {
		check_param(L, PARAM_TYPE_CARD, 3);
		cg = *(card**) lua_touserdata(L, 3);
	}
	lua_pushboolean(L, pcard->fution_check(pgroup, cg));
	return 1;
}
int32 scriptlib::card_is_immune_to_effect(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_EFFECT, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	effect* peffect = *(effect**) lua_touserdata(L, 2);
	lua_pushboolean(L, !pcard->is_affect_by_effect(peffect));
	return 1;
}
int32 scriptlib::card_is_can_be_effect_target(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	duel* pduel = pcard->pduel;
	effect* peffect = pduel->game_field->core.reason_effect;
	if(lua_gettop(L) > 1) {
		check_param(L, PARAM_TYPE_EFFECT, 2);
		peffect = *(effect**) lua_touserdata(L, 2);
	}
	lua_pushboolean(L, pcard->is_capable_be_effect_target(peffect, pduel->game_field->core.reason_player));
	return 1;
}
int32 scriptlib::card_is_can_be_battle_target(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	card* bcard = *(card**) lua_touserdata(L, 2);
	lua_pushboolean(L, pcard->is_capable_be_battle_target(bcard));
	return 1;
}
int32 scriptlib::card_add_trap_monster_attribute(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	int32 is_effect = lua_toboolean(L, 2);
	card* pcard = *(card**) lua_touserdata(L, 1);
	duel* pduel = pcard->pduel;
	effect* peffect = pduel->new_effect();
	peffect->owner = pcard;
	peffect->type = EFFECT_TYPE_SINGLE;
	peffect->code = EFFECT_ADD_TYPE;
	peffect->flag = EFFECT_FLAG_CANNOT_DISABLE;
	peffect->reset_flag = RESET_EVENT + 0x1fc0000;
	if(is_effect)
		peffect->value = TYPE_MONSTER + TYPE_EFFECT + TYPE_TRAPMONSTER;
	else
		peffect->value = TYPE_MONSTER + TYPE_NORMAL + TYPE_TRAPMONSTER;
	pcard->add_effect(peffect);
	peffect = pduel->new_effect();
	peffect->owner = pcard;
	peffect->type = EFFECT_TYPE_FIELD;
	peffect->range = LOCATION_MZONE;
	peffect->code = EFFECT_USE_EXTRA_SZONE;
	peffect->flag = EFFECT_FLAG_CANNOT_DISABLE;
	peffect->reset_flag = RESET_EVENT + 0x1fc0000;
	peffect->value = 1 + (0x10000 << pcard->previous.sequence);
	pcard->add_effect(peffect);
	return 0;
}
int32 scriptlib::card_cancel_to_grave(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	pcard->set_status(STATUS_LEAVE_CONFIRMED, FALSE);
	return 0;
}
int32 scriptlib::card_get_tribute_requirement(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	int32 rcount = pcard->get_summon_tribute_count();
	lua_pushinteger(L, rcount & 0xffff);
	lua_pushinteger(L, (rcount >> 16) & 0xffff);
	return 2;
}
