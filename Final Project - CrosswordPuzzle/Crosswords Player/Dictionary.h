#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

class Dictionary {
public:
    Dictionary(ifstream &dict_file);
	vector<string> getHeadWords();
    vector<string> getSynonyms(string mainWord);
private:
    map<string, vector<string>> dict_map;
};