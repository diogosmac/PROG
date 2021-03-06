#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Board.h"
#include "Dictionary.h"
#include "Puzzle.h"

using namespace std;

// -----------------------------------------------------------------
bool validInput(string pos, Board game) {
	if (pos.length() != 3)
		return false;

	char posX, posY, orientation;
	posX = char(int(pos[0] - 'A') + 1);
	posY = char(int(pos[1] - 'a') + 1);
	orientation = pos[2];

	pair<int, int> boardDims = game.getDims();

	if ((posX >= 1 && posX <= boardDims.first) && (posY >= 1 && posY <= boardDims.second)) {
		return true;
	}
	else
		return false;
}

bool boardSaveCheck(Board activeGame) {
	string reply;
	cout << "Do you wish to save the current board ( Y - yes / [anything else] - no ) ? "; cin >> reply;
	if (reply == "Y")
		return true;
	else
		return false;
}
// -----------------------------------------------------------------

void introduction() {
	cout << "========================================================================================\n";
	cout << "CROSSWORDS PUZZLE CREATOR\n";
	cout << "========================================================================================\n\n";
	cout << "INSTRUCTIONS:\n\n";
	cout << "Position ( LCD / CTRL-Z = stop )\n";
	cout << " -> LCD stands for Line, Column and Direction\n";
	cout << " -> Save or finish your Crosswords Puzzle by typing CTRL-Z in the Position field.\n\n";
	cout << "Word ( - = remove / ? = help )\n";
	cout << " -> Typing \"-\" removes the word that starts in the specified position (if there is one).\n";
	cout << " -> \"?\", for help, displays all the words (if any) that fit in the specified position.\n\n";
	cout << "========================================================================================\n\n";
	cout << "OPTIONS:\n";
	cout << "1 - Create puzzle\n2 - Resume puzzle\n0 - Exit\n\n";
}

void displayHelp(string pos, Board b) {
	char posX, posY, orientation;
	posY = char(unsigned(pos[0] - 'A') + 1);
	posX = char(unsigned(pos[1] - 'a') + 1);
	orientation = pos[2];

	vector<string> allWords = b.getHeadWords();
	vector<string> validWords;
	for (unsigned i = 0; i < allWords.size(); i++) {
		string word = allWords[i];
		if (b.wordFitCheck(word, posX, posY, orientation))
			validWords.push_back(word);
	}

	if (validWords.size() == 0)
		cout << "No words fit the specified position.\n\n";
	else {
		cout << "These words fit the specified position:\n\n";
		for (unsigned i = 0; i < validWords.size(); i++) {
			cout << validWords[i] << '\n';
		}
	}
	cout << '\n';
}

void puzzleAdd(Board activeGame) {
	activeGame.show();
	string pos, word;
	cout << "Position ( LCD / CTRL-Z = stop ) ? "; cin >> pos;
	if (cin.eof()) {
		activeGame.boardExport();
		cout << '\n';
		system("pause");
		exit(1);
	}
	cout << "Word ( - = remove / ? = help )   ? "; cin >> word; cout << '\n';
	if (!validInput(pos, activeGame) || cin.eof()) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid input. No action taken. Try again!\n\n";
		puzzleAdd(activeGame);
	}
	else {
		if (word == "?") {
			displayHelp(pos, activeGame);
			puzzleAdd(activeGame);
		}
		else if (word == "-") {
			activeGame.wordRemove(pos);
			puzzleAdd(activeGame);
		}
		else {
			activeGame.wordAdd(pos, word);
			puzzleAdd(activeGame);
		}
	}
}

void newPuzzle(Dictionary wordList) {
	cout << "\n----------------------------------------------\n";
	cout << "CREATE PUZZLE\n";
	cout << "----------------------------------------------\n\n";

	int x, y;
	cout << "Board size (lines columns) ? ";
	while (true) {
		cin >> x >> y;
		if (!cin.fail()) {
			cout << '\n';
			break;
		}
		cout << "Invalid input. Please try again.\n\n";
		cout << "Board size (lines columns) ? ";
		cin.clear();
		cin.ignore(1000, '\n');
	}
	Board game(x, y);
	vector<string> headWords = wordList.getHeadWords();
	game.setWordList(headWords);
	puzzleAdd(game);
}

void loadPuzzle(Dictionary wordList) {
	cout << "\n----------------------------------------------\n";
	cout << "RESUME PUZZLE\n";
	cout << "----------------------------------------------\n\n";

	cout << "Board Saved File Name ? ";
	ifstream load;
	while (true) {
		string boardFileName;
		cin >> boardFileName;
		load.open(boardFileName);
		if (load.is_open()) {
			break;
		}
		cout << "Board file not found. Please insert a valid name.\n";
		cout << "Board Saved File Name ? ";
		load.close();
		load.clear();
		cin.clear();
		cin.ignore(1000, '\n');
	}
	cout << '\n';

	string line;
	int boardWidth = 0, boardHeight = 0, emptyLineCount = 0, lineCount = 0;
	Puzzle loadedPuzzle;
	while (getline(load, line)) {
		lineCount++;

		if (emptyLineCount >= 2) {
			if (!line.empty()) {
				istringstream current_pos_word(line);
				string pos, word;
				current_pos_word >> pos >> word;
				loadedPuzzle.addWord(pos, word);
			}
		}
		else {
			if (lineCount == 4) {
				if (!line.empty()) {
					for (unsigned i = 0; i < line.length(); i++) {
						if (line[i] == '.' || line[i] == '#' || isalpha(line[i]))
							boardWidth++;
					}
				}
			}
			if (emptyLineCount == 1 && !line.empty())
				boardHeight++;
		}

		if (line.empty())
			emptyLineCount++;
	}

	Board loadedBoard(boardWidth, boardHeight, loadedPuzzle);
	vector<string> headWords = wordList.getHeadWords();
	loadedBoard.setWordList(headWords);
	puzzleAdd(loadedBoard);
}

int main() {
	introduction();

	char option;
	cout << "Option ? "; cin >> option;
	while ((option != '0') && (option != '1') && (option != '2')) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid input. Please try again.\n\n";
		cout << "Option ? "; cin >> option;
	}

	if (option == '0') {
		cout << "\nThank you, come again!\n\n";
		system("pause");
		exit(1);
	}

	// open dictionary file
	ifstream dictFile;
	cout << "Dictionary File Name (xxxx.txt) ? ";
	while (true) {
		string inputFileName;
		cin >> inputFileName;
		dictFile.open(inputFileName);
		if (dictFile.is_open()) {
			break;
		}
		cout << "Dictionary file not found. Please insert a valid name.\n";
		cout << "Dictionary File Name (xxxx.txt) ? ";
		dictFile.close();
		dictFile.clear();
		cin.clear();
		cin.ignore(1000, '\n');
	}
	Dictionary wordList(dictFile);
	// -----------------

	if (option == '1') {
		newPuzzle(wordList);
	}
	else {
		loadPuzzle(wordList);
	}

	return 0;
}