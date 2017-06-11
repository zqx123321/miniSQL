#pragma once
#include "minisql.h"

class Page {
	friend class Buffer;
private:
	fileType type;	//	the file it belongs to
	int offset;		//  page offset
	bool pin;		//  if it's pinned
	bool lastUsed;	//	if it's used the last time
	bool dirty;		//  if it's consistent with the disk
	unsigned int dataSize;
	//  physical a ddr. for this page
	char* data;
public:
	Page(fileType t, int o);
	Page(const Page & orig);
	~Page();
};

class Buffer {
	friend class Page;
public:
	Buffer();
	~Buffer();
	int getPageNum() { return pageList.size(); }
	const char* readPage(fileType type, int offset); // read from buffer
	bool WritePage(fileType type, int offset,
		const char* source, int size, writeMode mode);
	Page& addPage(fileType type, int offset);
private:
	vector<string> filePath;
	vector<Page> pageList;
};

