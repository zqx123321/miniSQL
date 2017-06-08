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
			cout << ">>";
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
			cerr << "Error: ";
			cerr << errorMsg << endl;
		}
		cout << endl;
	}
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
	vector<string> command;

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
		iter++;
		if(iter != element.end()) {
			string tableName = *iter;
			if (CatalogManager->FindTable(tableName) == true)
				throw "Table name already exists!";
			iter++;
			bool done = false;
			TableDef newTable(tableName, 0);

			while (1) {
				string attrName;
				dataType type;
				int width;
				bool unique;
				bool isKey;

				done = false;
				attrName = *iter;
				if (attrName == "primary")
					throw "primary is a key word, you may not use it!";
				iter++;
				if (iter == element.end())
					break;

				string attrType = *iter;
				if (attrType != "int" && attrType != "float" && attrType != "char")
					throw "Undefined data type!";
				if (attrType == "char") {
					type = CHAR;
					iter++;
					if (iter == element.end())
						break;
					else {
						string temp = *iter;
						width = atoi(temp.c_str());
					}
				}
				else {
					width = 0;
					if (attrType == "int")
						type = INT;
					else
						type = FLOAT;
				}
				iter++;

				if (iter != element.end() && *iter == "unique") {
					unique = true;
					iter++;
				}
				else
					unique = false;
				isKey = false;

				AttrDef newAttr(attrName, type, width, unique, isKey);
				newTable.attrList.push_back(newAttr);
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
						string priKey = *iter;
						newTable.primaryKey = priKey;
					}
				}
			}
			if (done == false)
				throw "Information is not complete enough to create a table!";
			
		// construct the command
			char buf[64];
			string temp;
			/*
			command.push_back(newTable.name);
			command.push_back(newTable.primaryKey);
			sprintf(buf, "%d", newTable.columnNum);
			temp = buf;
			command.push_back(temp);
			*/


			cout << sizeof(newTable);

		}
		else
			throw "Table name empty!";

		/*
		char buf[10];
		sprintf(buf, "%d", columnNum);
		string count(buf);
		vector<string>::const_iterator iter2;
		iter2 = command.begin();
		command.insert(iter2 + 1, count);*/
		
		
		API_Main(CREATE_TABLE, command);
	}
	else if (*iter == "index") {

		API_Main(CREATE_INDEX, command);
	}
	else {
		throw "Only table or index can be created!";
	}
}

void INTERPRETER_Select() {

}

void INTERPRETER_Insert() {

}

void INTERPRETER_Update() {

}

void INTERPRETER_Delete() {

}

void INTERPRETER_Drop() {

}

void INTERPRETER_Execfile() {

}