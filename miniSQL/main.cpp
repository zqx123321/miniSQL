#include "minisql.h"
#include "interpreter.h"
#include "buffer.h"
#include "catalog.h"
#include "record.h"
#include "index.h"

extern Record* RecordManager;
extern Index* IndexManager;
extern Catalog* CatalogManager;
extern Buffer* BufferManager;

void TOP_Initial();
void TOP_execute();
void TOP_exit();

int main() {
	TOP_Initial();
	TOP_execute();
	TOP_exit();
}

void TOP_Initial() {
	BufferManager = new Buffer();
	BufferManager->loadAllPages();

	CatalogManager = new Catalog();
	CatalogManager->LoadAllTables();
	RecordManager = new Record();
	IndexManager = new Index();
	
}

void TOP_execute() {
	INTERPRETER_Main();
}

void TOP_exit() {
	delete CatalogManager;
	delete BufferManager;
	delete RecordManager;
	delete IndexManager;
}

vector<string> split(const string s) {
	vector<string> result;
	int begin, end;

	begin = end = 0;
	while (end < s.size()) {
		if (s[end] == '"' || s[end] == '(' || s[end] == ')' || s[end] == ';'
				|| s[end] == ',' || s[end] == ' ' || s[end] == '\'') {
			if (begin <= end - 1) {
				string temp = s.substr(begin, end - begin);
				result.push_back(temp);
			}
			begin = end + 1;
			end = begin;
		}
		else
			end++;
	}
	return result;
}

bool Judge(string temp, string value, dataType type, string operation) {
	bool result;

	if (operation == "=") {
		switch (type)
		{
		case INT:
			int  num1,num2;
			sscanf(temp.c_str(), "%d", &num1);
			sscanf(value.c_str(), "%d", &num2);
			result = num1 == num2;
			break;
		case CHAR:
			result = temp == value;
			break;
		case FLOAT:
			float float1, float2;
			sscanf(temp.c_str(), "%f", &float1);
			sscanf(value.c_str(), "%f", &float2);
			result = float1 == float2;
			break;
		default:
			break;
		}
	}
	else if (operation == "<>") {
		switch (type)
		{
		case INT:
			int num1, num2;
			sscanf(temp.c_str(), "%d", &num1);
			sscanf(value.c_str(), "%d", &num2);
			result = num1 != num2;
			break;
		case CHAR:
			result = temp != value;
			break;
		case FLOAT:
			float float1, float2;
			sscanf(temp.c_str(), "%f", &float1);
			sscanf(value.c_str(), "%f", &float2);
			result = float1 != float2;
			break;
		default:
			break;
		}
	}
	else if (operation == "<") {
		switch (type)
		{
		case INT:
			int num1, num2;
			sscanf(temp.c_str(), "%d", &num1);
			sscanf(value.c_str(), "%d", &num2);
			result = num1 < num2;
			break;
		case CHAR:
			result = temp < value;
			break;
		case FLOAT:
			float float1, float2;
			sscanf(temp.c_str(), "%f", &float1);
			sscanf(value.c_str(), "%f", &float2);
			result = float1 < float2;
			break;
		default:
			break;
		}
	}
	else if (operation == ">") {
		switch (type)
		{
		case INT:
			int num1, num2;
			sscanf(temp.c_str(), "%d", &num1);
			sscanf(value.c_str(), "%d", &num2);
			result = num1 > num2;
			break;
		case CHAR:
			result = temp > value;
			break;
		case FLOAT:
			float float1, float2;
			sscanf(temp.c_str(), "%f", &float1);
			sscanf(value.c_str(), "%f", &float2);
			result = float1 > float2;
			break;
		default:
			break;
		}
	}
	else if (operation == "<=") {
		switch (type)
		{
		case INT:
			int num1, num2;
			sscanf(temp.c_str(), "%d", &num1);
			sscanf(value.c_str(), "%d", &num2);
			result = num1 <= num2;
			break;
		case CHAR:
			result = temp <= value;
			break;
		case FLOAT:
			float float1, float2;
			sscanf(temp.c_str(), "%f", &float1);
			sscanf(value.c_str(), "%f", &float2);
			result = float1 <= float2;
			break;
		default:
			break;
		}
	}
	else if (operation == ">=") {
		switch (type)
		{
		case INT:
			int num1, num2;
			sscanf(temp.c_str(), "%d", &num1);
			sscanf(value.c_str(), "%d", &num2);
			result = num1 >= num2;
			break;
		case CHAR:
			result = temp >= value;
			break;
		case FLOAT:
			float float1, float2;
			sscanf(temp.c_str(), "%f", &float1);
			sscanf(value.c_str(), "%f", &float2);
			result = float1 >= float2;
			break;
		default:
			break;
		}
	}
	else
		throw "Unknown comparison!";

	return result;
}

