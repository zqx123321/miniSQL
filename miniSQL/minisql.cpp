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

NodeDef::NodeDef(bool isLeaf, bool isRoot,
	int fatherPage, int fatherOffset, int keyNum)
	: isLeaf(isLeaf), isRoot(isRoot), fatherPage(fatherPage),
	fatherOffset(fatherOffset), keyNum(keyNum) {
	parent = nullptr;
}

Query::Query(string t) : tableName(t) {}

void BPlus::insertNode(AidedNode & node) {
	// if the tree is empty, create an empty lead node L, which is also the root
	if (Root == nullptr) {
		
		Root = new NodeDef(true, true, -1, -1, 0);
		Root->allKeys.push_back(&node);
		Root->keyNum = 1;
		Root->parent = nullptr;
	}
	// if the tree is not empty
	else {
		NodeDef & Leaf = findLeaf(node.value);
		if (Leaf.keyNum < order) {
			insertInLeaf(Leaf, node);
		}
			
		// need spliting
		else {
			// create node L'
			NodeDef* newLeaf = new NodeDef(true, false, -1, -1, 0);
			// copy to memory
			NodeDef* temp = new NodeDef(true, false, -1, -1, 0);
			int size = Leaf.allKeys.size();
			
			int i;
			for (i = 0; i < size; i++)
				temp->allKeys.push_back(Leaf.allKeys.at(i));
			
			insertTemp(*temp, node);
			

			// redistribution into 2 nodes
			int half = (order+1) / 2;
			for (i = half; i <= order; i++)
				newLeaf->allKeys.push_back(temp->allKeys.at(i));
			newLeaf->keyNum = order - half + 1;

			if (Leaf.children.size())
				newLeaf->children.push_back
					(Leaf.children.back());
			
			Leaf.allKeys.clear();
			Leaf.children.clear();
			for (i = 0; i < half; i++)
				Leaf.allKeys.push_back(temp->allKeys.at(i));
			Leaf.keyNum = half;
			Leaf.children.clear();
			Leaf.children.push_back(newLeaf);

			// insert them into their parent
			AidedNode & Key = (*newLeaf->allKeys.at(0));
			insertInParent(*(Leaf.parent), Leaf, *newLeaf, Key);
		}
	}
}

// for leaf node
void BPlus::insertTemp(NodeDef & temp, AidedNode & node) {
	int size = temp.allKeys.size();
	int i = 0;
	for (i = 0; i < size; i++) {
		AidedNode & old = *(temp.allKeys.at(i));
		if (Judge(old.value, node.value, type, ">"))
			break;
	}
	temp.allKeys.insert(temp.allKeys.begin() + i, &node);
	temp.keyNum++;
}

NodeDef & BPlus::findLeaf(string value) {
	NodeDef* C;
	C = Root;
	// while C is not a leaf
	while (C->isLeaf == false) {
		// Let i = smallest number sunch that V <=C.Ki
		int i, size = C->allKeys.size();
		string Key;
		for (i = size-1; i >= 0; i--) {
			Key = (*C->allKeys.at(i)).value;
			if (Judge(value, Key, type, "<"))
				break;
		}
		// if not such i
		if (i == -1) {
			C = C->children.at(size);
		}
		else {
			C = C->children.at(i);
		}
	}
	return *C;
}

// insert into leaf, no need adding any pointer
void BPlus::insertInLeaf(NodeDef & Leaf, AidedNode & node) {
	string FirstKey = (*Leaf.allKeys.at(0)).value;
	string nodeKey = node.value;
	if (Judge(nodeKey, FirstKey, type, "<")) {
		Leaf.allKeys.insert(Leaf.allKeys.begin(), &node);
	}
		
	else {
		int size = Leaf.allKeys.size();
		int i = 0;
		for (i = 0; i < size; i++) {
			string Key = (*Leaf.allKeys.at(i)).value;
			if (Judge(Key, nodeKey, type, ">"))
				break;
		}
		Leaf.allKeys.insert(Leaf.allKeys.begin() + i, &node);
	}
	Leaf.keyNum++;
}

string BPlus::DFSWrite(NodeDef & C) {
	char buffer[12];
	string result;
	int size = C.allKeys.size();

	if (C.isLeaf == false) {
		size = C.children.size();
		for (int i = 0; i < size; i++)
			result += DFSWrite((*C.children.at(i)));
	}
	else {
		result += " ` ";

		// isLeaf
		memset(buffer, 0, sizeof(buffer));
		if (C.isLeaf == true) {
			sprintf(buffer, "%d", 1);
			result += buffer;
			result += ' ';
		}
		else {
			sprintf(buffer, "%d", 0);
			result += buffer;
			result += ' ';
		}

		// isRoot
		memset(buffer, 0, sizeof(buffer));
		if (C.isRoot == true) {
			sprintf(buffer, "%d", 1);
			result += buffer;
			result += ' ';
		}
		else {
			sprintf(buffer, "%d", 0);
			result += buffer;
			result += ' ';
		}

		// fatherPage
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d", C.fatherPage);
		result += buffer;
		result += ' ';

		// fatherOffset
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d", C.fatherOffset);
		result += buffer;
		result += ' ';

		// keyNum
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d", C.keyNum);
		result += buffer;
		result += ' ';

		// allKeys
		for (int i = 0; i < C.allKeys.size(); i++) {

			result += C.allKeys.at(i)->value;
			result += ' ';
			
			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer, "%d", C.allKeys.at(i)->loc.page);
			result += buffer;
			result += ' ';

			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer, "%d", C.allKeys.at(i)->loc.offset);
			result += buffer;
			result += ' ';
		}
		result += '~';
	}
	return result;
}

