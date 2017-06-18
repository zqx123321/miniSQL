#include "minisql.h"

AttrDef::AttrDef(string name, dataType type, int width, bool unique)
	: name(name), type(type), width(width), unique(unique){
	hasIndex = false;
	indexPage = -1;
	indexOffset = -1;
}

TableDef::TableDef(string name, int columnNum)
	: name(name), columnNum(columnNum) {}

RecordDef::RecordDef(string tableName)
	: tableName(tableName) {}

Key::Key(int p, int o, string v)
	: page(p), offset(o), value(v) {}

NodeDef::NodeDef(bool isLeaf, bool isRoot,
	int fatherPage, int fatherOffset, int keyNum)
	: isLeaf(isLeaf), isRoot(isRoot), fatherPage(fatherPage),
	fatherOffset(fatherOffset), keyNum(keyNum) {}
