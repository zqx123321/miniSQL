#include "index.h"
#include "buffer.h"

Index* IndexManager;
extern Buffer* BufferManager;

set<Location> Index::Select(string tableName, int index, dataType type, string value) {
	set<Location> result;

	int i;
	for (i = 0; i < allIndices.size(); i++)
		if (allIndices.at(i).tableName == tableName
			&& allIndices.at(i).index == index)
			break;
	BPlus & tree = allIndices.at(i);
	NodeDef* C = tree.Root;
	while (C->isLeaf == false) {
		// Let i = smallest number sunch that V <=C.Ki
		int i, size = C->allKeys.size();
		string Key;
		for (i = size - 1; i >= 0; i--) {
			Key = (*C->allKeys.at(i)).value;
			if (Judge(value, Key, type, "<"))
				break;
		}
		// if not such i
		if (i == -1) {
			C = C->children.at(size);
		}
		else {
			C = C->children.at(i);
		}
	}
	// now C is the leaf it ought to belong to
	for (int i = 0; i < C->allKeys.size(); i++)
		if (Judge((*C->allKeys.at(i)).value, value, type, "="))
			break;

	if (i == C->allKeys.size()) // not found
		return result;
	else {
		Location temp((*C->allKeys.at(i)).loc);
		result.insert(temp);
		return result;
	}
}

Location Index::BuildBPlus(string name, TableDef & table, int index,
	vector<AidedNode> nodeList) {
	if (nodeList.size() == 0)
		throw "Empty table!";

	BPlus* tree = new BPlus(name, table.name, index,
		table.attrList.at(index).type);
	for (int i = 0; i < nodeList.size(); i++)
		tree->insertNode(nodeList.at(i));

	allIndices.push_back(*tree);

	string writeData = "# ";
	writeData += tree->Write();
	writeData += " *";
	
	BufferManager->WritePage(INDEX, 0, writeData.c_str(), strlen(writeData.c_str()), ADD);
	return (*tree->Root->allKeys.at(0)).loc;
}

bool Index::findBPlus(string name) {
	for (int i = 0; i < allIndices.size(); i++)
		if (allIndices.at(i).name == name)
			return true;
	return false;
}

indexInfo Index::dropBPlus(string name) {
	// erase record in abstract class
	string table;
	int infoIndex;
	int i;
	for (i = 0; i < allIndices.size(); i++)
		if (allIndices.at(i).name == name)
			break;
	table = allIndices.at(i).tableName;
	infoIndex = allIndices.at(i).index;
	allIndices.erase(allIndices.begin() + i);
	
	// erase record in page in memory
	int pageNum = BufferManager->pageList.size();
	for (int i = 0; i < pageNum; i++) {

		if (BufferManager->pageList.at(i).type != INDEX)
			continue;
		const char* data = BufferManager
			->readPage(INDEX, BufferManager->pageList.at(i).offset);

		int index = 0;
		while (data[index] == '#' || data[index] == '/') {
			if (data[index] == '/') {
				index++;
				while (data[index] != '/')
					index++;
				index++;
				continue;
			}

			int index2 = index + 2; // skip # and space
			string temp;
			while (data[index2] != ' ') {
				temp += data[index2];
				index2++;
			}
			if (temp == name) {
				char* writeData = new char[strlen(data)];
				memset(writeData, 0, strlen(data));
				strcpy(writeData, data);
				writeData[index] = '/';
				while (writeData[index] != '*')
					index++;
				writeData[index] = '/';
				BufferManager->WritePage(INDEX, BufferManager->pageList.at(i).offset,
					writeData, strlen(writeData), COVER);
				
				break;
			}
			while (data[index] != '*')
				index++;
			index++;
		}
	}
	
	return indexInfo(table, infoIndex);
}

void Index::LoadAllIndices() {
	int pageNum = BufferManager->pageList.size();

	for (int i = 0; i < pageNum; i++) {

		if (BufferManager->pageList.at(i).type != INDEX)
			continue;
		const char* searchAddr = BufferManager
			->readPage(INDEX, BufferManager->pageList.at(i).offset);
		int index = 0;
		while (searchAddr[index] == '#' || searchAddr[index] == '/') {
			if (searchAddr[index] == '/') {
				index++;
				while (searchAddr[index] != '/')
					index++;
				index++;
				continue;
			}
			string rawData;
			index++;
			while (*(searchAddr + index) != '*') {
				rawData += *(searchAddr + index);
				index++;
			}
			vector<string> splitData = split(rawData);

			char buffer[12];

			string name = splitData.at(0);
			string tableName = splitData.at(1);

			int columnIndex = atoi(splitData.at(2).c_str());
			int order = atoi(splitData.at(3).c_str());
			
			dataType type;
			if (splitData.at(4) == "int")
				type = INT;
			else if (splitData.at(4) == "char")
				type = CHAR;
			else if (splitData.at(4) == "float")
				type = FLOAT;

			BPlus* tree = new BPlus(name, tableName, columnIndex, type);
			int j = 5;
			vector<AidedNode*> nodeList;
			while (j != splitData.size() && splitData.at(j) == "`") {
				j++; // isLeaf
				j++; // isRoot
				j++; // fatherPage 
				j++; // fatherOffset
				j++;
				int keyNum = atoi(splitData.at(j).c_str());
				
				for (int k = 0; k < keyNum; k++) {
					j++;
					string value = splitData.at(j);
					j++;
					int page = atoi(splitData.at(j).c_str());
					j++;
					int offset = atoi(splitData.at(j).c_str());
					AidedNode * newNode = new AidedNode(value, Location(page, offset));
					nodeList.push_back(newNode);
					
				}
				j++; // "~"
				j++;
			}
			for (int i = 0; i < nodeList.size(); i++)
				tree->insertNode((*nodeList.at(i)));
			allIndices.push_back(*tree);
			index++;
		}
	}
}