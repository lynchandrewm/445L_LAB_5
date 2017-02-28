// MusicMaker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
using namespace std;

#define MAX_FILE_SIZE 100
#define MAX_NUMBER_OF_FILES 20
#define MAX_NUMBER_OF_NOTES 200

struct musicInfo {
	string note;
	float duration;
	bool slur;
};

struct musicfile {
	struct musicInfo musicArray[MAX_FILE_SIZE];
	int index;
	int size;
	string partname;
};

struct note {
	string a_str;
	float a_f;
};

struct Legend {
	struct note notes[MAX_NUMBER_OF_NOTES], rests[MAX_NUMBER_OF_NOTES];
	int numNotes, numRests;
};

enum {
	newNote,
	newRest,
	exists
};

struct Legend MusicKey;
struct musicfile files[MAX_NUMBER_OF_FILES];
int filesIndex = 0;

int waveRes, numFiles;
float ftempo, fbus;

void AddToMusicKey(int flag, string note, int duration);
void MakeNoteKey(string* outputStr);
float freqFinder(string str);

int main()
{
	int debug;
	string outputTitle, line;
	cout << "Welcome to MusicMaker! I hope this works.\n";
	cout << "Please ensure the file is formatted correctly.\n";
	cout << "Because this really wont work if it's not.\n";
	cout << "Processor Play frequency:";
	cin >> fbus;
	cout << "Resoultion of sound wave:";
	cin >> waveRes;
	cout << "Music Tempo:";
	cin >> ftempo;
	ftempo = ftempo / 60;
	cout << "Number of files to process:";
	cin >> numFiles;
	
	// read and parse files
	for (int k = 0; k < numFiles; k++) {
		cout << "Name of file (no extension): ";
		string inputFileName;
		cin >> inputFileName;
		ifstream inputfile(inputFileName);
		if (!inputfile.is_open()) {
			cout << "Failed to open file :(\n";
		}
		else {
			cout << "Success!\n";
		}
		files[k].partname = inputFileName;
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
			files[k].musicArray[files[k].index].note = string(temp_c);
			temp_c = std::strtok(NULL, " ,");
			files[k].musicArray[files[k].index].duration = stof(string(temp_c));
			temp_c = std::strtok(NULL, " ,");
			files[k].musicArray[files[k].index].slur = (bool)stoi(string(temp_c));
			files[k].index++;
			getline(inputfile, line);
			found = line.find("}");
		} while (found == string::npos);
		files[k].size = files[k].index;
		inputfile.close();
	}

	// make the new file
	string tempLocation = "temp";
	ofstream tempOutFile(tempLocation);
	tempOutFile.clear();

	// add the music parts
	for (int k = 0; k < numFiles; k++) {
		tempOutFile << "const music " << files[k].partname << "[] = {\n";
		string a;
		float b;
		string rest;  int restCount = 0; float restDuration, noteValue;
		for (int i = 0; i < files[k].size; i++) {
			rest = string("rest");
			float freq = freqFinder(files[k].musicArray[i].note);
			if (freq) {
				a = files[k].musicArray[i].note;
				noteValue = fbus / (freq * waveRes);
				AddToMusicKey(newNote, files[k].musicArray[i].note, noteValue);
				b = freq * files[k].musicArray[i].duration * waveRes / ftempo;
			}
			else {
				a = rest.append(to_string(restCount++));
				restDuration = files[k].musicArray[i].duration;
				AddToMusicKey(newRest, rest, restDuration);
				b = 1;
			}
			tempOutFile << "  { " << a << ", " << (int)b << " },\n";
			if (!files[k].musicArray[i].slur) {
				a = rest.append(to_string(restCount++));
				restDuration = (files[k].musicArray[i].duration * 0.9) * fbus / ftempo;
				AddToMusicKey(newRest, rest, restDuration);
				b = 1;
				tempOutFile << "  { " << a << ", " << (int)b << " },\n";
			}
		}
		tempOutFile << "};\n\n";
	}

	// output "a" key
	tempOutFile.close();
	cout << "Name of new file (with extension):";
	string outputFileLocation;
	cin >> outputFileLocation;
	ofstream finalFile(outputFileLocation);
	finalFile << "/****************************************\n";
	finalFile << "     Begin Music Note Definitions\n";
	finalFile << "****************************************/\n";
	for (int i = 0; i < MusicKey.numNotes; i++) {
		finalFile << "#define " << MusicKey.notes[i].a_str << "  " << (int)MusicKey.notes[i].a_f << '\n';
	}
	finalFile << '\n';
	finalFile << "/****************************************\n";
	finalFile << "     Begin Music Rest Definitions\n";
	finalFile << "****************************************/\n";
	for (int i = 0; i < MusicKey.numRests; i++) {
		finalFile << "#define " << MusicKey.rests[i].a_str << "  " << (int)MusicKey.rests[i].a_f << '\n';
	}
	finalFile << '\n';
	ifstream tempInFile(tempLocation);
	while (getline(tempInFile, line)) {
		finalFile << line << '\n';
	}
	tempInFile.close();
	finalFile.close();
	std::remove(tempLocation.c_str());


	cout << "Safe to close this terminal";
	cin >> debug;
}

void AddToMusicKey(int flag, string note, int value) {
	float duration = value * fbus / ftempo;
	for (int i = 0; i < MusicKey.numNotes; i++) {
		if (MusicKey.notes[i].a_str == note) {
			return;
		}
	}
	for (int i = 0; i < MusicKey.numRests; i++) {
		if ((MusicKey.rests[i].a_str == note) || (MusicKey.rests[i].a_f == duration)) {
			return;
		}
	}
	switch (flag) {
	case newNote:
		MusicKey.notes[MusicKey.numNotes].a_str = note;
		MusicKey.notes[MusicKey.numNotes].a_f = value;
		MusicKey.numNotes++;
		break;
	case newRest:
		MusicKey.rests[MusicKey.numRests].a_str = note;
		MusicKey.rests[MusicKey.numRests].a_f = duration;
		MusicKey.numRests++;
		break;
	}
	assert((MusicKey.numNotes < MAX_NUMBER_OF_NOTES) && (MusicKey.numRests < MAX_NUMBER_OF_NOTES));
}

void MakeNoteKey(string* outputStr) {

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

