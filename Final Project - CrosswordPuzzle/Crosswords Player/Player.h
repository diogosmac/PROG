#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>


using namespace std;

class Player {
public:
	Player();
	string getName();
	void updateFileName(string fileName);
	void setStartTime();
	void updateTime();
	void incHints();
	void statsPlayer();
private:
	string name;
	string puzzleFileName;
	unsigned startTime;
	int numbHints;
	unsigned timePlayed;
};