string BPlus::Write() {
	char buffer[12];
	string result;
	result += name;
	result += ' ';

	result += tableName;
	result += ' ';

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%d", index);
	result += buffer;
	result += ' ';

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%d", order);
	result += buffer;
	result += ' ';

	switch (type)
	{
	case INT:
		result += "int";
		break;
	case CHAR:
		result += "char";
		break;
	case FLOAT:
		result += "float";
		break;
	default:
		break;
	}
	result += ' ';

	NodeDef & C = *Root;
	result += DFSWrite(C);
	return result;
}

void BPlus::test(NodeDef & N) {
	int size = N.allKeys.size();
	cout << size << ':';

	for (int i = 0; i < size; i++)
		cout << (*N.allKeys.at(i)).value << ' ';
	cout << endl;

	if (N.isLeaf == false) {
		size = N.children.size();
		for (int i = 0; i < size; i++)
			test((*N.children.at(i)));
	}
}

void BPlus::insertInParent(NodeDef & parent, NodeDef & L1, NodeDef & L2, AidedNode & key) {
	// if L1 is the root of the tree
	
	if (L1.isRoot == true) {
			
		NodeDef* node = new NodeDef(false, true, -1, -1, 0);
		Root = node;
		Root->allKeys.push_back(&key);
		Root->children.push_back(&L1);
		Root->children.push_back(&L2);
		Root->keyNum = 1;
		L1.isRoot = false;
		L1.parent = Root;
		L1.fatherPage = key.loc.page;
		L1.fatherOffset = key.loc.page;
		L2.parent = Root;
		L2.fatherPage = key.loc.page;
		L2.fatherOffset = key.loc.page;
	}
	else {
		if (parent.keyNum < order - 1) {
			// insert N'
			int size = parent.allKeys.size();
			int i;
			for (i = 0; i < size; i++) {
				if (&(*parent.children.at(i)) == &L1)
					break;
			}
			i++;
			parent.children.insert(parent.children.begin() + i, &L2);
			parent.allKeys.push_back(&key);
			L2.parent = &parent;
			L2.fatherOffset = parent.allKeys.at(0)->loc.offset;
			L2.fatherPage = parent.allKeys.at(0)->loc.page;
			parent.keyNum++;
		}
		else {
			NodeDef* newNode = new NodeDef(false, false, -1, -1, 0);
			NodeDef* temp = new NodeDef(false, false, -1, -1, 0);
			
			int i, size = parent.allKeys.size();
			for (i = 0; i < size; i++) {
				temp->allKeys.push_back(parent.allKeys.at(i));
				temp->children.push_back(parent.children.at(i));
			}
			temp->children.push_back(parent.children.at(i));
			
			insertTemp(*temp, key);
			for (i = 0; i < size; i++)
				if ((*temp->allKeys.at(i)).value == key.value)
					break;
			i++;
			temp->children.insert(temp->children.begin() + i, &L2);

			int half = order / 2;
			AidedNode & Key = *(temp->allKeys.at(half));
			

			for (i = half + 1; i < order; i++) {
				newNode->allKeys.push_back(temp->allKeys.at(i));
				newNode->children.push_back(temp->children.at(i));
				temp->children.at(i)->parent = newNode;
				temp->children.at(i)->fatherOffset = temp->allKeys.at(half + 1)->loc.offset;
				temp->children.at(i)->fatherPage = temp->allKeys.at(half + 1)->loc.page;
			}
			newNode->children.push_back(temp->children.at(i));
			temp->children.at(i)->parent = newNode;
			temp->children.at(i)->fatherOffset = temp->allKeys.at(half + 1)->loc.offset;
			temp->children.at(i)->fatherPage = temp->allKeys.at(half + 1)->loc.page;
			newNode->keyNum = order - half - 1;

			parent.allKeys.clear();
			parent.children.clear();
			
			for (i = 0; i < half; i++) {
				parent.allKeys.push_back(temp->allKeys.at(i));
				parent.children.push_back(temp->children.at(i));
			}
			parent.children.push_back(temp->children.at(i));
			parent.keyNum = half + 1;
			// insert them into their parent
			insertInParent(*(parent.parent), parent, *newNode, Key);
		}
	}
}