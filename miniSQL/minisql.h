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

struct Location {
	Location(int p, int o) : page(p), offset(o) {}
	int page;
	int offset;
	bool operator<(const Location & a) const {
		return page < a.page || offset < a.offset;
	}
};

struct AidedNode {
	AidedNode(string v, Location l) : value(v), loc(l) {}
	string value;
	Location loc;
};

struct NodeDef {
	bool isLeaf;
	bool isRoot;
	int fatherPage;
	int fatherOffset;
	NodeDef * parent;
	int keyNum;
	vector<AidedNode*> allKeys;
	vector<NodeDef*> children;
	NodeDef(bool isLeaf, bool isRoot, 
		int fatherPage, int fatherOffset, int keyNum);
private:
	NodeDef(const NodeDef &);
};

struct BPlus {
	BPlus(string n, string n2, int i, dataType t) : name(n),
		tableName(n2), index(i),
		type(t), order(3) { Root = nullptr; }
	string name;
	string tableName;
	int index;
	int order;
	NodeDef* Root;
	dataType type;
	void insertNode(AidedNode & node);
	NodeDef & findLeaf(string value);
	void insertInLeaf(NodeDef & Leaf, AidedNode & node);
	void insertInParent(NodeDef & parent, NodeDef & L1, NodeDef & L2, AidedNode & key);
	void insertTemp(NodeDef & temp, AidedNode & node);
	string DFSWrite(NodeDef & C);
	string Write();
	void test(NodeDef & N);
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

struct indexInfo {
	indexInfo(string t, int i) : tableName(t), index(i) {}
	string tableName;
	int index;
};



vector<string> split(const string s);

bool Judge(string temp, string value, dataType type, string operation);

