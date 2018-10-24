#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "Colors.h"
#include "Puzzle.h"
#include "Dictionary.h"

using namespace std;

class Board {
public:
	Board(unsigned x, unsigned y);
	Board(unsigned x, unsigned y, Puzzle gameKey);
	void setWordList(vector<string> wordList);
	pair<unsigned, unsigned> getDims();
	vector<vector<char>> getMatrix();
	vector<string> getHeadWords();
	Puzzle getPuzzle();
	void show();
	bool wordFound(string word);
	bool wordUnique(string word);
	bool wordFitCheck(string word, unsigned posX, unsigned posY, char orientation);
	void wordAdd(string pos, string word);
	void wordFill(string pos, string word);
	void wordRemove(string pos);
	void boardReset();
	void boardUpdate_v1();
	void boardComplete();
	void boardExport();

	string getPosition(string word);
	vector<pair<unsigned, unsigned>> getBlackSquares();
	void lockBlackSquares(vector<pair<unsigned, unsigned>> blackSquares);
	bool isComplete();
private:
	unsigned dim_X, dim_Y;
	vector<vector<char>> matrix;
	Puzzle boardKey;
	vector<string> headWordList;
};