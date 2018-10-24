#include "dictionary.h"

void line_Separate(string line, string &headWord, vector<string> &synonyms) {
    int index_start = 0;
    for (unsigned i = 0; i < line.length(); i++) {
        if (line[i] == ':') {
            headWord = line.substr(index_start, i - index_start);
            index_start = i + 2;
        }
        else if (line[i] == ',') {
            synonyms.push_back(line.substr(index_start, i - index_start));
            index_start = i + 2;
        }
        else if (i == line.length() - 1) {
			synonyms.push_back(line.substr(index_start, i - index_start + 1));
        }
    }
}

Dictionary::Dictionary(ifstream &dict_file) {
    string line;
    while (getline(dict_file, line)) {
		string headWord;
		vector<string> synonyms;
        line_Separate(line, headWord, synonyms);
		for (unsigned i = 0; i < headWord.length(); i++)
			headWord[i] = toupper(headWord[i]);
        dict_map.insert(pair<string, vector<string>>(headWord, synonyms));
    }
}

vector<string> Dictionary::getHeadWords() {
	vector<string> headWordList;
	for (auto word : dict_map) {
		string head;
		head = word.first;
		headWordList.push_back(head);
	}
	return headWordList;
}

vector<string> Dictionary::getSynonyms(string mainWord) {
    vector<string> synonyms = dict_map.at(mainWord);
    return synonyms;
}