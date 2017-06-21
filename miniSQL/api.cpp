#define _SCL_SECURE_NO_WARNINGS
#include "api.h"
#include "catalog.h"
#include "record.h"
#include "index.h"

extern Catalog* CatalogManager;
extern Record* RecordManager;
extern Index* IndexManager;

void API_CreateTable(TableDef & table) {
	CatalogManager->CreateTable(table, 1);
}

int API_CreateIndex(string indexName, TableDef & table, int index) {
	Location rootLoc(-1,-1);
	vector<AidedNode> nodeList = RecordManager->SelectForAidedNode(table.name, index);
	rootLoc = IndexManager->BuildBPlus(indexName, table, index, nodeList);
	CatalogManager->CreateIndex(table, index, rootLoc);
	return nodeList.size();
}

int API_DropIndex(string name) {
	
	indexInfo info = IndexManager->dropBPlus(name);
	CatalogManager->dropIndex(info);
	return 0;
}

bool API_FindTable(string name) {
	return CatalogManager->FindTable(name);
}

TableDef & API_FetchTable(string table) {
	return CatalogManager->FetchTable(table);
}

int API_Insert(RecordDef & record) {
	int columnNum = CatalogManager->FetchTable(record.tableName).columnNum;
	return RecordManager->Insert(record, columnNum);
}

int API_FindColumnIndex(string table, string attribute) {
	return CatalogManager->FindColumnNum(table, attribute);
}

dataType API_FindColumnType(string table, string attribute) {
	return CatalogManager->FindColumnType(table, attribute);
}

int API_Select(Query & query) {
	// initialization
	int attrNum = CatalogManager->showHeader(query);
	vector<set<Location>> allLoc;

	// try to use all the existing indices
	if (query.conditionNum == 0) {
		allLoc.push_back(RecordManager->Select(query.tableName));
	}
	for (int i = 0; i < query.conditionNum; i++) {
		set<Location> resLoc;
		int page, offset;
		bool found;
		found = CatalogManager->FindIndex(query.tableName,
			query.columnIndex.at(i));
		if (found && query.operation.at(i) == "=") { // If index can help
			resLoc = IndexManager->Select(query.tableName,
				query.columnIndex.at(i), query.type.at(i),
				query.value.at(i));
		}
		else {
			resLoc = RecordManager->Select(query.tableName,
				query.columnIndex.at(i), query.type.at(i), query.operation.at(i),
				query.value.at(i));
		}
		allLoc.push_back(resLoc);
	}
	// intersection
	set<Location>::iterator iter;

	set<Location> tempLoc;
	set<Location> resLoc = allLoc.at(0);
	for (int i = 1; i <query.conditionNum; i++){
		set_intersection(allLoc.at(i).begin(), allLoc.at(i).end(),
			resLoc.begin(), resLoc.end(), inserter(tempLoc, tempLoc.begin()));
		resLoc = tempLoc;
		tempLoc.clear();
	}
	// display result
	RecordManager->showRecord(attrNum, resLoc);
	cout << endl << endl;
	return resLoc.size();
}

int API_Delete(Query & query) {

	vector<set<Location>> allLoc;
	// try to use all the existing indices
	if (query.conditionNum == 0) {
		allLoc.push_back(RecordManager->Select(query.tableName));
	}
	for (int i = 0; i < query.conditionNum; i++) {
		set<Location> resLoc;
		int page, offset;
		bool found;
		found = CatalogManager->FindIndex(query.tableName,
			query.columnIndex.at(i));
		if (found && query.operation.at(i) == "=") { // If index can help
			resLoc = IndexManager->Select(query.tableName,
				query.columnIndex.at(i), query.type.at(i),
				query.value.at(i));
		}
		else {
			resLoc = RecordManager->Select(query.tableName,
				query.columnIndex.at(i), query.type.at(i), query.operation.at(i),
				query.value.at(i));
		}
		allLoc.push_back(resLoc);
	}
	// intersection
	set<Location> tempLoc = allLoc.at(0);
	set<Location> resLoc = allLoc.at(0);
	for (int i = 1; i <query.conditionNum; i++) {
		set_intersection(allLoc.at(i).begin(), allLoc.at(i).end(),
			tempLoc.begin(), tempLoc.end(), inserter(resLoc, resLoc.begin()));
		tempLoc.clear();
		tempLoc = resLoc;
	}
	// display result

	RecordManager->deleteRecord(tempLoc);
	return resLoc.size();
}

int API_Drop(string name) {
	// drop from record
	Query query(name);
	query.conditionNum = 0;
	int count = API_Delete(query);

	// drop from calalog	
	CatalogManager->dropTable(name, 1);
	return count;
}

bool API_FindIndex(string name) {
	return IndexManager->findBPlus(name);
}