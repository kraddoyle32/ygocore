/*
 * interpreter.cpp
 *
 *  Created on: 2010-4-28
 *      Author: Argon
 */

#include <stdio.h>
#include <iostream>
#include "duel.h"
#include "interpreter.h"

static const struct luaL_reg bitlib[] = {
	{ "band", scriptlib::bit_and },
	{ "bor", scriptlib::bit_or },
	{ "bxor", scriptlib::bit_xor },
	{ "bls", scriptlib::bit_ls },
	{ "brs", scriptlib::bit_rs },
	{ NULL, NULL }
};
static const struct luaL_reg cardlib[] = {
	{ "GetCode", scriptlib::card_get_code },
	{ "GetOriginalCode", scriptlib::card_get_origin_code },
	{ "IsSetCard", scriptlib::card_is_set_card },
	{ "GetType", scriptlib::card_get_type },
	{ "GetOriginalType", scriptlib::card_get_origin_type },
	{ "GetLevel", scriptlib::card_get_level },
	{ "GetSynchroLevel", scriptlib::card_get_synchro_level },
	{ "GetRitualLevel", scriptlib::card_get_ritual_level },
	{ "GetOriginalLevel", scriptlib::card_get_origin_level },
	{ "GetAttribute", scriptlib::card_get_attribute },
	{ "GetOriginalAttribute", scriptlib::card_get_origin_attribute },
	{ "GetRace", scriptlib::card_get_race },
	{ "GetOriginalRace", scriptlib::card_get_origin_race },
	{ "GetAttack", scriptlib::card_get_attack },
	{ "GetBaseAttack", scriptlib::card_get_origin_attack },
	{ "GetTextAttack", scriptlib::card_get_text_attack },
	{ "GetDefence",	scriptlib::card_get_defence },
	{ "GetBaseDefence", scriptlib::card_get_origin_defence },
	{ "GetTextDefence", scriptlib::card_get_text_defence },
	{ "GetOwner", scriptlib::card_get_owner },
	{ "GetControler", scriptlib::card_get_controler },
	{ "GetPreviousControler", scriptlib::card_get_previous_controler },
	{ "GetReason", scriptlib::card_get_reason },
	{ "GetReasonCard", scriptlib::card_get_reason_card },
	{ "GetReasonPlayer", scriptlib::card_get_reason_player },
	{ "GetReasonEffect", scriptlib::card_get_reason_effect },
	{ "GetPosition", scriptlib::card_get_position },
	{ "GetPreviousPosition", scriptlib::card_get_previous_position },
	{ "GetBattlePosition", scriptlib::card_get_battle_position },
	{ "GetLocation", scriptlib::card_get_location },
	{ "GetPreviousLocation", scriptlib::card_get_previous_location },
	{ "GetSequence", scriptlib::card_get_sequence },
	{ "GetPreviousSequence", scriptlib::card_get_previous_sequence },
	{ "GetSummonType", scriptlib::card_get_summon_type },
	{ "GetSummonPlayer", scriptlib::card_get_summon_player },
	{ "GetDestination", scriptlib::card_get_destination },
	{ "GetTurnID", scriptlib::card_get_turnid },
	{ "GetFieldID", scriptlib::card_get_fieldid },
	{ "IsCode", scriptlib::card_is_code },
	{ "IsType", scriptlib::card_is_type },
	{ "IsRace", scriptlib::card_is_race },
	{ "IsAttribute", scriptlib::card_is_attribute },
	{ "IsReason", scriptlib::card_is_reason },
	{ "IsStatus", scriptlib::card_is_status },
	{ "IsNotTuner", scriptlib::card_is_not_tuner },
	{ "SetStatus", scriptlib::card_set_status },
	{ "IsDualState", scriptlib::card_is_dual_state },
	{ "EnableDualState", scriptlib::card_enable_dual_state },
	{ "SetTurnCounter",	scriptlib::card_set_turn_counter },
	{ "GetTurnCounter", scriptlib::card_get_turn_counter },
	{ "SetMaterial", scriptlib::card_set_material },
	{ "GetMaterial", scriptlib::card_get_material },
	{ "GetMaterialCount", scriptlib::card_get_material_count },
	{ "GetEquipGroup", scriptlib::card_get_equip_group },
	{ "GetEquipCount", scriptlib::card_get_equip_count },
	{ "GetEquipTarget", scriptlib::card_get_equip_target },
	{ "GetPreviousEquipTarget", scriptlib::card_get_pre_equip_target },
	{ "CheckEquipTarget", scriptlib::card_check_equip_target },
	{ "GetUnionCount", scriptlib::card_get_union_count },
	{ "GetOverlayGroup", scriptlib::card_get_overlay_group },
	{ "GetOverlayCount", scriptlib::card_get_overlay_count },
	{ "CheckRemoveOverlayCard", scriptlib::card_check_remove_overlay_card },
	{ "RemoveOverlayCard", scriptlib::card_remove_overlay_card },
	{ "GetAttackedGroup", scriptlib::card_get_attacked_group },
	{ "GetAttackedGroupCount", scriptlib::card_get_attacked_group_count },
	{ "GetAttackedCount", scriptlib::card_get_attacked_count },
	{ "GetBattledGroup", scriptlib::card_get_battled_group },
	{ "GetBattledGroupCount", scriptlib::card_get_battled_group_count },
	{ "GetAttackAnnouncedCount", scriptlib::card_get_attack_announced_count },
	{ "IsDirectAttacked", scriptlib::card_is_direct_attacked },
	{ "SetCardTarget", scriptlib::card_set_card_target },
	{ "GetCardTarget", scriptlib::card_get_card_target },
	{ "GetCardTargetCount", scriptlib::card_get_card_target_count },
	{ "IsHasCardTarget", scriptlib::card_is_has_card_target },
	{ "GetOwnerTarget", scriptlib::card_get_owner_target },
	{ "GetOwnerTargetCount", scriptlib::card_get_owner_target_count },
	{ "GetActivateEffect", scriptlib::card_get_activate_effect },
	{ "RegisterEffect", scriptlib::card_register_effect },
	{ "IsHasEffect", scriptlib::card_is_has_effect },
	{ "GetEffectCount", scriptlib::card_get_effect_count },
	{ "RegisterFlagEffect", scriptlib::card_register_flag_effect },
	{ "GetFlagEffect", scriptlib::card_get_flag_effect },
	{ "ResetFlagEffect", scriptlib::card_reset_flag_effect },
	{ "CreateRelation", scriptlib::card_create_relation },
	{ "ReleaseRelation", scriptlib::card_release_relation },
	{ "CreateEffectRelation", scriptlib::card_create_effect_relation },
	{ "IsRelateToEffect", scriptlib::card_is_relate_to_effect },
	{ "IsRelateToCard", scriptlib::card_is_relate_to_card },
	{ "IsRelateToBattle", scriptlib::card_is_relate_to_battle },
	{ "CopyEffect", scriptlib::card_copy_effect },
	{ "EnableReviveLimit", scriptlib::card_enable_revive_limit },
	{ "CompleteProcedure", scriptlib::card_complete_procedure },
	{ "IsDisabled", scriptlib::card_is_disabled },
	{ "IsDestructable", scriptlib::card_is_destructable },
	{ "IsSummonableCard", scriptlib::card_is_summonable },
	{ "IsSpecialSummonable", scriptlib::card_is_special_summonable },
	{ "IsSynchroSummonable", scriptlib::card_is_synchro_summonable },
	{ "IsSummonable", scriptlib::card_is_can_be_summoned },
	{ "IsMSetable", scriptlib::card_is_msetable },
	{ "IsSSetable", scriptlib::card_is_ssetable },
	{ "IsCanBeSpecialSummoned", scriptlib::card_is_can_be_special_summoned },
	{ "IsAbleToHand", scriptlib::card_is_able_to_hand },
	{ "IsAbleToDeck", scriptlib::card_is_able_to_deck },
	{ "IsAbleToExtra", scriptlib::card_is_able_to_extra },
	{ "IsAbleToGrave", scriptlib::card_is_able_to_grave },
	{ "IsAbleToRemove", scriptlib::card_is_able_to_remove },
	{ "IsAbleToHandAsCost", scriptlib::card_is_able_to_hand_as_cost },
	{ "IsAbleToDeckAsCost", scriptlib::card_is_able_to_deck_as_cost },
	{ "IsAbleToExtraAsCost", scriptlib::card_is_able_to_extra_as_cost },
	{ "IsAbleToGraveAsCost", scriptlib::card_is_able_to_grave_as_cost },
	{ "IsAbleToRemoveAsCost", scriptlib::card_is_able_to_remove_as_cost },
	{ "IsReleaseable", scriptlib::card_is_releaseable },
	{ "IsDiscardable", scriptlib::card_is_discardable },
	{ "IsAttackable", scriptlib::card_is_attackable },
	{ "IsChainAttackable", scriptlib::card_is_chain_attackable },
	{ "IsFaceup", scriptlib::card_is_faceup },
	{ "IsAttack", scriptlib::card_is_attack },
	{ "IsFacedown", scriptlib::card_is_facedown },
	{ "IsDefence", scriptlib::card_is_defence },
	{ "IsPosition", scriptlib::card_is_position },
	{ "IsPreviousPosition", scriptlib::card_is_pre_position },
	{ "IsControler", scriptlib::card_is_controler },
	{ "IsOnField", scriptlib::card_is_onfield },
	{ "IsLocation", scriptlib::card_is_location },
	{ "IsPreviousLocation", scriptlib::card_is_pre_location },
	{ "IsLevelBelow", scriptlib::card_is_level_below },
	{ "IsLevelAbove", scriptlib::card_is_level_above },
	{ "IsAttackBelow", scriptlib::card_is_attack_below },
	{ "IsAttackAbove", scriptlib::card_is_attack_above },
	{ "IsDefenceBelow", scriptlib::card_is_defence_below },
	{ "IsDefenceAbove", scriptlib::card_is_defence_above },
	{ "IsPublic", scriptlib::card_is_public },
	{ "IsForbidden", scriptlib::card_is_forbidden },
	{ "IsAbleToChangeControler", scriptlib::card_is_able_to_change_controler },
	{ "IsControlerCanBeChanged", scriptlib::card_is_controler_can_be_changed },
	{ "AddCounter", scriptlib::card_add_counter },
	{ "RemoveCounter", scriptlib::card_remove_counter },
	{ "GetCounter", scriptlib::card_get_counter },
	{ "EnableCounterPermit", scriptlib::card_enable_counter_permit },
	{ "SetCounterLimit", scriptlib::card_set_counter_limit },
	{ "IsCanTurnSet", scriptlib::card_is_can_turn_set },
	{ "IsCanAddCounter", scriptlib::card_is_can_add_counter },
	{ "IsCanRemoveCounter", scriptlib::card_is_can_remove_counter },
	{ "IsCanBeFusionMaterial", scriptlib::card_is_can_be_fusion_material },
	{ "IsCanBeSynchroMaterial", scriptlib::card_is_can_be_synchro_material },
	{ "IsCanBeXyzMaterial", scriptlib::card_is_can_be_xyz_material },
	{ "CheckFusionMaterial", scriptlib::card_check_fusion_material },
	{ "IsImmuneToEffect", scriptlib::card_is_immune_to_effect },
	{ "IsCanBeEffectTarget", scriptlib::card_is_can_be_effect_target },
	{ "IsCanBeBattleTarget", scriptlib::card_is_can_be_battle_target },
	{ "AddTrapMonsterAttribute", scriptlib::card_add_trap_monster_attribute },
	{ "CancelToGrave", scriptlib::card_cancel_to_grave },
	{ "GetTributeRequirement", scriptlib::card_get_tribute_requirement },
	{ NULL, NULL }
};

