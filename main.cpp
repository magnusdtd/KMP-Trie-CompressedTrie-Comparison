#include "./model/KMP.hpp"
#include "./model/Trie.hpp"
#include "./include/checker.hpp"
#include "./include/readWriteData.hpp"
#include "./include/chart.hpp"
#include "./include/mergeFiles.hpp"


int main() {

    freopen("./../input.txt", "r+", stdin);
    freopen("./../output.txt", "w+", stdout);

    // // UNCOMMENT THIS TO MERGE FILES
    // std::string file1 = "./../data/words_alpha.txt";
    // std::string file2 = "./../data/English_words.csv";
    // std::string outputFile = "./../data/merged_words.txt";
    // mergeFiles(file1, file2, outputFile);

    // DRAW CHARTS TO COMPARE TRIE AND KMP
    // std::vector<int> limitWords = {1};
    std::vector<int> limitWords = {416318}; 
    // compare_Trie_KMP(100, limitWords);
    compare_Trie_CompressedTrie(10000, limitWords);

    return 0;
}