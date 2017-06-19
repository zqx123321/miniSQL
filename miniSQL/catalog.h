#pragma once
#include "minisql.h"
using namespace std;

class Catalog {
public:
	void CreateTable(TableDef & table, int mode);
	void CreateIndex(TableDef & table, int index, Location root);
	bool FindTable(string name) const;
	TableDef & FetchTable(string name);
	void dropTable(string name, int mode);
	void dropIndex(indexInfo info);
	void LoadAllTables();

	bool FindIndex(string table, int colNum);
	int FindColumnNum(string table, string attribute);
	dataType FindColumnType(string table, string attribute);
	int showHeader(Query & query);
private:
	vector<TableDef> allTables;
};

