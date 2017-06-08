#include "buffer.h"

Buffer* BufferManager;
const int PAGE_SIZE = 4096;
const int PAGE_MAX_NUM = 500;

Buffer::Buffer()
{
	filePath.push_back("../catalog/0.catalog");
	filePath.push_back("../record/0.record");
	filePath.push_back("../index/0.index");
}

const char* Buffer::readFirstPage(fileType type) {
	//first see if it already exists
	int i = 0;
	while (i < pageList.size()) {
		if (pageList.at(i).type == type &&
			pageList.at(i).offset == 0);
		break;
	}
	if (i == pageList.size()) {
		return addPage(type, 0);
	}
	else {
		return pageList.at(i).head;
	}
}

char* Buffer::addPage(fileType type, int offset) {
	Page newPage(type, offset);
	pageList.push_back(newPage);
	return newPage.head;
}

Page::Page(fileType t, int o) : type(t), offset(o){
	// settings
	lastUsed = false;
	pin = false;
	dirty = false;
	head = new char[PAGE_SIZE];

	// import the data from file
	ifstream fin;
	fin.open(BufferManager->filePath.at(t), ios::in | ios::binary);
	fin.seekg(offset*PAGE_SIZE, ios::end);
	fin.read(head, PAGE_SIZE);
}

Page::~Page() {
	delete [] head;
}
