#pragma once
#include <vector>
#include <string>
#include "minisql.h"
using namespace std;

class AttrDef {
public:
	AttrDef(string n, dataType d, int w, bool u);
private:
	string name;
	dataType type;
	int width;
	bool unique;
};

class TableDef {
	friend class Catalog;
public:
	TableDef(string n, int c);
	void addAttr(AttrDef a);
private:	
	string name;
	string primaryKey;
	int columnNum;
	vector<AttrDef> attrList;
};

class IndexDef {
public:
	IndexDef(string t, string a);
private:
	string tableName;
	string attrName;
};

class Catalog {
public:
	void CreateTable(vector<string> cmd);
	void CreateIndex(vector<string> cmd);
	bool FindTable(string temp) const;
private:
	vector<TableDef> allTables;
	vector<IndexDef> allIndexes;
};

void CATALOG_Initial();
