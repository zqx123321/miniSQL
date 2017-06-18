#include "record.h"
#include "buffer.h"

Record *RecordManager;
extern Buffer* BufferManager;

int Record::Insert(RecordDef & record, int columnNum) {
	
	string data;
	
	int size = record.value.size();
	int iter = 0;
	int count = 0;
	while (iter != size) {
		data += '#';
		data += ' ';
		data += record.tableName;
		data += ' ';
		for (int i = 0; i < columnNum; i++) {
			data += record.value.at(iter);
			data += ' ';
			iter++;
		}
		data += '*';
		count++;
	}

	int dataSize = data.size();
	BufferManager->WritePage(RECORD, 0, data.c_str(), dataSize, ADD);
	return count;
}

void Record::showRecord(int attrNum, set<Location> loc) {
	set<Location>::iterator iter;
	for (iter = loc.begin(); iter != loc.end(); iter++){
		cout << endl;

		const char* data = BufferManager->readPage
			(RECORD, BufferManager->pageList.at(iter->page).offset);

		int index = iter->offset;
		string temp;

		index += 2; // skip * and space
		while (data[index] != ' ') {	// skip table name
			index++;
		}
		index++;

		for (int j = 0; j < attrNum; j++) {
			temp.clear();
			while (data[index] != ' ') {
				temp += data[index];
				index++;
			}
			cout << left << setw(12) << temp;
			if (j != attrNum - 1)
				cout << '|';
			index++;
		}
	}
}

set<Location> Record::Select(string table,
	int attrIndex, dataType type,
	string operation, string value) {
	set<Location> result;

	int pageNum = BufferManager->pageList.size();
	for (int i = 0; i < pageNum; i++) {

		if (BufferManager->pageList.at(i).type != RECORD)
			continue;
		const char* data = BufferManager
			->readPage(RECORD, BufferManager->pageList.at(i).offset);

		int index = 0;
		while (data[index] == '#') {
			int index2 = index + 2; // skip # and space
			string temp;
			while (data[index2] != ' ') {
				temp += data[index2];
				index2++;
			}
			index2++;

			if (temp == table) {
				for (int j = 0; j <= attrIndex; j++) {
					temp.clear();
					while (data[index2] != ' ') {
						temp += data[index2];
						index2++;
					}
					index2++;
				}
				
				if (Judge(temp, value, type, operation) == true) {
					int page = i;
					int offset = index;
					result.insert(Location(page, offset));
				}
			}
			while (data[index] != '*')
				index++;
			index++;
		}
	}
	return result;
}

set<Location> Record::Select(string table) {
	set<Location> result;

	int pageNum = BufferManager->pageList.size();
	for (int i = 0; i < pageNum; i++) {

		if (BufferManager->pageList.at(i).type != RECORD)
			continue;
		const char* data = BufferManager
			->readPage(RECORD, BufferManager->pageList.at(i).offset);

		int index = 0;
		while (data[index] == '#') {
			int index2 = index + 2; // skip # and space
			string temp;
			while (data[index2] != ' ') {
				temp += data[index2];
				index2++;
			}
			if (temp == table) {
				int page = i;
				int offset = index;
				result.insert(Location(page, offset));
			}
			while (data[index] != '*')
				index++;
			index++;
		}
	}
	return result;
}

void Record::deleteRecord(set<Location> loc) {

}

