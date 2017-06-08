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