static const struct luaL_reg effectlib[] = {
	{ "CreateEffect", scriptlib::effect_new },
	{ "Clone", scriptlib::effect_clone },
	{ "Reset", scriptlib::effect_reset },
	{ "SetDescription", scriptlib::effect_set_description },
	{ "SetCode", scriptlib::effect_set_code },
	{ "SetRange", scriptlib::effect_set_range },
	{ "SetTargetRange", scriptlib::effect_set_target_range },
	{ "SetAbsoluteRange", scriptlib::effect_set_absolute_range },
	{ "SetCountLimit", scriptlib::effect_set_count_limit },
	{ "SetReset", scriptlib::effect_set_reset },
	{ "SetType", scriptlib::effect_set_type },
	{ "SetProperty", scriptlib::effect_set_property },
	{ "SetLabel", scriptlib::effect_set_label },
	{ "SetLabelObject", scriptlib::effect_set_label_object },
	{ "SetCategory", scriptlib::effect_set_category },
	{ "SetHintTiming", scriptlib::effect_set_hint_timing },
	{ "SetCondition", scriptlib::effect_set_condition },
	{ "SetTarget", scriptlib::effect_set_target },
	{ "SetCost", scriptlib::effect_set_cost },
	{ "SetValue", scriptlib::effect_set_value },
	{ "SetOperation", scriptlib::effect_set_operation },
	{ "SetOwnerPlayer", scriptlib::effect_set_owner_player },
	{ "GetDescription", scriptlib::effect_get_description },
	{ "GetCode", scriptlib::effect_get_code },
	{ "GetType", scriptlib::effect_get_type },
	{ "GetProperty", scriptlib::effect_get_property },
	{ "GetLabel", scriptlib::effect_get_label },
	{ "GetLabelObject", scriptlib::effect_get_label_object },
	{ "GetCategory", scriptlib::effect_get_category },
	{ "GetOwner", scriptlib::effect_get_owner },
	{ "GetHandler", scriptlib::effect_get_handler },
	{ "GetCondition", scriptlib::effect_get_condition },
	{ "GetTarget", scriptlib::effect_get_target },
	{ "GetCost", scriptlib::effect_get_cost },
	{ "GetValue", scriptlib::effect_get_value },
	{ "GetOperation", scriptlib::effect_get_operation },
	{ "GetOwnerPlayer", scriptlib::effect_get_owner_player },
	{ "GetHandlerPlayer", scriptlib::effect_get_handler_player },
	{ "IsHasProperty", scriptlib::effect_is_has_property },
	{ "IsHasCategory", scriptlib::effect_is_has_category },
	{ "IsHasType", scriptlib::effect_is_has_type },
	{ NULL, NULL }
};

