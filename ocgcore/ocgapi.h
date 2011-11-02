/*
 * interface.h
 *
 *  Created on: 2010-4-28
 *      Author: Argon
 */

#ifndef OCGAPI_H_
#define OCGAPI_H_

#include "common.h"
#include "card.h"
#include "group.h"
#include "effect.h"
#include "field.h"
#include "interpreter.h"
#include <windows.h>

class card;
struct card_data;
struct card_info;
class group;
class effect;
class interpreter;
struct event;

typedef byte* (*script_reader)(const char*, int*);
typedef uint32 (*card_reader)(uint32, card_data*);
typedef uint32 (*message_handler)(void*, uint32);

extern "C" _declspec(dllexport) void set_script_reader(script_reader f);
extern "C" _declspec(dllexport) void set_card_reader(card_reader f);
extern "C" _declspec(dllexport) void set_message_handler(message_handler f);

byte* read_script(const char* script_name, int* len);
uint32 read_card(uint32 code, card_data* data);
uint32 handle_message(void* pduel, uint32 message_type);

extern "C" _declspec(dllexport) ptr create_duel(uint32 seed);
extern "C" _declspec(dllexport) void start_duel(ptr pduel, int32 options);
extern "C" _declspec(dllexport) void end_duel(ptr pduel);
extern "C" _declspec(dllexport) void set_player_info(ptr pduel, int32 playerid, int32 lp, int32 startcount, int32 drawcount);
extern "C" _declspec(dllexport) void get_log_message(ptr pduel, byte* buf);
extern "C" _declspec(dllexport) int32 get_message(ptr pduel, byte* buf);
extern "C" _declspec(dllexport) int32 process(ptr pduel);
extern "C" _declspec(dllexport) void new_card(ptr pduel, uint32 code, uint8 owner, uint8 playerid, uint8 location, uint8 sequence, uint8 position);
extern "C" _declspec(dllexport) int32 query_card(ptr pduel, uint8 playerid, uint8 location, uint8 sequence, int32 query_flag, byte* buf);
extern "C" _declspec(dllexport) int32 query_field_count(ptr pduel, uint8 playerid, uint8 location);
extern "C" _declspec(dllexport) int32 query_field_card(ptr pduel, uint8 playerid, uint8 location, int32 query_flag, byte* buf);
extern "C" _declspec(dllexport) void set_responsei(ptr pduel, int32 value);
extern "C" _declspec(dllexport) void set_responseb(ptr pduel, byte* buf);
extern "C" _declspec(dllexport) void run_script(ptr pduel, byte* scriptbuf, int32 len);
byte* default_script_reader(const char* script_name, int* len);
uint32 default_card_reader(uint32 code, card_data* data);
uint32 default_message_handler(void* pduel, uint32 msg_type);

#endif /* OCGAPI_H_ */
