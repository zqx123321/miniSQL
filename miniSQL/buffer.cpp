#include "buffer.h"

Buffer* BufferManager;
const int PAGE_SIZE = 4092;
const int PAGE_MAX_NUM = 30;

Buffer::Buffer()
{
	fileInfo = new File();
}

Buffer::~Buffer() {

	BufferManager->storePage(CATALOG);
	BufferManager->storePage(RECORD);
	BufferManager->storePage(INDEX);
}

void Buffer::loadAllPages() {
	ifstream fin;
	for (int i = 0; i < 3; i++) {
		fin.open(fileInfo->filePath.at(i), ios::in | ios::binary);
		fin.seekg(0);
		fin.read((char*)&fileInfo->blockNum[i], sizeof(int));
		fin.close();
		for (int j = 0; j < fileInfo->blockNum[i]; j++) {
			if (i == CATALOG) {
				
				addPage(CATALOG, j);
				fileInfo->catalogExistIndex.push_back(j);
			}
			else if(i == RECORD){
				addPage(RECORD, j);
				fileInfo->recordExistIndex.push_back(j);
				
			}
			else if (i == INDEX) {
				addPage(INDEX, j);
				fileInfo->indexExistIndex.push_back(j);
			}
		}
		fin.close();
	}
	
}
	
void Buffer::storePage(fileType type) {

	ofstream fout;
	for (unsigned int i = 0; i < pageList.size(); i++) {
		if (pageList.at(i).type != type)
			continue;

		if (pageList.at(i).dirty == true) {
			fout.open(BufferManager->fileInfo->filePath.at((int)type), ios::in | ios::out | ios::binary);
			fout.seekp(pageList.at(i).offset * PAGE_SIZE + 4);
			fout.write((char*)&pageList.at(i).dataSize, sizeof(unsigned int));
			fout.write(pageList.at(i).data, PAGE_SIZE);
			pageList.at(i).dirty = false;
			fout.close();


		}
	}
}

const char* Buffer::readPage(fileType type, int offset) {
	//first see if it already exists
	int i = 0;
	int pageNum = pageList.size();
	while (i < pageNum) {
		if (pageList.at(i).type == type &&
			pageList.at(i).offset == offset)
		break;
		i++;
	}
	if (i == pageList.size()) {
		return addPage(type, offset).data;
	}
	else {
		return pageList.at(i).data;
	}
}

bool Buffer::WritePage(fileType type, int offset, 
	const char* source, int size, writeMode mode) {
	ofstream fout;
	int i = 0;
	int pageNum = pageList.size();
	while (i < pageNum) {
		if (pageList.at(i).type == type &&
			pageList.at(i).offset == offset)
		break;
		i++;
	}
	// if not found in the memory
	if (i == pageList.size()) {
		Page& current = addPage(type, offset);
		if (mode == COVER) {
			memcpy(current.data, source, size);
			current.dataSize = size;
		}
		else if (mode == ADD) {
			memcpy(current.data + current.dataSize, source, size);
			current.dataSize += size;
		}
		current.dirty = true;
	}

	else {
		// check if it's dirty
		if (pageList.at(i).dirty == true) {
			fout.open(BufferManager->fileInfo->filePath.at(type), ios::in | ios::out | ios::binary);
			fout.seekp(offset*PAGE_SIZE + 4);
			fout.write((char*)&pageList.at(i).dataSize,
				sizeof(unsigned int));

			fout.write(pageList.at(i).data, PAGE_SIZE);
			fout.close();
			pageList.at(i).dirty = false;
		}

		Page& current = pageList.at(i);

		if (mode == COVER) {
			memcpy(current.data, source, size);
			current.dataSize = size;
		}
		else if (mode == ADD) {
			memcpy(current.data + current.dataSize, source, size);
			current.dataSize += size;
		}
		
		current.dirty = true;
	}

	return true;
}

Page& Buffer::addPage(fileType type, int offset) {
	
	// realocation, FIFO
	if(pageList.size() == PAGE_MAX_NUM) {
		int i;
		for (i = 0; i < PAGE_MAX_NUM; i++)
			if (pageList.at(i).pin == false)
				break;
		if (i == PAGE_MAX_NUM) {
			throw "All pages are pinned! Cannot allocate new page!";
		}

		if (pageList.at(i).dirty == true) {
			fstream fout;
			fout.seekp(offset*PAGE_SIZE + 4);
			fout.write((char*)pageList.at(i).dataSize, sizeof(unsigned int));
			fout.write(pageList.at(i).data, PAGE_SIZE);
			fout.close();
			pageList.at(i).dirty = false;
		}
		fileInfo->eraseIndex(pageList.at(i).type, pageList.at(i).offset);
		pageList.erase(pageList.begin() + i);
	}
	
	pageList.push_back(Page(type, offset));
	return pageList.back();
}

Page::Page(fileType t, int o) : type(t), offset(o) {

	// settings
	lastUsed = false;
	pin = false;
	dirty = false;
	data = new char[PAGE_SIZE];
	memset(data, 0, PAGE_SIZE);

	// import the data from file
	ifstream fin;
	fin.open(BufferManager->fileInfo->filePath.at(t), ios::in | ios::binary);
	
	fin.seekg(offset*PAGE_SIZE + 4);
	fin.read((char*)&dataSize, sizeof(unsigned int));
	fin.read(data, dataSize);
}

Page::Page(const Page & orig) {
	type = orig.type;
	offset = orig.offset;
	pin = orig.pin;
	lastUsed = orig.lastUsed;
	dirty = orig.dirty;
	dataSize = orig.dataSize;
	data = new char[PAGE_SIZE];
	memset(data, 0, PAGE_SIZE);

	unsigned int i = 0;
	while (i < dataSize)
		data[i++] = orig.data[i];
}

Page::~Page() {
	delete [] data;
}

File::File(int c, int r, int i) {
	blockNum[CATALOG] = c;
	blockNum[RECORD] = r;
	blockNum[INDEX] = i;
	filePath.push_back("../catalog/0.catalog");
	filePath.push_back("../record/0.record");
	filePath.push_back("../index/0.index");
}

void File::eraseIndex(fileType type, int offset) {
	unsigned int j;
	switch (type) {
	case CATALOG:
		for (j = 0; j < catalogExistIndex.size(); j++)
			if (catalogExistIndex.at(j) == offset)
				break;
		catalogExistIndex.erase(catalogExistIndex.begin() + j);
		break;

	case RECORD:
		for (j = 0; j < recordExistIndex.size(); j++)
			if (recordExistIndex.at(j) == offset)
				break;
		recordExistIndex.erase(recordExistIndex.begin() + j);
		break;
	case INDEX:
		for (j = 0; j < indexExistIndex.size(); j++)
			if (indexExistIndex.at(j) ==offset)
				break;
		indexExistIndex.erase(indexExistIndex.begin() + j);
		break;
	}
}