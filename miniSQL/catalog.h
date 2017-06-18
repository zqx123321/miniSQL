#pragma once
#include "minisql.h"
using namespace std;

class Catalog {
public:
	void CreateTable(TableDef & table);


	bool FindTable(string name) const;
	TableDef & FetchTable(string name);
	void LoadAllTables();
private:
	vector<TableDef> allTables;
};

