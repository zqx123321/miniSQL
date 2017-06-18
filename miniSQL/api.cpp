#include "api.h"
#include "catalog.h"

extern Catalog* CatalogManager;

void API_CreateTable(TableDef & table) {
	CatalogManager->CreateTable(table);
}

bool API_FindTable(string name) {
	return CatalogManager->FindTable(name);
}