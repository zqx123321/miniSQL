#pragma once
#include "minisql.h"

class Index {
public:
	set<Location> Select(int page, int offset,
		int columnIndex, string operation,
		string value);
private:

};

