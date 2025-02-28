#include "./model/KMP.hpp"
#include "./model/Trie.hpp"
#include "./include/checker.hpp"
#include "./include/readWriteData.hpp"
#include "./include/chart.hpp"
#include "./include/mergeFiles.hpp"


int main() {

    freopen("./../output.txt", "w+", stdout);

    // UNCOMMENT THIS TO MERGE FILES
    std::string file1 = "./../data/words_alpha.txt";
    std::string file2 = "./../data/English_words.csv";
    std::string outputFile = "./../data/merged_words.txt";
    mergeFiles(file1, file2, outputFile);

    // DRAW CHARTS TO COMPARE TRIE AND KMP
    std::vector<int> limitWords = {1, 5, 10, 15}; 
    compare_Trie_KMP(1000, limitWords);
    compare_Trie_CompressedTrie(1000, limitWords);

    return 0;
}