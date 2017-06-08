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
		if (iter != element.end()) {
			string temp = *iter;
			command.push_back(temp); // table name
			iter++;

			bool done = false;
			string attrName, attrType, attrType_app, unique;
			int columnNum = 0;
			while (1) {
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
					iter++;
					if (iter == element.end())
						break;
					else
						attrType_app = *iter;
				}
				else
					attrType_app = "0";
				iter++;

				if (iter != element.end() && *iter == "unique") {
					unique = "1";
					iter++;
				}
				else
					unique = "0";
				command.push_back(attrName);
				command.push_back(attrType);
				command.push_back(attrType_app);
				command.push_back(unique);
				columnNum++;
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
						command.push_back(priKey);
					}
				}
			}
			if (done == false)
				throw "Information is not complete enough to create a table!";

			char buf[10];
			sprintf(buf, "%d", columnNum);
			string count(buf);
			vector<string>::const_iterator iter2;
			iter2 = command.begin();
			command.insert(iter2 + 1, count);
		}
		else {
			throw "Table name empty!";
			return;
		}
		// execute
		API_CreateTable(command);
	}

	else if (*iter == "index") {

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