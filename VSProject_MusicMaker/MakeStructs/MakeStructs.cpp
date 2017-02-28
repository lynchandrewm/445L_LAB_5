// MakeStructs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
using namespace std;


int main()
{
	string FileName = "Structs.txt";
	ofstream file(FileName);
	for (int i = 0; i < 720; i++) {
		for (int k = 0; k < 10; k++) {
			file << "{0,0}, ";
		}
		file << '\n';
	}
    return 0;
}

