#pragma once
#include "minisql.h"

class Page {
private:
	unsigned int  FileID;	//	the file it belongs to
	unsigned long PageID;	//	its number
	bool lastUsed;			//	if it's used the last time
	void* head;				// Physical addr. for this page
	char* data;				// Physical data for this page
public:
	Page();
	~Page();
};

class File {
protected:
	unsigned int ID;
	string path;
	void *head;
public:
	File(unsigned int i, string p);
	virtual ~File();
};

class Buffer {
public:
	Buffer(string c, string r, string i);
private:
	string catalogPath;
	string recordPath;
	string indexPath;

};

