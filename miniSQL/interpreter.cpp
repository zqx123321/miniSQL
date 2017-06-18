#include "interpreter.h"
#include "catalog.h"
#include "api.h"
#include "minisql.h"

extern Catalog* CatalogManager;

void INTERPRETER_Main() {
	INTERPRETER_Welcome();
	opType operation = CREATE;
	while (QUIT != operation) {
		try {
			cout << ">> ";
			operation = INTERPRETER_GetOp();
			switch (operation) {
			case CREATE:	INTERPRETER_Create();	break;
			case SELECT:	INTERPRETER_Select();	break;
			case INSERT:	INTERPRETER_Insert();	break;
			case UPDATE:	INTERPRETER_Update();	break;
			case DELETE:	INTERPRETER_Delete();	break;
			case DROP:		INTERPRETER_Drop();		break;
			case EXECFILE:	INTERPRETER_Execfile(); break;
			case QUIT:		break;
			default: break;
			}
		}
		catch (const char* errorMsg) {
			INTERPRETER_NewLine();
			cerr << "Error: ";
			cerr << errorMsg << endl;
		}
		cout << endl;
	}
}

void INTERPRETER_NewLine() {
	char temp;
	do {
		cin.get(temp);
	} while (temp != '\n');
}

void INTERPRETER_Welcome() {
	// first line
	for (int i = 0; i < 50; i++) cout << '/';
	cout << endl;

	// second line
	cout << "//";
	for (int i = 0; i < 46; i++) cout << ' ';
	cout << "//" << endl;

	// third line
	cout << "//";
	for (int i = 0; i < 12; i++) cout << ' ';
	cout << "Welcome to JZF miniSQL";
	for (int i = 0; i < 12; i++) cout << ' ';
	cout << "//" << endl;

	// fourth line
	cout << "//";
	for (int i = 0; i < 46; i++) cout << ' ';
	cout << "//" << endl;

	// fifth line
	for (int i = 0; i < 50; i++) cout << '/';
	cout << endl << endl;
}

opType INTERPRETER_GetOp() {
	string cmd;
	cin >> cmd;
	if (cmd == "create")
		return CREATE;
	else if (cmd == "select")
		return SELECT;
	else if (cmd == "insert")
		return INSERT;
	else if (cmd == "update")
		return UPDATE;
	else if (cmd == "delete")
		return DELETE;
	else if (cmd == "drop")
		return DROP;
	else if (cmd == "execfile")
		return EXECFILE;
	else if (cmd == "quit")
		return QUIT;
	else
		throw "Undefined operation!";
}

void INTERPRETER_Create() {
	
	// read in all the word
	string word;
	string sentence;
	do {
		cin >> word;
		sentence += word + ' ';
		
	} while (word.back() != ';');
	
	// parse for create
	vector<string> element = split(sentence);
	vector<string>::const_iterator iter = element.begin();
	
	if (iter == element.end()) {
		throw "Empty command!";
	}
	else if (*iter == "table") {
		string name;
		iter++;
		if (iter != element.end()) {
			string name = *iter;
			if (API_FindTable(name))
				throw "Table already exists!";
			iter++;

			TableDef newTable(name, 0);
			bool done = false;

			while (1) {
				string attrName, attrType;
				int attrWidth;
				dataType type;
				bool isUnique = false;

				done = false;
				attrName = *iter;
				if (attrName == "primary")
					throw "primary is a key word, you may not use it!";
				iter++;
				if (iter == element.end())
					break;

				attrType = *iter;
				if (attrType != "int" && attrType != "float" && attrType != "char") {
					throw "Undefined data type!";
					break;
				}
				if (attrType == "char") {
					type = CHAR;
					iter++;
					if (iter == element.end())
						break;
					else {
						string temp = *iter;
						attrWidth = atoi(temp.c_str());
					}
				}
				else if (attrType == "int") {
					type = INT;
					attrWidth = sizeof(int);
				}
				else {
					type = FLOAT;
					attrWidth = sizeof(float);
				}
				iter++;

				if (iter != element.end() && *iter == "unique") {
					isUnique = true;
					iter++;
				}

				newTable.attrList.push_back(AttrDef(attrName, type, attrWidth,
					isUnique));
				newTable.columnNum++;
				done = true;
				if (iter == element.end() || *iter == "primary")
					break;
			}

			if (iter == element.end())
				;
			else if (*iter == "primary") {
				iter++;
				if (iter == element.end())
					done = false;
				else if (*iter != "key")
					throw "Syntax error!";
				else {
					iter++;
					if (iter == element.end())
						done = false;
					else {
						newTable.primaryKey = *iter;
					}
				}
			}
			if (done == false)
				throw "Information is not complete enough to create a table!";

			API_CreateTable(newTable);
			cout << "Create successfully!" << endl;
			cout << newTable.columnNum << " row(s) affected." << endl;
		}
		else {
			throw "Table name empty!";
			return;
		}
	}
		

	else if (*iter == "index") {

	}
	else {
		throw "Only table or index can be created!";
	}

}