static const struct luaL_reg grouplib[] = {
	{ "CreateGroup", scriptlib::group_new },
	{ "KeepAlive", scriptlib::group_keep_alive },
	{ "DeleteGroup", scriptlib::group_delete },
	{ "Clone", scriptlib::group_clone },
	{ "FromCards", scriptlib::group_from_cards },
	{ "Clear", scriptlib::group_clear },
	{ "AddCard", scriptlib::group_add_card },
	{ "RemoveCard", scriptlib::group_remove_card },
	{ "GetNext", scriptlib::group_get_next },
	{ "GetFirst", scriptlib::group_get_first },
	{ "GetCount", scriptlib::group_get_count },
	{ "ForEach", scriptlib::group_for_each },
	{ "Filter", scriptlib::group_filter },
	{ "FilterCount", scriptlib::group_filter_count },
	{ "FilterSelect", scriptlib::group_filter_select },
	{ "Select", scriptlib::group_select },
	{ "RandomSelect", scriptlib::group_random_select },
	{ "IsExists", scriptlib::group_is_exists },
	{ "CheckWithSumEqual", scriptlib::group_check_with_sum_equal },
	{ "SelectWithSumEqual", scriptlib::group_select_with_sum_equal },
	{ "CheckWithSumGreater", scriptlib::group_check_with_sum_greater },
	{ "SelectWithSumGreater", scriptlib::group_select_with_sum_greater },
	{ "GetMinGroup", scriptlib::group_get_min_group },
	{ "GetMaxGroup", scriptlib::group_get_max_group },
	{ "GetSum", scriptlib::group_get_sum },
	{ "GetClassCount", scriptlib::group_get_class_count },
	{ "Remove", scriptlib::group_remove },
	{ "Merge", scriptlib::group_merge },
	{ "Sub", scriptlib::group_sub },
	{ "IsContains", scriptlib::group_is_contains },
	{ "SearchCard", scriptlib::group_search_card },
	{ NULL, NULL }
};

