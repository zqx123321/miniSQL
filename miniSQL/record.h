#pragma once
#include "minisql.h"

class Record {
public:
	int Insert(RecordDef & record, int columnNum);
	set<Location> Select(string table, 
		int attrIndex, dataType type,
		string operation, string value);
	set<Location> Select(string table);
	void showRecord(int attrNum, set<Location> loc);
	void deleteRecord(set<Location> loc);
};
 