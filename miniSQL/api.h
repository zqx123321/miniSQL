#pragma once
#include "minisql.h"

enum opType {
	CREATE,
	SELECT,
	INSERT,
	DELETE,
	DROP,
	EXECFILE,
	QUIT,
};

void API_CreateTable(TableDef & table);
void API_CreateIndex(vector<string> cmd);
bool API_FindTable(string name);
int API_Insert(RecordDef & record);
int API_Select(Query & query);
int API_Delete(Query & query);
int API_Drop(string name);

int API_FindColumnIndex(string table, string attrbute);
dataType API_FindColumnType(string table, string attrbute);
