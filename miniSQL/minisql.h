#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
	AttrDef(string n, dataType d, int w, bool u, bool k);
	string name;
	dataType type;
	int width;
	bool unique;
	bool isKey;
};

struct TableDef {
	TableDef(string n, int c);
	string name;
	string primaryKey;
	int columnNum;
	vector<AttrDef> attrList;
};


