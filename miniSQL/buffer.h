#pragma once
#include "minisql.h"

struct Page {
	fileType type;	//	the file it belongs to
	int offset;		//  page offset
	bool pin;		//  if it's pinned
	bool lastUsed;	//	if it's used the last time
	bool dirty;		//  if it's consistent with the disk
	unsigned int dataSize;
	//  physical a ddr. for this page
	char* data;

	Page(fileType t, int o);
	Page(const Page & orig);
	~Page();
};


struct File {
	File(int c = 0, int r = 0, int i = 0);
	int blockNum[3];
	vector<int> catalogExistIndex;
	vector<int> recordExistIndex;
	vector<int> indexExistIndex;
	vector<string> filePath;
	void eraseIndex(fileType type, int offset);
};

struct Buffer {
	Buffer();
	~Buffer();
	void loadAllPages();
	void storePage(fileType type);
	const char* readPage(fileType type, int offset); // read from buffer
	bool WritePage(fileType type, int offset,
		const char* source, int size, writeMode mode);
	Page& addPage(fileType type, int offset);
	vector<Page> pageList;
	File* fileInfo;
};

