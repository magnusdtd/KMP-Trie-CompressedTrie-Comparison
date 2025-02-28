#ifndef TRIE_HPP
#define TRIE_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <chrono>

class Trie {
    class Node {
        std::unordered_map<char, Node*> child;
        int exist, cnt;
        int weight;  // Weight for ranking suggestions

    public:
        Node() : exist(0), cnt(0), weight(0) {}

        friend class Trie;
    };

    Node* root;
    int limitWord;
    std::unordered_map<std::string, int> userPreference;  // Track user behavior

    bool deleteString(Node* p, const std::string& s, int i);

    void demolish(Node* node);

   std::vector<std::string> weightedBfs(Node* startNode, const std::string& prefix);
   std::vector<std::string> weightedBfs(Node* startNode, const std::string& prefix, long long &count_comparisons);

public:

    Trie() : root(new Node()), limitWord(466550) {}

    Trie(int limitWord) : root(new Node()), limitWord(limitWord) {}

    ~Trie() { demolish(root); }

    void setLimitWord(int limitWord) { 
        this->limitWord = limitWord; 
    }

    void getLimitWord() {
        std::cout << "Limit word: " << limitWord << "\n"; 
    }
    
    void addString(const std::string& s);

    void deleteString(const std::string& s);

    bool findString(const std::string& s);

    std::vector<std::string> search(const std::string& prefix);
    std::vector<std::string> search(const std::string& prefix, long long &count_comparisons);

    void readDataFromFile(const std::string &filePath);

    void userSelected(const std::string& word);

};

#endif