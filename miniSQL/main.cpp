#define _CRT_SECURE_NO_WARNINGS
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
	CatalogManager = new Catalog();
	RecordManager = new Record();
	IndexManager = new Index();
	BufferManager = new Buffer();
	BufferManager->loadAllPages();
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
				|| s[end] == ',' || s[end] == ' ') {
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
