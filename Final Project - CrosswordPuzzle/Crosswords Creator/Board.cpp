#include "Board.h"

Board::Board(unsigned x, unsigned y) {
	dim_X = x;
	dim_Y = y;

	vector<char> columnMarkers;
	columnMarkers.push_back(' ');
	for (unsigned i = 1; i <= dim_Y; i++) {
		char c = char(i + 64);
		columnMarkers.push_back(c);
	}
	matrix.push_back(columnMarkers);

	for (unsigned i = 1; i <= dim_X; i++) {
		vector<char> generic;
		generic.push_back(i + 96);
		for (unsigned j = 1; j <= dim_Y; j++) {
			generic.push_back('.');
		}
		matrix.push_back(generic);
	}
}

Board::Board(unsigned x, unsigned y, Puzzle gameKey) {
	dim_X = x;
	dim_Y = y;

	vector<char> columnMarkers;
	columnMarkers.push_back(' ');
	for (unsigned i = 1; i <= dim_Y; i++) {
		char c = char(i + 64);
		columnMarkers.push_back(c);
	}
	matrix.push_back(columnMarkers);

	for (unsigned i = 1; i <= dim_X; i++) {
		vector<char> generic;
		generic.push_back(i + 96);
		for (unsigned j = 1; j <= dim_Y; j++) {
			generic.push_back('.');
		}
		matrix.push_back(generic);
	}

	boardKey = gameKey;
	boardUpdate_v1();
}

void Board::setWordList(vector<string> wordList) {
	headWordList = wordList;
}

pair<unsigned, unsigned> Board::getDims() {
	return make_pair(dim_X, dim_Y);
}

vector<vector<char>> Board::getMatrix() {
	return matrix;
}

vector<string> Board::getHeadWords() {
	return headWordList;
}

Puzzle Board::getPuzzle() {
	return boardKey;
}

void Board::show() {
	setcolor(LIGHTRED);
	for (unsigned j = 0; j <= dim_X; j++) {
		cout << matrix[j][0] << "  ";
	}
	cout << '\n';
	for (unsigned i = 1; i <= dim_Y; i++) {
		setcolor(LIGHTRED);
		cout << matrix[0][i] << " ";
		for (unsigned j = 1; j <= dim_X; j++) {
			if (matrix[j][i] == '#') {
				setcolor(LIGHTGRAY, BLACK);
				cout << " " << matrix[j][i] << " ";
			}
			else {
				setcolor(BLACK, LIGHTGRAY);
				cout << " " << matrix[j][i] << " ";
			}
		}
		setcolor(WHITE, BLACK);						// important only on fullscreen
		cout << '\n';
	}
	cout << '\n';
}

bool Board::wordFound(string word) {
	bool check = false;
	for (unsigned i = 0; i < headWordList.size(); i++) {
		string checkWord = headWordList[i];
		for (unsigned i = 0; i < checkWord.length(); i++) {
			checkWord[i] = toupper(checkWord[i]);
		}
		if (word == checkWord)
			check = true;
	}
	return check;
}

bool Board::wordUnique(string word) {
	vector<string> insertedWords = boardKey.getWords();
	for (unsigned i = 0; i < insertedWords.size(); i++) {
		if (word == insertedWords[i])
			return false;
	}
	return true;
}

bool Board::wordFitCheck(string word, unsigned posX, unsigned posY, char orientation) {
	if (!wordUnique(word))
		return false;

	string newWord = word;
	string boardSpace_string = "";
	if (orientation == 'V') {
		if (posY > 1) {
			boardSpace_string += matrix[posX][posY - 1];
			newWord = '#' + word;
		}
		for (unsigned i = 0; i <= dim_Y - posY; i++) {
			if (matrix[posX][posY + i] != '#')
				boardSpace_string += matrix[posX][posY + i];
			else
				break;
		}
		if (posY + word.length() - 1 < dim_Y) {
			boardSpace_string += matrix[posX][posY + word.length()];
			newWord += '#';
		}
	}
	else if (orientation == 'H') {
		if (posX > 1) {
			boardSpace_string += matrix[posX - 1][posY];
			newWord = '#' + newWord;
		}
		for (unsigned i = 0; i <= dim_X - posX; i++) {
			if (matrix[posX + i][posY] != '#')
				boardSpace_string += matrix[posX + i][posY];
			else
				break;
		}
		if (posX + word.length() - 1 < dim_X) {
			boardSpace_string += matrix[posX + word.length()][posY];
			newWord += '#';
		}
	}
	else
		return false;

	for (unsigned i = 0; i < newWord.length(); i++) {
		if (boardSpace_string[i] != newWord[i] && boardSpace_string[i] != '.')
			return false;
	}

	return true;
}

