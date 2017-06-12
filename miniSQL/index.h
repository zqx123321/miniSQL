#pragma once
#include "minisql.h"

class Index {
	
};

class node {
protected:
	bool isLeaf;
	bool isRoot;
	int keyNum;
	int fatherBlock;
	int fatherOffset;
public:
	node();
	virtual ~node();
};

class internalNode : public node {
	
};

class leafNode : public node {
	int blockNum;
	int offset;

};