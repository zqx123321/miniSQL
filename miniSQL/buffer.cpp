#include "buffer.h"

Buffer* BufferManager;
const int PAGE_SIZE = 4096;
const int PAGE_MAX_NUM = 30;

Buffer::Buffer()
{
	filePath.push_back("../catalog/0.catalog");
	filePath.push_back("../record/0.record");
	filePath.push_back("../index/0.index");
}

Buffer::~Buffer() {
	ofstream fout;

	int i = 0;
	int pageNum = getPageNum();
	for (; i < pageNum; i++) {
		if (pageList.at(i).dirty == true) {
			fileType type = pageList.at(i).type;
			fout.open(BufferManager->filePath.at(type), ios::out | ios::binary);
			fout.write(pageList.at(i).head, PAGE_SIZE);
			pageList.at(i).dirty = false;
		}
	}

}

const char* Buffer::readPage(fileType type, int offset) {
	//first see if it already exists
	int i = 0;
	int pageNum = getPageNum();
	while (i < pageNum) {
		if (pageList.at(i).type == type &&
			pageList.at(i).offset == offset);
		break;
	}
	if (i == pageList.size()) {
		return addPage(type, offset).head;
	}
	else {
		return pageList.at(i).head;
	}
}

bool Buffer::WritePage(fileType type, int offset, const char* source, int size) {
	ofstream fout;

	int i = 0;
	int pageNum = getPageNum();
	while (i < pageNum) {
		if (pageList.at(i).type == type &&
			pageList.at(i).offset == offset);
		break;
	}

	if (i == pageList.size()) {
		Page& current = addPage(type, offset);
		memcpy(current.head, source, size);
		current.dirty = true;
	}
	else {
		fout.open(BufferManager->filePath.at(type), ios::out | ios::binary);
		// check if it's dirty
		if (pageList.at(i).dirty == true) {
			fout.seekp(offset*PAGE_SIZE, ios::end);
			fout.write(pageList.at(i).head, PAGE_SIZE);
			pageList.at(i).dirty = false;
		}

		Page& current = pageList.at(i);
		memcpy(current.head, source, size);
		current.dirty = true;
	}
	
	return true;
}

Page& Buffer::addPage(fileType type, int offset) {
	
	// realocation, FIFO
	if(getPageNum() == PAGE_MAX_NUM) {
		int i;
		for (i = 0; i < PAGE_MAX_NUM; i++)
			if (pageList.at(i).pin == false)
				break;
		if (i == PAGE_MAX_NUM) {
			throw "All pages are pinned! Cannot allocate new page!";
		}
		if (pageList.at(i).dirty == true) {
			fstream fout;
			fout.seekp(offset*PAGE_SIZE, ios::end);
			fout.write(pageList.at(i).head, PAGE_SIZE);
			pageList.at(i).dirty = false;
		}
		pageList.erase(pageList.begin() + i);
	}
	
	Page newPage(type, offset);
	pageList.push_back(newPage);
	return pageList.back();
}

Page::Page(fileType t, int o) : type(t), offset(o){
	// settings
	lastUsed = false;
	pin = false;
	dirty = false;
	head = new char[PAGE_SIZE];
	head[0] = '\0';
	// import the data from file
	ifstream fin;
	fin.open(BufferManager->filePath.at(t), ios::in | ios::binary);
	fin.seekg(offset*PAGE_SIZE, ios::end);
	fin.read(head, PAGE_SIZE);
}

Page::Page(const Page & orig) {
	type = orig.type;
	offset = orig.offset;
	pin = orig.pin;
	lastUsed = orig.lastUsed;
	dirty = orig.dirty;
	head = new char[PAGE_SIZE];
	int i = 0;
	while (orig.head[i] != '\0')
		head[i++] = orig.head[i];
	head[i] = '\0';
}

Page::~Page() {
	delete [] head;
}
