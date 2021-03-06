#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "Board.h"
#include "Dictionary.h"
#include "Player.h"
#include "Puzzle.h"

using namespace std;

// ---------------------------------------------------------

bool validInput(string pos, Board game) {
	if (pos.length() != 3)
		return false;

	char posX, posY, orientation;
	posX = char(int(pos[0] - 'A') + 1);
	posY = char(int(pos[1] - 'a') + 1);
	orientation = pos[2];

	pair<int, int> boardDims = game.getDims();

	if ((posX >= 1 && posX <= boardDims.first) && (posY >= 1 && posY <= boardDims.second) && (orientation == 'V' || orientation == 'H')) {
		return true;
	}
	else
		return false;
}

string getPuzzleFileName() {
	cout << "Board Saved File Name ? ";
	ifstream load;
	string boardFileName;
	while (true) {
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
	load.close();
	cout << '\n';
	return boardFileName;
}

Board loadPuzzle(Player p1, string filename, vector<string> headWords, Dictionary wordList, bool &completionCheck) {
	ifstream load(filename);

	string line;
	int boardWidth = 0, boardHeight = 0, emptyLineCount = 0, lineCount = 0;
	completionCheck = true;
	getline(load, line);														// ignores first line of the file

	while (getline(load, line)) {												// removes any empty lines between the first one and the line that contains the letters
		if (!line.empty())														// used to identify each column of the board
			break;															
	}

	for (unsigned i = 0; i < line.length(); i++) {								// goes through the line with the lower-case board position identifiers, in order to
		if (line[i] != ' ')														// obtain the number of columns in the board
			boardWidth++;
	}

	while (getline(load, line)) {												// goes through each of the following lines of the board, thus getting the number of
		if (line.empty())														// horizontal lines in it, and also finding out whether the board is full or not
			break;
		else {
			boardHeight++;
			for (unsigned i = 0; i < line.length(); i++) {
				if (line[i] == '.')
					completionCheck = false;
			}
		}
	}

	Puzzle loadedPuzzle;
	while (getline(load, line)) {												// goes through the last set of lines in the file, which connects each position to the
		if (!line.empty()) {													// word that starts in it, and extracts that information into the object loadedPuzzle
			istringstream current_pos_word(line);
			string pos, word;
			current_pos_word >> pos >> word;
			loadedPuzzle.addWord(pos, word);
		}
	}

	Board loadedBoard(boardWidth, boardHeight, loadedPuzzle);
	if (completionCheck)
		loadedBoard.boardComplete();
	loadedBoard.setWordList(headWords);

	return loadedBoard;

}

// ---------------------------------------------------------

void introduction() {
	cout << "========================================================================================\n";
	cout << "CROSSWORDS PUZZLE CREATOR\n";
	cout << "========================================================================================\n\n";
	cout << "INSTRUCTIONS:\n\n";
	cout << "Position ( LCD )\n";
	cout << " -> LCD stands for Line, Column and Direction\n";
	cout << "Word ( - = remove / ? = help )\n";
	cout << " -> Typing \"-\" removes the word that starts in the specified position\n";
	cout << " -> \"?\", for help, gives you a hint for the word in the specified position, if available\n\n";
	cout << "========================================================================================\n\n";
}

bool showHint(Player p1, map<string, vector<string>> &hiddenHints, map<string, vector<string>> &shownHints, string pos) {
	for (auto &elem_hidden : hiddenHints) {
		if (pos == elem_hidden.first) {
			if (!elem_hidden.second.empty()) {
				for (auto &elem_shown : shownHints) {
					if (pos == elem_shown.first) {
						srand((unsigned)time(NULL));
						unsigned index = rand() % elem_hidden.second.size();
						elem_shown.second.push_back(elem_hidden.second[index]);
						elem_hidden.second.erase(elem_hidden.second.begin() + index);
						return true;
					}
				}
			}
			else {
				cout << "There are no more hints for the selected word.\n";
			}
		}
	}
	return false;
}

void displayHints(map<string, vector<string>> &hiddenHints, map<string, vector<string>> &shownHints) {
	cout << "VERTICAL : \n\n";
	for (auto elem : shownHints) {
		string pos = elem.first;
		if (pos[2] == 'V') {
			cout << pos << '\t';
			cout << elem.second[0];
			for (unsigned i = 1; i < elem.second.size(); i++) {
				cout << ", " << elem.second[i];
			}
			cout << '\n';
		}
	}
	cout << "\nHORIZONTAL : \n\n";
	for (auto elem : shownHints) {
		string pos = elem.first;
		if (pos[2] == 'H') {
			cout << pos << '\t';
			cout << elem.second[0];
			for (unsigned i = 1; i < elem.second.size(); i++) {
				cout << ", " << elem.second[i];
			}
			cout << '\n';
		}
	}
	cout << '\n';
}

void game_play(Player p1, Board solutionBoard, Board solvingBoard, map<string, vector<string>> hiddenHints, map<string, vector<string>> shownHints);

void solutionCheck(Player p1, Board solutionBoard, Board solvingBoard, map<string, vector<string>> hiddenHints, map<string, vector<string>> shownHints) {
	Board solutionCompare = solvingBoard;
	solutionCompare.boardComplete();
	if (solutionCompare.getMatrix() == solutionBoard.getMatrix()) {
		cout << '\n';
		solutionCompare.show();
		cout << "\nCongratulations, " << p1.getName() << "! You have solved the crosswords puzzle!\n\n";
		p1.statsPlayer();
		system("pause");
		exit(1);
	}
	else {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Oops, something isn't correct! How do you wish to proceed ? \n";
		cout << "To keep trying to solve the puzzle, choose A.\n";
		cout << "To view the solution to the puzzle, choose B.\n";
		cout << "Choose your option : ";
		char option;
		while (true) {
			cin >> option;
			option = toupper(option);
			if (option == 'A' || option == 'B')
				break;
			cout << "Invalid input. Please choose your option correctly.\nChoose your option : ";
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cout << '\n';
		if (option == 'A')
			game_play(p1, solutionBoard, solvingBoard, hiddenHints, shownHints);
		else {
			cout << "This was the correct solution :\n\n";
			solutionBoard.show();
			cout << "\nBetter luck next time!\n\n";
			system("pause");
			exit(1);
		}
	}
}

void game_play(Player p1, Board solutionBoard, Board solvingBoard, map<string, vector<string>> hiddenHints, map<string, vector<string>> shownHints) {
	cout << '\n';
	solvingBoard.show();

	displayHints(hiddenHints, shownHints);

	string pos, word;
	cout << "Position ( LCD / CTRL-Z = finish ) ? "; cin >> pos;

	if (cin.eof()) {
		solutionCheck(p1, solutionBoard, solvingBoard, hiddenHints, shownHints);
	}
	else {
		cout << "Word ( -  = remove / ? = ask for hint ) ? "; cin >> word;

		if (!validInput(pos, solvingBoard) || cin.eof()) {
			cout << "Invalid input. No action taken. Try again!\n\n";
			cin.clear();
			cin.ignore(1000, '\n');
			game_play(p1, solutionBoard, solvingBoard, hiddenHints, shownHints);
		}
		else {
			if (word == "?") {
				if (showHint(p1, hiddenHints, shownHints, pos))
					p1.incHints();
				game_play(p1, solutionBoard, solvingBoard, hiddenHints, shownHints);
			}
			else if (word == "-") {
				solvingBoard.wordRemove(pos);
				game_play(p1, solutionBoard, solvingBoard, hiddenHints, shownHints);
			}
			else {
				solvingBoard.wordAdd(pos, word);
				if (solvingBoard.isComplete()) {
					solutionCheck(p1, solutionBoard, solvingBoard, hiddenHints, shownHints);
				}
				else
					game_play(p1, solutionBoard, solvingBoard, hiddenHints, shownHints);
			}
		}
	}
}

int main()
{
	introduction();
	Player p1;
	ifstream dictFile;
	cout << "Dictionary File Name (xxxx.txt) ? ";
	string inputFileName;
	while (true) {
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
	cout << '\n';
	Dictionary wordList(dictFile);
	vector<string> headWords = wordList.getHeadWords();
	string filename = getPuzzleFileName();
	bool completionCheck = true;
	Board loadedBoard = loadPuzzle(p1, filename, headWords, wordList, completionCheck);
	p1.updateFileName(filename);

	while (true) {
		if (!completionCheck) {
			cout << "Warning! Your loaded puzzle is not complete. How do you wish to proceed ? \n";
			cout << "To automatically complete the puzzle and move on, choose option A.\n";
			cout << "To load another board file, choose option B.\n";
			cout << "Choose your option : ";
			char option;
			while (true) {
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> option;
				option = toupper(option);
				if (option == 'A' || option == 'B')
					break;
				cout << "Invalid input. Please choose your option correctly.\nChoose your option : ";
			}
			cout << '\n';
			if (option == 'A') {
				loadedBoard.boardComplete();
				break;
			}
			else {
				filename = getPuzzleFileName();
				loadedBoard = loadPuzzle(p1, filename, headWords, wordList, completionCheck);
			}
		}
		else
			break;
	}

	pair<unsigned, unsigned> boardDims = loadedBoard.getDims();
	unsigned boardWidth = boardDims.first, boardHeight = boardDims.second;
	Board solvingBoard(boardWidth, boardHeight);
	solvingBoard.lockBlackSquares(loadedBoard.getBlackSquares());
	solvingBoard.setWordList(wordList.getHeadWords());

	map <string, vector<string>> hiddenWordHints;
	map <string, vector<string>> shownWordHints;
	Puzzle boardPuzzle = loadedBoard.getPuzzle();
	for (auto elem : boardPuzzle.getMap()) {
		string pos = elem.first;
		string word = elem.second;
		hiddenWordHints.insert(make_pair(pos, wordList.getSynonyms(word)));
		vector<string> syn;
		shownWordHints.insert(make_pair(pos, syn));
	}

	for (auto &elem_hidden : hiddenWordHints) {
		string pos = elem_hidden.first;
		for (auto &elem_shown : shownWordHints) {
			if (pos == elem_shown.first) {
				srand((unsigned)time(NULL));
				unsigned index = rand() % elem_hidden.second.size();
				elem_shown.second.push_back(elem_hidden.second[index]);
				elem_hidden.second.erase(elem_hidden.second.begin() + index);
			}
		}
	}

	p1.setStartTime();
	game_play(p1, loadedBoard, solvingBoard, hiddenWordHints, shownWordHints);

    return 0;
}