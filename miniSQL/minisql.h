#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

enum dataType {
	INT, CHAR, FLOAT
};

class Buffer;
extern Buffer* BufferManager;

class Record;
extern Record* RecordManager;

class Index;
extern Index* IndexManager;

class Catalog;
extern Catalog* CatalogManager;

vector<string> split(const string s);


