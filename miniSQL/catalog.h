#pragma once
#include "minisql.h"
using namespace std;

class Catalog {
public:
	void CreateTable(TableDef & table);

	bool FindTable(string name) const;
	TableDef & FetchTable(string name);

	void LoadAllTables();

	void FindColumnIndex(string table, int colNum, int & page, int & offset);
	int FindColumnNum(string table, string attribute);
	dataType FindColumnType(string table, string attribute);
	int showHeader(Query & query);
private:
	vector<TableDef> allTables;
};

