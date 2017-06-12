#pragma once
#include "minisql.h"

enum opType {
	CREATE,
	SELECT,
	INSERT,
	UPDATE,
	DELETE,
	DROP,
	EXECFILE,
	QUIT,
	CREATE_TABLE,
	CREATE_INDEX
};

void API_CreateTable(vector<string> cmd);
void API_CreateIndex(vector<string> cmd);
bool API_FindTable(string name);
