#include "api.h"
#include "catalog.h"

extern Catalog* CatalogManager;

void API_CreateTable(vector<string> cmd) {
	CatalogManager->CreateTable(cmd);
	CatalogManager->CreateIndex(cmd);
}

bool API_FindTable(string name) {
	return CatalogManager->FindTable(name);
}