void INTERPRETER_Select() {
	// read in all the word
	string word;
	string sentence;
	do {
		cin >> word;
		sentence += word + ' ';

	} while (word.back() != ';');

	// parse for create
	vector<string> element = split(sentence);
	vector<string>::const_iterator iter = element.begin();

	iter++; // skip *
	iter++; // skip from
	string tableName = *iter;
	if (API_FindTable(tableName) == false)
		throw "Table doesn't exist!";
	iter++;

	Query newQuery(tableName);
	newQuery.conditionNum = 0;

	if (iter != element.end()) {
		iter++; //skip where
		do {
			string attributeName = *iter;
			
			int index = API_FindColumnIndex(tableName, attributeName);
			newQuery.columnIndex.push_back(index);
			iter++;

			dataType t = API_FindColumnType(tableName, attributeName);
			newQuery.type.push_back(t);

			string operation = *iter;
			newQuery.operation.push_back(operation);
			*iter++;

			string value = *iter;
			newQuery.value.push_back(value);
			iter++;

			newQuery.conditionNum++;
			if (iter != element.end() && *iter == "and")
				iter++;
		} while (iter != element.end());
	}

	int count;
	count = API_Select(newQuery);
	cout << "Select successfully!" << endl;
	cout << count << " row(s) affected." << endl;
}

void INTERPRETER_Insert() {
	string word;
	string sentence;
	do {
		cin >> word;
		sentence += word + ' ';

	} while (word.back() != ';');

	// parse for create
	vector<string> element = split(sentence);
	vector<string>::const_iterator iter = element.begin();

	iter++; // skip into
	string tableName = *iter;
	iter++; // skip value/values
	iter++;
	if (API_FindTable(tableName) == false)
		throw "Table doesn't exist!";

	RecordDef newRecord(tableName);
	while (iter != element.end()) {
		string temp = *iter;
		newRecord.value.push_back(temp);
		iter++;
	}

	int count;
	count = API_Insert(newRecord);
	cout << "Insert successfully!" << endl;
	cout << count << " row(s) affected." << endl;
}

void INTERPRETER_Update() {

}

void INTERPRETER_Delete() {
	// read in all the word
	string word;
	string sentence;
	do {
		cin >> word;
		sentence += word + ' ';

	} while (word.back() != ';');

	// parse for create
	vector<string> element = split(sentence);
	vector<string>::const_iterator iter = element.begin();

	iter++; // skip from
	string tableName = *iter;
	if (API_FindTable(tableName) == false)
		throw "Table doesn't exist!";
	iter++;

	Query newQuery(tableName);
	newQuery.conditionNum = 0;

	if (iter != element.end()) {
		iter++; //skip where
		do {
			string attributeName = *iter;

			int index = API_FindColumnIndex(tableName, attributeName);
			newQuery.columnIndex.push_back(index);
			iter++;

			dataType t = API_FindColumnType(tableName, attributeName);
			newQuery.type.push_back(t);

			string operation = *iter;
			newQuery.operation.push_back(operation);
			*iter++;

			string value = *iter;
			newQuery.value.push_back(value);
			iter++;

			newQuery.conditionNum++;
			if (iter != element.end() && *iter == "and")
				iter++;
		} while (iter != element.end());
	}

	int count;
	count = API_Delete(newQuery);
	cout << "Delete successfully!" << endl;
	cout << count << " row(s) affected." << endl;
}

void INTERPRETER_Drop() {

}

void INTERPRETER_Execfile() {

}