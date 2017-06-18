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

void API_CreateTable(TableDef & table);
void API_CreateIndex(vector<string> cmd);
bool API_FindTable(string name);
int API_Insert(RecordDef & record);
int API_Select(Query & query);


int API_FindColumnIndex(string table, string attrbute);
dataType API_FindColumnType(string table, string attrbute);
