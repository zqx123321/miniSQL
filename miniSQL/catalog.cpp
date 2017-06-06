#include "catalog.h"

Catalog* CatalogManager;

AttrDef::AttrDef(string n, dataType t, int w, bool u) : 
	name(n), type(t), width(w), unique(u) {
}

TableDef::TableDef(string n, int c) : name(n), columnNum(c) {}

void TableDef::addAttr(AttrDef a) {
	attrList.push_back(a);
}

IndexDef::IndexDef(string t, string a) : tableName(t), attrName(a) {}

void Catalog::CreateTable(vector<string> cmd) {
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

void Catalog::CreateIndex(vector<string> cmd) {
	
}

bool Catalog::FindTable(string temp) const {
	vector<TableDef>::const_iterator iter;
	iter = allTables.begin();
	while (iter != allTables.end()) {
		if (iter->name == temp)
			return true;
		iter++;
	}
	return false;
}