static const struct luaL_Reg duellib[] = {
	{ "GetLP", scriptlib::duel_get_lp },
	{ "SetLP", scriptlib::duel_set_lp },
	{ "GetTurnPlayer", scriptlib::duel_get_turn_player },
	{ "GetTurnCount", scriptlib::duel_get_turn_count },
	{ "GetDrawCount", scriptlib::duel_get_draw_count },
	{ "DebugMessage", scriptlib::duel_debug_message },
	{ "RegisterEffect", scriptlib::duel_register_effect },
	{ "RegisterFlagEffect", scriptlib::duel_register_flag_effect },
	{ "GetFlagEffect", scriptlib::duel_get_flag_effect },
	{ "ResetFlagEffect", scriptlib::duel_reset_flag_effect },
	{ "Destroy", scriptlib::duel_destroy },
	{ "Remove", scriptlib::duel_remove },
	{ "SendtoGrave", scriptlib::duel_sendto_grave },
	{ "SendtoHand", scriptlib::duel_sendto_hand },
	{ "SendtoDeck", scriptlib::duel_sendto_deck },
	{ "GetOperatedGroup", scriptlib::duel_get_operated_group },
	{ "Summon", scriptlib::duel_summon },
	{ "SpecialSummonRule", scriptlib::duel_special_summon_rule },
	{ "SynchroSummon", scriptlib::duel_synchro_summon },
	{ "MSet", scriptlib::duel_setm },
	{ "SSet", scriptlib::duel_sets },
	{ "CreateToken", scriptlib::duel_create_token },
	{ "SpecialSummon", scriptlib::duel_special_summon },
	{ "SpecialSummonStep", scriptlib::duel_special_summon_step },
	{ "SpecialSummonComplete", scriptlib::duel_special_summon_complete },
	{ "RemoveCounter", scriptlib::duel_remove_counter },
	{ "IsCanRemoveCounter", scriptlib::duel_is_can_remove_counter },
	{ "GetCounter", scriptlib::duel_get_counter },
	{ "ChangePosition", scriptlib::duel_change_form },
	{ "Release", scriptlib::duel_release },
	{ "MoveToField", scriptlib::duel_move_to_field },
	{ "ReturnToField", scriptlib::duel_return_to_field },
	{ "MoveSequence", scriptlib::duel_move_sequence },
	{ "SetChainLimit", scriptlib::duel_set_chain_limit },
	{ "SetChainLimitTillChainEnd", scriptlib::duel_set_chain_limit_p },
	{ "GetChainMaterial", scriptlib::duel_get_chain_material },
	{ "ConfirmDecktop", scriptlib::duel_confirm_decktop },
	{ "ConfirmCards", scriptlib::duel_confirm_cards },
	{ "SortDecktop", scriptlib::duel_sort_decktop },
	{ "CheckEvent", scriptlib::duel_check_event },
	{ "RaiseEvent", scriptlib::duel_raise_event },
	{ "RaiseSingleEvent", scriptlib::duel_raise_single_event },
	{ "CheckTiming", scriptlib::duel_check_timing },
	{ "GetEnvironment", scriptlib::duel_get_environment },
	{ "Win", scriptlib::duel_win },
	{ "Draw", scriptlib::duel_draw },
	{ "Damage", scriptlib::duel_damage },
	{ "Recover", scriptlib::duel_recover },
	{ "Equip", scriptlib::duel_equip },
	{ "GetControl", scriptlib::duel_get_control },
	{ "SwapControl", scriptlib::duel_swap_control },
	{ "CheckLPCost", scriptlib::duel_check_lp_cost },
	{ "PayLPCost", scriptlib::duel_pay_lp_cost },
	{ "DiscardDeck", scriptlib::duel_discard_deck },
	{ "DiscardHand", scriptlib::duel_discard_hand },
	{ "DisableShuffleCheck", scriptlib::duel_disable_shuffle_check },
	{ "ShuffleDeck", scriptlib::duel_shuffle_deck },
	{ "ShuffleHand", scriptlib::duel_shuffle_hand },
	{ "ShuffleSetCard", scriptlib::duel_shuffle_setcard },
	{ "ChangeAttacker", scriptlib::duel_change_attacker },
	{ "ReplaceAttacker", scriptlib::duel_replace_attacker },
	{ "ChangeAttackTarget", scriptlib::duel_change_attack_target },
	{ "ReplaceAttackTarget", scriptlib::duel_replace_attack_target },
	{ "GetBattleDamage", scriptlib::duel_get_battle_damage },
	{ "ChangeBattleDamage", scriptlib::duel_change_battle_damage },
	{ "ChangeTargetCard", scriptlib::duel_change_target },
	{ "ChangeTargetPlayer", scriptlib::duel_change_target_player },
	{ "ChangeTargetParam", scriptlib::duel_change_target_param },
	{ "BreakEffect", scriptlib::duel_break_effect },
	{ "ChangeChainOperation", scriptlib::duel_change_effect },
	{ "NegateActivation", scriptlib::duel_disable_activate },
	{ "NegateEffect", scriptlib::duel_disable_effect },
	{ "NegateSummon", scriptlib::duel_disable_summon },
	{ "IncreaseSummonedCount", scriptlib::duel_increase_summon_count },
	{ "CheckSummonedCount", scriptlib::duel_check_summon_count },
	{ "GetLocationCount", scriptlib::duel_get_location_count },
	{ "GetFieldCard", scriptlib::duel_get_field_card },
	{ "CheckLocation", scriptlib::duel_check_location },
	{ "GetCurrentChain", scriptlib::duel_get_current_chain },
	{ "GetChainInfo", scriptlib::duel_get_chain_info },
	{ "GetFirstTarget", scriptlib::duel_get_first_target },
	{ "GetCurrentPhase", scriptlib::duel_get_current_phase },
	{ "SkipPhase", scriptlib::duel_skip_phase },
	{ "IsDamageCalculated", scriptlib::duel_is_damage_calculated },
	{ "GetAttacker", scriptlib::duel_get_attacker },
	{ "GetAttackTarget", scriptlib::duel_get_attack_target },
	{ "DisableAttack", scriptlib::duel_disable_attack },
	{ "ChainAttack", scriptlib::duel_chain_attack },
	{ "Readjust", scriptlib::duel_readjust },
	{ "GetFieldGroup", scriptlib::duel_get_field_group },
	{ "GetFieldGroupCount", scriptlib::duel_get_field_group_count },
	{ "GetDecktopGroup", scriptlib::duel_get_decktop_group },
	{ "GetMatchingGroup", scriptlib::duel_get_matching_group },
	{ "GetMatchingGroupCount", scriptlib::duel_get_matching_count },
	{ "GetFirstMatchingCard", scriptlib::duel_get_first_matching_card },
	{ "IsExistingMatchingCard", scriptlib::duel_is_existing_matching_card },
	{ "SelectMatchingCard", scriptlib::duel_select_matching_cards },
	{ "GetReleaseGroup", scriptlib::duel_get_release_group },
	{ "GetReleaseGroupCount", scriptlib::duel_get_release_group_count },
	{ "CheckReleaseGroup", scriptlib::duel_check_release_group },
	{ "SelectReleaseGroup", scriptlib::duel_select_release_group },
	{ "GetTributeGroup", scriptlib::duel_get_tribute_group },
	{ "GetTributeCount", scriptlib::duel_get_tribute_count },
	{ "SelectTribute", scriptlib::duel_select_tribute },
	{ "GetTargetCount", scriptlib::duel_get_target_count },
	{ "IsExistingTarget", scriptlib::duel_is_existing_target },
	{ "SelectTarget", scriptlib::duel_select_target },
	{ "SelectFusionMaterial", scriptlib::duel_select_fusion_material },
	{ "SetFusionMaterial", scriptlib::duel_set_fusion_material },
	{ "SetSynchroMaterial", scriptlib::duel_set_synchro_material },
	{ "SelectSynchroMaterial", scriptlib::duel_select_synchro_material },
	{ "CheckSynchroMaterial", scriptlib::duel_check_synchro_material },
	{ "SelectTunerMaterial", scriptlib::duel_select_tuner_material },
	{ "CheckTunerMaterial", scriptlib::duel_check_tuner_material },
	{ "GetRitualMaterial", scriptlib::duel_get_ritual_material },
	{ "ReleaseRitualMaterial", scriptlib::duel_release_ritual_material },
	{ "SetTargetCard", scriptlib::duel_set_target_card },
	{ "ClearTargetCard", scriptlib::duel_clear_target_card },
	{ "SetTargetPlayer", scriptlib::duel_set_target_player },
	{ "SetTargetParam", scriptlib::duel_set_target_param },
	{ "SetOperationInfo", scriptlib::duel_set_operation_info },
	{ "GetOperationInfo", scriptlib::duel_get_operation_info },
	{ "GetOperationCount", scriptlib::duel_get_operation_count },
	{ "GetXyzMaterial", scriptlib::duel_get_xyz_material },
	{ "Overlay", scriptlib::duel_overlay },
	{ "GetOverlayGroup", scriptlib::duel_get_overlay_group },
	{ "GetOverlayCount", scriptlib::duel_get_overlay_count },
	{ "CheckRemoveOverlayCard", scriptlib::duel_check_remove_overlay_card },
	{ "RemoveOverlayCard", scriptlib::duel_remove_overlay_card },
	{ "Hint", scriptlib::duel_hint },
	{ "SelectEffectYesNo", scriptlib::duel_select_effect_yesno },
	{ "SelectYesNo", scriptlib::duel_select_yesno },
	{ "SelectOption", scriptlib::duel_select_option },
	{ "SelectSequence", scriptlib::duel_select_sequence },
	{ "SelectPosition", scriptlib::duel_select_position },
	{ "SelectDisableField", scriptlib::duel_select_disable_field },
	{ "AnnounceRace", scriptlib::duel_announce_race },
	{ "AnnounceAttribute", scriptlib::duel_announce_attribute },
	{ "AnnounceLevel", scriptlib::duel_announce_level },
	{ "AnnounceCard", scriptlib::duel_announce_card },
	{ "AnnounceType", scriptlib::duel_announce_type },
	{ "AnnounceNumber", scriptlib::duel_announce_number },
	{ "AnnounceCoin", scriptlib::duel_announce_coin },
	{ "TossCoin", scriptlib::duel_toss_coin },
	{ "TossDice", scriptlib::duel_toss_dice },
	{ "SetCoinResult", scriptlib::duel_set_coin_result },
	{ "SetDiceResult", scriptlib::duel_set_dice_result },
	{ "IsPlayerAffectedByEffect", scriptlib::duel_is_player_affected_by_effect },
	{ "IsPlayerCanDraw", scriptlib::duel_is_player_can_draw },
	{ "IsPlayerCanDiscardDeck", scriptlib::duel_is_player_can_discard_deck },
	{ "IsPlayerCanDiscardDeckAsCost", scriptlib::duel_is_player_can_discard_deck_as_cost },
	{ "IsPlayerCanSummon", scriptlib::duel_is_player_can_summon },
	{ "IsPlayerCanSpecialSummon", scriptlib::duel_is_player_can_spsummon },
	{ "IsPlayerCanFlipSummon", scriptlib::duel_is_player_can_flipsummon },
	{ "IsPlayerCanSpecialSummonMonster", scriptlib::duel_is_player_can_spsummon_monster },
	{ "IsPlayerCanRelease", scriptlib::duel_is_player_can_release },
	{ "IsPlayerCanRemove", scriptlib::duel_is_player_can_remove },
	{ "IsPlayerCanSendtoHand", scriptlib::duel_is_player_can_send_to_hand },
	{ "IsPlayerCanSendtoGrave", scriptlib::duel_is_player_can_send_to_grave },
	{ "IsPlayerCanSendtoDeck", scriptlib::duel_is_player_can_send_to_deck },
	{ "IsChainInactivatable", scriptlib::duel_is_chain_inactivatable },
	{ "IsChainDisablable", scriptlib::duel_is_chain_disablable },
	{ "CheckChainTarget", scriptlib::duel_check_chain_target },
	{ "CheckChainUniqueness", scriptlib::duel_check_chain_uniqueness },
	{ "CheckSummonActivity", scriptlib::duel_check_summon_activity },
	{ "CheckNormalSummonActivity", scriptlib::duel_check_normal_summon_activity },
	{ "CheckFlipSummonActivity", scriptlib::duel_check_flip_summon_activity },
	{ "CheckSpecialSummonActivity", scriptlib::duel_check_special_summon_activity },
	{ "CheckAttackActivity", scriptlib::duel_check_attack_activity },
	{ "CheckPhaseActivity", scriptlib::duel_check_phase_activity },
	{ "VenomSwampCheck", scriptlib::duel_venom_swamp_check },
	{ "SwapDeckAndGrave", scriptlib::duel_swap_deck_and_grave },
	{ "MajesticCopy", scriptlib::duel_majestic_copy },
	{ NULL, NULL }
};

