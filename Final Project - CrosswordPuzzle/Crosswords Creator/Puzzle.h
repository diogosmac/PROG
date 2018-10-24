#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

class Puzzle {
public:
    Puzzle();
    void addWord (string pos, string word);
    void removeWord (string pos);
    map<string, string> getMap();
    vector<string> getWords();
private:    
    map<string, string> pos_word;
};