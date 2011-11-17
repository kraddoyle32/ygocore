#ifndef REPLAY_H
#define REPLAY_H

#include "config.h"
#include <sys/time.h>

namespace ygo {

#define REPLAY_COMPRESSED	0x1

struct ReplayHeader {
	unsigned int id;
	unsigned int version;
	wchar_t player1[20];
	wchar_t player2[20];
	unsigned int flag;
	unsigned int seed;
	unsigned int startlp;
	unsigned int starthand;
	unsigned int drawcount;
	unsigned int option;
	unsigned int datasize;
	unsigned int hash;
	unsigned char props[8];
};

class Replay {
public:
	Replay();
	~Replay();
	void BeginRecord();
	void WriteHeader(ReplayHeader& header);
	void WriteData(const void* data, unsigned int length, bool flush = true);
	void WriteInt32(int data, bool flush = true);
	void WriteInt16(short data, bool flush = true);
	void WriteInt8(char data, bool flush = true);
	void Flush();
	void EndRecord();
	void SaveReplay(const wchar_t* name);
	bool OpenReplay(const wchar_t* name);
	bool ReadNextResponse();
	void ReadHeader(ReplayHeader& header);
	void ReadData(void* data, unsigned int length);
	int ReadInt32();
	short ReadInt16();
	char ReadInt8();

	FILE* fp;
	ReplayHeader pheader;
	unsigned char* replay_data;
	unsigned char* comp_data;
	unsigned char* pdata;
	size_t replay_size;
	size_t comp_size;
	bool is_recording;
	bool is_replaying;
};

}

#endif
