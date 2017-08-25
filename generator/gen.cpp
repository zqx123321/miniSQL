#include <fstream>
#include <iostream>
using namespace std;

int main(){
	ofstream fout;
	fout.open("0.catalog", ios::out | ios::binary);
	fout.seekp(0);
	
	int a = 1;
	fout.write((char*)&a, sizeof(int));
	int b = 0;
	fout.write((char*)&b, sizeof(int));
	fout.close();
	cout << "haha";
	cin.get();
	cin.get();
	return 0;
}
