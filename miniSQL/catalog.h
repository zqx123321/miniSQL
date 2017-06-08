#pragma once
#include <vector>
#include <string>
#include "minisql.h"
using namespace std;

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

