// MusicMaker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define MAX_FILE_SIZE 100

struct musicInfo {
	float freq;
	float duration;
	bool slur;
};

struct musicInfo musicArray[MAX_FILE_SIZE];
int musicIndex = 0;

float freqFinder(string str);

int main()
{
	int debug;
	int fbus, waveRes;
	float ftempo;
	string outputTitle;
	cout << "Welcome to MusicMaker! I hope this works.\n";
	cout << "Please ensure the file is formatted correctly.\n";
	cout << "Because this really wont work if it's not.\n";
	cout << "Name of file (in this directory): ";
	string inputFileLocation;
	cin >> inputFileLocation;
	ifstream inputfile(inputFileLocation);
	if (!inputfile.is_open()) {
		cout << "Failed to open file :(\n";
	}
	else {
		cout << "Success!\n";
	}
	cout << "Bus frequency:";
	cin >> fbus;
	cout << "Resoultion of sound wave:";
	cin >> waveRes;
	cout << "Music Tempo:";
	cin >> ftempo;
	ftempo = ftempo / 60;
	string line;
	std::size_t found;
	while (getline(inputfile, line)) {
		found = line.find("{");
		if (found != string::npos) {
			getline(inputfile, line);
			break;
		}
	}
	char *line_c, *temp_c;
	do {
		line_c = new char[line.length() + 1];
		std::strcpy(line_c, line.c_str());
		temp_c = std::strtok(line_c, " ,");
		musicArray[musicIndex].freq = freqFinder(string(temp_c));
		temp_c = std::strtok(NULL, " ,");
		musicArray[musicIndex].duration = stof(string(temp_c));
		temp_c = std::strtok(NULL, " ,");
		musicArray[musicIndex].slur = (bool)stoi(string(temp_c));
		musicIndex++;
		getline(inputfile, line);
		found = line.find("}");
	} while (found == string::npos);
	inputfile.close();

	cout << "Name of new file (no .txt):";
	string outputFileName;
	cin >> outputFileName;
	string outputFileLocation = string(outputFileName);
	outputFileLocation.append(".txt");
	ofstream outputFile(outputFileLocation);
	outputFile.clear();
	outputFile << "const music " << outputFileName << "[] = {\n";
	float a, b;
	for (int i = 0; i < musicIndex; i++) {
		if (musicArray[i].freq) {
			a = fbus / (musicArray[i].freq * waveRes);
			b = musicArray[i].freq * musicArray[i].duration * waveRes / ftempo;
		}
		else {
			a = musicArray[i].duration * fbus / ftempo;
			b = 1;
		}
		outputFile << "  { " << (int)a << ", " << (int)b << " },\n";
		if (!musicArray[i].slur) {
			a = (musicArray[i].duration * 0.9) * fbus / ftempo;
			b = 1;
			outputFile << "  { " << (int)a << ", " << (int)b << " },\n";
		}
	}
	outputFile << "};";
	outputFile.close();
	cout << "Safe to close this terminal";
	cin >> debug;
}

float freqFinder(string str) {
	if (!str.compare("rest")) {
		return 0;
	}
	if (!str.compare("a4")) {
		return 440;
	}
	if (!str.compare("b4")) {
		return 493.88;
	}
	if (!str.compare("c4")) {
		return 261.63;
	}
	if (!str.compare("d4")) {
		return 293.66;
	}
	if (!str.compare("e4")) {
		return 329.63;
	}
	if (!str.compare("f4")) {
		return 349.23;
	}
	if (!str.compare("g4")) {
		return 392;
	}
	if (!str.compare("d5")) {
		return 587.33;
	}
	return -1;
}

