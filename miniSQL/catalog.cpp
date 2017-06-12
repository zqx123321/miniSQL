#include "catalog.h"
#include "buffer.h"

const int PAGE_SIZE = 4096;

Catalog* CatalogManager;
extern Buffer* BufferManager;

AttrDef::AttrDef(string n, dataType t, int w, bool u, bool k) : 
	name(n), type(t), width(w), unique(u), isKey(k) {
}

TableDef::TableDef(string n, int c) : name(n), columnNum(c) {}

IndexDef::IndexDef(string t, string a) : tableName(t), attrName(a) {}

/*
test command:
create table jzf(name char(8) unique, age int, salary float, primary key (name));
*/

void Catalog::CreateTable(vector<string> cmd) {
	for (int i = 0; i < cmd.size(); i++)
		cout << cmd.at(i) << ' ';
	cout << endl;

	string data;
	data += "# ";
	for (int i = 0; i < cmd.size(); i++)
		data += cmd.at(i) + ' ';
	data += "*";

	int size = data.size();
	BufferManager->WritePage(CATALOG, 0, data.c_str(), size, ADD);
}

/*
void Catalog::CreateTable(TableDef table) { 
	


	
	int count = 0;
	string priKey;
	vector<string>::const_iterator iter = cmd.begin() + 1; 
										// The first one is table name
	vector<AttrDef> attrList;
	while (iter != cmd.end()) {
		string name = *iter;
		if (*iter == "primary") {
			++iter;
			priKey = *iter;
			++iter;
			continue;
		}
		iter++;

		dataType type;
		if (*iter == "int")
			type = INT;
		else if (*iter == "float")
			type = FLOAT;
		else if (*iter == "char")
			type = CHAR;
		iter++;
		
	
		string temp = *iter;
		int width = atoi(temp.c_str());
		iter++;
		temp = *iter;
		bool unique = atoi(temp.c_str()) != 0;
		iter++;

		AttrDef a(name, type, width, unique);	
		attrList.push_back(a);
		count++;

	}

	TableDef t(cmd[0], count);
	t.primaryKey = priKey;
	allTables.push_back(t);
	cout << "Create successfully!";
	cout << count << " row(s) affected." << endl;
}
*/

void Catalog::CreateIndex(vector<string> cmd) {
	
}

bool Catalog::FindTable(string temp) const {
	
	int pageNum = BufferManager->pageList.size();

	for (int i = 0; i < pageNum; i++) {

		if (BufferManager->pageList.at(i).type != CATALOG)
			continue;
		const char* searchAddr = BufferManager
			->readPage(CATALOG, BufferManager->pageList.at(i).offset);

		int index = 0;
		while (*(searchAddr + index) == '#') {
			index+=2;
			string name;
			while (*(searchAddr + index) != ' ') {
				name += *(searchAddr + index);
				index++;
			}
			if (name == temp)
				return true;
			while (*(searchAddr + index) != '*')
				index++;-

			index++;
		}
	}
	
	return false;
}