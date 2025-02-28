#ifndef KMP_HPP
#define KMP_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <algorithm>

class KMP {
private:
    std::unordered_set<std::string> strings;
    std::unordered_map<std::string, int> userPreference;
    int limitWord;

    std::vector<int> computeLPSArray(const std::string& prefix);
    bool KMPSearch(const std::string& prefix, const std::string& text);
    std::vector<int> computeLPSArray(const std::string& prefix, long long &count_comparisons);
    bool KMPSearch(const std::string& prefix, const std::string& text, long long &count_comparisons);

public:
    KMP() : limitWord(10) {}

    void add(const std::string& str) {
        strings.insert(str);
    }

    void remove(const std::string& str) {
        strings.erase(str);
    }

    void getLimitWord() {
        std::cout << "Limit word: " << limitWord << "\n"; 
    }

    void setLimitWord(int limitWord) { 
        this->limitWord = limitWord;
    }

    std::vector<std::string> search(const std::string& prefix);
    std::vector<std::string> search(const std::string& prefix, long long &count_comparisons);
    void readDataFromFile(const std::string& filename);
    
    void userSelected(const std::string& word) {
        userPreference[word]++;
    }
};

#endif