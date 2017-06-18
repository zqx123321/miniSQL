#include "catalog.h"
#include "buffer.h"

const int PAGE_SIZE = 4092;

Catalog* CatalogManager;
extern Buffer* BufferManager;

void Catalog::CreateTable(TableDef & table) {

	allTables.push_back(table);

	string data;
	data += "# ";
	
	data += table.name;
	data += ' ';

	data += table.primaryKey;
	data += ' ';

	char buffer[12];
	sprintf(buffer, "%d", table.columnNum);
	data += buffer;
	data += ' ';

	for each (AttrDef attr in table.attrList) {
		data += attr.name;
		data += ' ';
		switch (attr.type) {
		case INT: 
			data += "int"; data += ' ';		break;
		case CHAR:
			data += "char"; data += ' ';	break;
		case FLOAT:
			data += "float"; data += ' ';	break;
		}
		
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d", attr.width);
		data += buffer; data += ' ';

		memset(buffer, 0, sizeof(buffer));
		if (attr.unique == true) {
			sprintf(buffer, "%d", 1);
			data += buffer;
			data += ' ';
		}
		else {
			sprintf(buffer, "%d", 0);
			data += buffer;
			data += ' ';
		}

		memset(buffer, 0, sizeof(buffer));
		if (attr.hasIndex == true) {
			sprintf(buffer, "%d", 1);
			data += buffer;
			data += ' ';
		}
		else {
			sprintf(buffer, "%d", 0);
			data += buffer;
			data += ' ';
		}

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d", attr.indexPage);
		data += buffer;
		data += ' ';

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d", attr.indexOffset);
		data += buffer;
		data += ' ';
	}

	data += '*';
	int size = data.size();
	BufferManager->WritePage(CATALOG, 0, data.c_str(), size, ADD);
}

bool Catalog::FindTable(string name) const{
	bool found = false;
	for (int i = 0; i < allTables.size(); i++)
		if (allTables.at(i).name == name) {
			found = true;
			break;
		}
	return found;
}

TableDef & Catalog::FetchTable(string name) {
	int i;
	for (i = 0; i < allTables.size(); i++)
		if (allTables.at(i).name == name)
			break;
	return allTables.at(i);
}


void Catalog::LoadAllTables() {
	int pageNum = BufferManager->pageList.size();

	for (int i = 0; i < pageNum; i++) {

		if (BufferManager->pageList.at(i).type != CATALOG)
			continue;
		const char* searchAddr = BufferManager
			->readPage(CATALOG, BufferManager->pageList.at(i).offset);

		int index = 0;
		while (*(searchAddr + index) == '#') {
			string rawData;
			index++;
			while (*(searchAddr + index) != '*') {
				rawData += *(searchAddr + index);
				index++;
			}
			vector<string> splitData = split(rawData);

			string tableName = splitData.at(0);
			string primaryKey = splitData.at(1);
			TableDef newTable(tableName, 0);

			int columnNum;
			sscanf(splitData.at(2).c_str(), "%d", &columnNum);
			int iter = 3;
			for (int j = 0; j < columnNum; j++) {
				string attrName;
				dataType attrType;
				int attrWidth, indexPage, indexOffset;
				bool isUnique, hasIndex;

				attrName = splitData.at(iter++);
				string type = splitData.at(iter++);
				if (type == "int")
					attrType = INT;
				else if (type == "char")
					attrType = CHAR;
				else if (type == "float")
					attrType = FLOAT;

				string width = splitData.at(iter++);
				attrWidth = atoi(width.c_str());

				string unique = splitData.at(iter++);
				if (unique == "1")
					isUnique = true;
				else
					isUnique = false;

				string index = splitData.at(iter++);
				if (index == "1")
					hasIndex = true;
				else
					hasIndex = false;

				string page = splitData.at(iter++);
				indexPage = atoi(page.c_str());

				string offset = splitData.at(iter++);
				indexOffset = atoi(offset.c_str());

				AttrDef newAttr(attrName, attrType, attrWidth, isUnique);
				newAttr.hasIndex = hasIndex;
				newAttr.indexPage = indexPage;
				newAttr.indexOffset = indexOffset;
				newTable.attrList.push_back(newAttr);
			}

			allTables.push_back(newTable);
			index++;
		}
	}
}

/*bool Catalog::FindTable(string temp) const {
	
	
}*/