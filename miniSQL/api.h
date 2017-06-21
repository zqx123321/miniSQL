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
int API_CreateIndex(string indexName, TableDef & table, int index);
TableDef & API_FetchTable(string table);
bool API_FindTable(string name);
bool API_FindIndex(string name);
int API_Insert(RecordDef & record);
int API_Select(Query & query);
int API_Delete(Query & query);
int API_Drop(string name);
int API_DropIndex(string name);
int API_FindColumnIndex(string table, string attrbute);
dataType API_FindColumnType(string table, string attrbute);
