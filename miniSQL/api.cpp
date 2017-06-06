#include "api.h"
#include "catalog.h"

void API_Main(opType type, vector<string> command) {
	switch (type) {
	case CREATE_TABLE:
		API_CreateTable(command); break;
	case CREATE_INDEX:
		API_CreateIndex(command); break;
	default:
		break;
	}
}


void API_CreateTable(vector<string> command) {
	CatalogManager->CreateTable(command);

}

void API_CreateIndex(vector<string> command) {

}

void API_Select(vector<string> command) {

}

void API_Insert(vector<string> command) {

}

void API_Update(vector<string> command) {

}

void API_Delete(vector<string> command) {

}

void API_Drop(vector<string> command) {

}

void API_Execfile(vector<string> command) {

}