interpreter::interpreter(duel* pd) {
	lua_state = luaL_newstate();
	current_state = lua_state;
	pduel = pd;
	no_action = 0;
	call_depth = 0;
	set_duel_info(lua_state, pd);
	//Initial
	luaL_openlibs(lua_state);
	lua_pushnil(lua_state);
	lua_setglobal(lua_state, "file");
	lua_pushnil(lua_state);
	lua_setglobal(lua_state, "io");
	lua_pushnil(lua_state);
	lua_setglobal(lua_state, "os");
	luaL_openlib(lua_state, "bit", bitlib, 0);
	lua_pop(lua_state, 1);
	//open all libs
	luaL_openlib(lua_state, "Card", cardlib, 0);
	lua_pushstring(lua_state, "__index");
	lua_pushvalue(lua_state, -2);
	lua_rawset(lua_state, -3);
	lua_pop(lua_state, 1);
	luaL_openlib(lua_state, "Effect", effectlib, 0);
	lua_pushstring(lua_state, "__index");
	lua_pushvalue(lua_state, -2);
	lua_rawset(lua_state, -3);
	lua_pop(lua_state, 1);
	luaL_openlib(lua_state, "Group", grouplib, 0);
	lua_pushstring(lua_state, "__index");
	lua_pushvalue(lua_state, -2);
	lua_rawset(lua_state, -3);
	lua_pop(lua_state, 1);
	luaL_openlib(lua_state, "Duel", duellib, 0);
	lua_pop(lua_state, 1);
	//extra scripts
	load_script((char*) "constant.lua");
	load_script((char*) "utility.lua");
}
interpreter::~interpreter() {
	lua_close(lua_state);
}
int32 interpreter::register_card(card *pcard) {
	//create a card in by userdata
	card ** ppcard = (card**) lua_newuserdata(lua_state, sizeof(card*));
	*ppcard = pcard;
	pcard->ref_handle = luaL_ref(lua_state, LUA_REGISTRYINDEX);
	//some userdata may be created in script like token so use current_state
	lua_rawgeti(current_state, LUA_REGISTRYINDEX, pcard->ref_handle);
	//load script
	load_card_script(pcard->data.code);
	//set metatable of pointer to base script
	lua_setmetatable(current_state, -2);
	lua_pop(current_state, 1);
	//Initial
	if(pcard->data.code && !(pcard->data.type & TYPE_NORMAL)) {
		pcard->set_status(STATUS_INITIALIZING, TRUE);
		add_param(pcard, PARAM_TYPE_CARD);
		call_card_function(pcard, (char*) "initial_effect", 1, 0);
		pcard->set_status(STATUS_INITIALIZING, FALSE);
	}
	pcard->cardid = pduel->game_field->infos.card_id++;
	return OPERATION_SUCCESS;
}
void interpreter::register_effect(effect *peffect) {
	if (!peffect)
		return;
	//create a effect in by userdata
	effect ** ppeffect = (effect**) lua_newuserdata(lua_state, sizeof(effect*));
	*ppeffect = peffect;
	peffect->ref_handle = luaL_ref(lua_state, LUA_REGISTRYINDEX);
	//set metatable of pointer to base script
	lua_rawgeti(lua_state, LUA_REGISTRYINDEX, peffect->ref_handle);
	lua_getfield(lua_state, LUA_GLOBALSINDEX, "Effect");
	lua_setmetatable(lua_state, -2);
	lua_pop(lua_state, 1);
}
void interpreter::unregister_effect(effect *peffect) {
	if (!peffect)
		return;
	if(peffect->condition)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, peffect->condition);
	if(peffect->cost)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, peffect->cost);
	if(peffect->target)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, peffect->target);
	if(peffect->operation)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, peffect->operation);
	if(peffect->value && (peffect->flag & EFFECT_FLAG_FUNC_VALUE))
		luaL_unref(lua_state, LUA_REGISTRYINDEX, peffect->value);
	luaL_unref(lua_state, LUA_REGISTRYINDEX, peffect->ref_handle);
	peffect->ref_handle = 0;
}
void interpreter::register_group(group *pgroup) {
	if (!pgroup)
		return;
	//create a group in by userdata
	group ** ppgroup = (group**) lua_newuserdata(lua_state, sizeof(group*));
	*ppgroup = pgroup;
	pgroup->ref_handle = luaL_ref(lua_state, LUA_REGISTRYINDEX);
	//set metatable of pointer to base script
	lua_rawgeti(lua_state, LUA_REGISTRYINDEX, pgroup->ref_handle);
	lua_getfield(lua_state, LUA_GLOBALSINDEX, "Group");
	lua_setmetatable(lua_state, -2);
	lua_pop(lua_state, 1);
}
void interpreter::unregister_group(group *pgroup) {
	if (!pgroup)
		return;
	luaL_unref(lua_state, LUA_REGISTRYINDEX, pgroup->ref_handle);
	pgroup->ref_handle = 0;
}
int32 interpreter::load_script(char* script_name) {
	int32 error;
	int32 len = 0;
	byte* buffer = read_script(script_name, &len);
	if (!buffer)
		return OPERATION_FAIL;
	no_action++;
	error = luaL_loadbuffer(current_state, (const char*) buffer, len, (const char*) script_name) || lua_pcall(current_state, 0, 0, 0);
	if (error) {
		sprintf(pduel->strbuffer, "%s", lua_tostring(current_state, -1));
		handle_message(pduel, 1);
		lua_pop(current_state, 1);
		no_action--;
		return OPERATION_FAIL;
	}
	no_action--;
	return OPERATION_SUCCESS;
}
int32 interpreter::load_card_script(uint32 code) {
	char class_name[20];
	char script_name[20];
	sprintf(class_name, "c%d", code);
	lua_getfield(current_state, LUA_GLOBALSINDEX, class_name);
	//if script is not loaded, create and load it
	if (lua_isnil(current_state, -1)) {
		lua_pop(current_state, 1);
		//create a table & set metatable
		lua_createtable(current_state, 0, 0);
		lua_setfield(current_state, LUA_GLOBALSINDEX, class_name);
		lua_getfield(current_state, LUA_GLOBALSINDEX, class_name);
		lua_getfield(current_state, LUA_GLOBALSINDEX, "Card");
		lua_setmetatable(current_state, -2);
		lua_pushstring(current_state, "__index");
		lua_pushvalue(current_state, -2);
		lua_rawset(current_state, -3);
		//load extra scripts
		sprintf(script_name, "c%d.lua", code);
		if (!load_script(script_name)) {
			return OPERATION_FAIL;
		}
	}
	return OPERATION_SUCCESS;
}
void interpreter::add_param(void *param, int32 type, bool front) {
	if(front)
		params.push_front(make_pair(param, type));
	else
		params.push_back(make_pair(param, type));
}
void interpreter::add_param(ptr param, int32 type, bool front) {
	if(front)
		params.push_front(make_pair((void*)param, type));
	else
		params.push_back(make_pair((void*)param, type));
}
void interpreter::push_param(lua_State* L) {
	param_list::iterator it;
	uint32 type;
	for (it = params.begin(); it != params.end(); ++it) {
		type = it->second;
		switch(type) {
		case PARAM_TYPE_INT:
			lua_pushinteger(L, (ptr) it->first);
			break;
		case PARAM_TYPE_STRING:
			lua_pushstring(L, (const char *) it->first);
			break;
		case PARAM_TYPE_BOOLEAN:
			lua_pushboolean(L, (ptr) it->first);
			break;
		case PARAM_TYPE_CARD: {
			if (it->first)
				lua_rawgeti(L, LUA_REGISTRYINDEX, ((card*)it->first)->ref_handle);
			else
				lua_pushnil(L);
			break;
		}
		case PARAM_TYPE_EFFECT: {
			if (it->first)
				lua_rawgeti(L, LUA_REGISTRYINDEX, ((effect*)it->first)->ref_handle);
			else
				lua_pushnil(L);
			break;
		}
		case PARAM_TYPE_GROUP: {
			if (it->first)
				lua_rawgeti(L, LUA_REGISTRYINDEX, ((group*)it->first)->ref_handle);
			else
				lua_pushnil(L);
			break;
		}
		case PARAM_TYPE_FUNCTION: {
			function2value(L, (int32)it->first);
			break;
		}
		case PARAM_TYPE_INDEX: {
			int32 index = (int32)it->first;
			if(index > 0)
				lua_pushvalue(L, index);
			else lua_pushnil(L);
			break;
		}
		}
	}
	params.clear();
}
int32 interpreter::call_function(int32 f, uint32 param_count, uint32 ret_count) {
	if (!f) {
		sprintf(pduel->strbuffer, "\"CallFunction\": attempt to call a null function.");
		handle_message(pduel, 1);
		params.clear();
		return OPERATION_FAIL;
	}
	if (param_count != params.size()) {
		sprintf(pduel->strbuffer, "\"CallFunction\": incorrect parameter count (%d expected, %d pushed)", param_count, params.size());
		handle_message(pduel, 1);
		params.clear();
		return OPERATION_FAIL;
	}
	function2value(current_state, f);
	if (!lua_isfunction(current_state, -1)) {
		sprintf(pduel->strbuffer, "\"CallFunction\": attempt to call an error function");
		handle_message(pduel, 1);
		lua_pop(current_state, 1);
		params.clear();
		return OPERATION_FAIL;
	}
	no_action++;
	call_depth++;
	push_param(current_state);
	if (lua_pcall(current_state, param_count, ret_count, 0)) {
		sprintf(pduel->strbuffer, lua_tostring(current_state, -1));
		handle_message(pduel, 1);
		lua_pop(current_state, 1);
		no_action--;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return OPERATION_FAIL;
	}
	no_action--;
	call_depth--;
	if(call_depth == 0)
		pduel->release_script_group();
	return OPERATION_SUCCESS;
}
int32 interpreter::call_card_function(card* pcard, char* f, uint32 param_count, uint32 ret_count) {
	if (param_count != params.size()) {
		sprintf(pduel->strbuffer, "\"CallCardFunction\": incorrect parameter count");
		handle_message(pduel, 1);
		params.clear();
		return OPERATION_FAIL;
	}
	card2value(current_state, pcard);
	lua_getfield(current_state, -1, f);
	if (!lua_isfunction(current_state, -1)) {
		sprintf(pduel->strbuffer, "\"CallCardFunction\": attempt to call an error function");
		handle_message(pduel, 1);
		lua_pop(current_state, 2);
		params.clear();
		return OPERATION_FAIL;
	}
	no_action++;
	call_depth++;
	lua_remove(current_state, -2);
	push_param(current_state);
	if (lua_pcall(current_state, param_count, ret_count, 0)) {
		sprintf(pduel->strbuffer, lua_tostring(current_state, -1));
		handle_message(pduel, 1);
		lua_pop(current_state, 1);
		no_action--;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return OPERATION_FAIL;
	}
	no_action--;
	call_depth--;
	if(call_depth == 0)
		pduel->release_script_group();
	return OPERATION_SUCCESS;
}
int32 interpreter::call_code_function(uint32 code, char* f, uint32 param_count, uint32 ret_count) {
	if (param_count != params.size()) {
		sprintf(pduel->strbuffer, "\"CallCodeFunction\": incorrect parameter count");
		handle_message(pduel, 1);
		params.clear();
		return OPERATION_FAIL;
	}
	load_card_script(code);
	lua_getfield(current_state, -1, f);
	if (!lua_isfunction(current_state, -1)) {
		sprintf(pduel->strbuffer, "\"CallCodeFunction\": attempt to call an error function");
		handle_message(pduel, 1);
		lua_pop(current_state, 2);
		params.clear();
		return OPERATION_FAIL;
	}
	lua_remove(current_state, -2);
	no_action++;
	call_depth++;
	push_param(current_state);
	if (lua_pcall(current_state, param_count, ret_count, 0)) {
		sprintf(pduel->strbuffer, lua_tostring(current_state, -1));
		handle_message(pduel, 1);
		lua_pop(current_state, 1);
		no_action--;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return OPERATION_FAIL;
	}
	no_action--;
	call_depth--;
	if(call_depth == 0)
		pduel->release_script_group();
	return OPERATION_SUCCESS;
}
int32 interpreter::check_condition(int32 f, uint32 param_count) {
	int32 result;
	if(!f) {
		params.clear();
		return TRUE;
	}
	no_action++;
	call_depth++;
	if (call_function(f, param_count, 1)) {
		result = lua_toboolean(current_state, -1);
		lua_pop(current_state, 1);
		no_action--;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return result;
	}
	no_action--;
	call_depth--;
	if(call_depth == 0)
		pduel->release_script_group();
	return OPERATION_FAIL;
}
int32 interpreter::check_matching(card* pcard, int32 findex, uint32 extraargs) {
	int32 result;
	if(!findex || lua_isnil(current_state, findex))
		return TRUE;
	no_action++;
	call_depth++;
	lua_pushvalue(current_state, findex);
	interpreter::card2value(current_state, pcard);
	for(uint32 i = 0; i < extraargs; ++i)
		lua_pushvalue(current_state, (int32)(-extraargs - 2));
	if (lua_pcall(current_state, 1 + extraargs, 1, 0)) {
		sprintf(pduel->strbuffer, lua_tostring(current_state, -1));
		handle_message(pduel, 1);
		lua_pop(current_state, 1);
		no_action--;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return OPERATION_FAIL;
	}
	result = lua_toboolean(current_state, -1);
	lua_pop(current_state, 1);
	no_action--;
	call_depth--;
	if(call_depth == 0)
		pduel->release_script_group();
	return result;
}
int32 interpreter::get_operation_value(card* pcard, int32 findex, uint32 extraargs) {
	int32 result;
	if(!findex || lua_isnil(current_state, findex))
		return 0;
	no_action++;
	call_depth++;
	lua_pushvalue(current_state, findex);
	interpreter::card2value(current_state, pcard);
	for(uint32 i = 0; i < extraargs; ++i)
		lua_pushvalue(current_state, (int32)(-extraargs - 2));
	if (lua_pcall(current_state, 1 + extraargs, 1, 0)) {
		sprintf(pduel->strbuffer, lua_tostring(current_state, -1));
		handle_message(pduel, 1);
		lua_pop(current_state, 1);
		no_action--;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return OPERATION_FAIL;
	}
	result = lua_tointeger(current_state, -1);
	lua_pop(current_state, 1);
	no_action--;
	call_depth--;
	if(call_depth == 0)
		pduel->release_script_group();
	return result;
}
int32 interpreter::get_function_value(int32 f, uint32 param_count) {
	int32 result;
	if(!f) {
		params.clear();
		return 0;
	}
	no_action++;
	call_depth++;
	if (call_function(f, param_count, 1)) {
		if (lua_isboolean(current_state, -1))
			result = lua_toboolean(current_state, -1);
		else
			result = lua_tointeger(current_state, -1);
		lua_pop(current_state, 1);
		no_action--;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return result;
	}
	no_action--;
	call_depth--;
	if(call_depth == 0)
		pduel->release_script_group();
	return OPERATION_FAIL;
}
int32 interpreter::call_coroutine(int32 f, uint32 param_count, uint32 * yield_value, uint16 step) {
	*yield_value = 0;
	if (!f) {
		sprintf(pduel->strbuffer, "\"CallCoroutine\": attempt to call a null function");
		handle_message(pduel, 1);
		params.clear();
		return OPERATION_FAIL;
	}
	if (param_count != params.size()) {
		sprintf(pduel->strbuffer, "\"CallCoroutine\": incorrect parameter count");
		handle_message(pduel, 1);
		params.clear();
		return OPERATION_FAIL;
	}
	coroutine_map::iterator it;
	it = coroutines.find(f);
	lua_State* rthread;
	if (it == coroutines.end()) {
		rthread = lua_newthread(lua_state);
		function2value(rthread, f);
		if(!lua_isfunction(rthread, -1)) {
			sprintf(pduel->strbuffer, "\"CallCoroutine\": attempt to call an error function");
			handle_message(pduel, 1);
			params.clear();
			return OPERATION_FAIL;
		}
		call_depth++;
		coroutines.insert(make_pair(f, rthread));
	} else {
		rthread = it->second;
		if(step == 0) {
			sprintf(pduel->strbuffer, "recursive event trigger detected.");
			handle_message(pduel, 1);
			params.clear();
			call_depth--;
			if(call_depth == 0)
				pduel->release_script_group();
			return OPERATION_FAIL;
		}
	}
	current_state = rthread;
	push_param(rthread);
	int32 result = lua_resume(rthread, param_count);
	if (result == 0) {
		coroutines.erase(f);
		if(yield_value)
			*yield_value = lua_isboolean(rthread, -1) ? lua_toboolean(rthread, -1) : lua_tointeger(rthread, -1);
		current_state = lua_state;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return COROUTINE_FINISH;
	} else if (result == LUA_YIELD) {
		return COROUTINE_YIELD;
	} else {
		coroutines.erase(f);
		sprintf(pduel->strbuffer, lua_tostring(rthread, -1));
		handle_message(pduel, 1);
		lua_pop(rthread, 1);
		current_state = lua_state;
		call_depth--;
		if(call_depth == 0)
			pduel->release_script_group();
		return COROUTINE_ERROR;
	}
}
//Convert a pointer to a lua value, use "p"+pointer value as the name, +1 -0
void interpreter::card2value(lua_State* L, card* pcard) {
	if (!pcard || pcard->ref_handle == 0)
		lua_pushnil(L);
	else
		lua_rawgeti(L, LUA_REGISTRYINDEX, pcard->ref_handle);
}
void interpreter::group2value(lua_State* L, group* pgroup) {
	if (!pgroup || pgroup->ref_handle == 0)
		lua_pushnil(L);
	else
		lua_rawgeti(L, LUA_REGISTRYINDEX, pgroup->ref_handle);
}
void interpreter::effect2value(lua_State* L, effect* peffect) {
	if (!peffect || peffect->ref_handle == 0)
		lua_pushnil(L);
	else
		lua_rawgeti(L, LUA_REGISTRYINDEX, peffect->ref_handle);
}
void interpreter::function2value(lua_State* L, int32 func_ref) {
	if (!func_ref)
		lua_pushnil(L);
	else
		lua_rawgeti(L, LUA_REGISTRYINDEX, func_ref);
}
int32 interpreter::get_function_handle(lua_State* L, int32 index) {
	lua_pushvalue(L, index);
	int32 ref = luaL_ref(L, LUA_REGISTRYINDEX);
	return ref;
}
void interpreter::set_duel_info(lua_State* L, duel* pduel) {
	lua_pushinteger(L, (ptr) pduel);
	lua_rawseti(L, LUA_GLOBALSINDEX, 1);
}
duel* interpreter::get_duel_info(lua_State * L) {
	lua_rawgeti(L, LUA_GLOBALSINDEX, 1);
	ptr pduel = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return (duel*) pduel;
}
