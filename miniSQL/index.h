#pragma once
#include "minisql.h"

class Index {
public:
	void LoadAllIndices();
	bool findBPlus(string name);
	indexInfo dropBPlus(string name);
	set<Location> Select(string tableName, int index,
		dataType type, string value);
	Location BuildBPlus(string name, TableDef & table, int index,
			vector<AidedNode> nodeList);
private:
	vector<BPlus> allIndices;
};