void Board::wordAdd(string pos, string word) {

	unsigned posX, posY;
	char orientation;
	posY = (unsigned(pos[0] - 'A') + 1);
	posX = (unsigned(pos[1] - 'a') + 1);
	orientation = pos[2];

	for (unsigned i = 0; i < word.length(); i++) {
		word[i] = toupper(word[i]);
	}

	if (!wordFound(word))
		cout << "ERROR: The inserted word is not in the dictionary.\n\n";
	else if (!wordUnique(word))
		cout << "ERROR: The inserted word is already on the board.\n\n";
	else if (!wordFitCheck(word, posX, posY, orientation))
		cout << "ERROR: The inserted word doesn't fit in the specified position.\n\n";
	else {
		boardKey.addWord(pos, word);
		boardUpdate_v1();
	}
}

void Board::wordFill(string pos, string word) {
	char posX, posY, orientation;
	posY = char(unsigned(pos[0] - 'A') + 1);
	posX = char(unsigned(pos[1] - 'a') + 1);
	orientation = pos[2];

	if (orientation == 'V') {
		if (posY > 1) {
			matrix[posX][posY - 1] = '#';
		}
		if (posY + word.length() - 1 < dim_Y) {
			matrix[posX][posY + word.length()] = '#';
		}
		for (unsigned i = 0; i < word.length(); i++) {
			matrix[posX][posY + i] = word[i];
		}
	}
	else if (orientation == 'H') {
		if (posX > 1) {
			matrix[posX - 1][posY] = '#';
		}
		if (posX + word.length() - 1 < dim_X) {
			matrix[posX + word.length()][posY] = '#';
		}
		for (unsigned i = 0; i < word.length(); i++) {
			matrix[posX + i][posY] = word[i];
		}
	}
}

void Board::wordRemove(string pos) {
	boardKey.removeWord(pos);
	boardUpdate_v1();
}

void Board::boardReset() {
	vector<vector<char>> cleanBoard;

	vector<char> columnMarkers;
	columnMarkers.push_back(' ');
	for (unsigned i = 1; i <= dim_Y; i++) {
		char c = char(i + 64);
		columnMarkers.push_back(c);
	}
	cleanBoard.push_back(columnMarkers);

	for (unsigned i = 1; i <= dim_X; i++) {
		vector<char> generic;
		generic.push_back(i + 96);
		for (unsigned j = 1; j <= dim_Y; j++) {
			generic.push_back('.');
		}
		cleanBoard.push_back(generic);
	}

	matrix = cleanBoard;
}

void Board::boardComplete() {
	for (unsigned i = 1; i <= dim_Y; i++) {
		for (unsigned j = 1; j <= dim_X; j++) {
			if (matrix[j][i] == '.')
				matrix[j][i] = '#';
		}
	}
}

void Board::boardExport() {
	cin.clear();
	cout << "How do you wish to proceed ?\nOption A: Save your board, so you can finish it later.\nOption B: Finish your board.\nChoose your option : ";
	while (true) {
		char option;
		cin >> option;
		if (toupper(option) == 'A' || toupper(option) == 'B') {
			if (toupper(option) == 'B')
				boardComplete();
			break;
		}
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Please choose a valid option : ";
	}

	ostringstream outputName;
	for (unsigned i = 1; i < 1000; i++) {
		outputName << "b";
		outputName << fixed << setfill('0');
		outputName << setw(3) << i;
		outputName << ".txt";
		ifstream fileNameTest(outputName.str());
		if (fileNameTest.is_open()) {
			fileNameTest.close();
			outputName.str("");
			outputName.clear();
		}
		else
			break;
	}

	ofstream saveBoard(outputName.str());

	// -----------------------------------------------

	saveBoard << outputName.str() << "\n\n";
	for (unsigned i = 0; i <= dim_Y; i++) {
		for (unsigned j = 0; j <= dim_X; j++) {
			saveBoard << matrix[j][i] << "  ";
		}
		saveBoard << '\n';
	}
	saveBoard << "\n\n";

	map<string, string> wl = boardKey.getMap();
	for (auto elem : wl) {
		string pos, word;
		pos = elem.first;
		word = elem.second;
		saveBoard << pos << " " << word << '\n';
	}

	saveBoard.close();
	cout << "Saved board to " << outputName.str() << "\n\n";
}

void Board::boardUpdate_v1() {
	boardReset();
	map<string, string> wl = boardKey.getMap();
	for (auto elem : wl) {
		string pos, word;
		pos = elem.first;
		word = elem.second;
		wordFill(pos, word);
	}
}