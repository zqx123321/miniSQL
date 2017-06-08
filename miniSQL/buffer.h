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
	char* head;		
	//  physical a ddr. for this page
public:
	Page(fileType t, int o);
	~Page();
};

class Buffer {
	friend class Page;
public:
	Buffer();
	const char* readFirstPage(fileType type); // read from buffer
	char* addPage(fileType type, int offset);
	
private:
	vector<string> filePath;
	vector<Page> pageList;
};

