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

void API_Main(opType type, vector<string> command);
void API_CreateTable(vector<string> command);
void API_CreateIndex(vector<string> command);
void API_Select(vector<string> command);
void API_Insert(vector<string> command);
void API_Update(vector<string> command);
void API_Delete(vector<string> command);
void API_Drop(vector<string> command);
void API_Execfile(vector<string> command);