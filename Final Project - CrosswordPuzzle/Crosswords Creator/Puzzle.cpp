#include "Puzzle.h"

Puzzle::Puzzle() {
}

void Puzzle::addWord(string pos, string word) {
    pos_word.insert(pair<string, string> (pos, word));
}

void Puzzle::removeWord(string pos){
    pos_word.erase(pos);
}

map<string, string> Puzzle::getMap() {
    return pos_word;
}

vector<string> Puzzle::getWords() {
    vector<string> wordList;
    for (auto elem : pos_word)
        wordList.push_back(elem.second);
	return wordList;
}