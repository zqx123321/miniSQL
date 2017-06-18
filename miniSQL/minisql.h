#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <iterator>
using namespace std;

enum dataType {
	INT, CHAR, FLOAT
};

enum fileType {
	CATALOG, RECORD, INDEX, UNKNOWN
};

enum writeMode {
	ADD, COVER
};

vector<string> split(const string s);

struct AttrDef {
	AttrDef(string name, dataType type, int width, bool unique);
	string name;
	dataType type;
	int width;
	bool unique;
	bool hasIndex;
	int indexPage;
	int indexOffset;
};

struct TableDef {
	TableDef(string name, int columnNum);
	string name;
	string primaryKey;
	int columnNum;
	vector<AttrDef> attrList;
};

struct RecordDef {
	RecordDef(string tableName);
	string tableName;
	vector<string> value;
};

struct Key {
	int page;
	int offset;
	string value;
	Key(int page, int offset, string value);
};

struct NodeDef {
	bool isLeaf;
	bool isRoot;
	int fatherPage;
	int fatherOffset;
	int keyNum;
	vector<Key> allKeys;
	NodeDef(bool isLeaf, bool isRoot, 
		int fatherPage, int fatherOffset, int keyNum);
};

struct Query {
	Query(string t);
	string tableName;
	int conditionNum;
	vector<int> columnIndex;
	vector<dataType> type;
	vector<string> operation;
	vector<string> value;
};

struct Location {
	Location(int p, int o) : page(p), offset(o) {}
	int page;
	int offset;
	bool operator<(const Location & a) const{
		return page < a.page || offset < a.offset;
	}
};



