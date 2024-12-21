#ifndef MERGE_FILES_HPP
#define MERGE_FILES_HPP

#include <iostream>
#include <fstream>
#include <set>
#include <string>

void mergeFiles(const std::string& file1, const std::string& file2, const std::string& outputFile) {
    std::set<std::string> words;
    std::ifstream infile;
    std::ofstream outfile(outputFile);

    // Lambda function to check if a file is a CSV
    auto isCSV = [](const std::string& filename) {
        return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv";
    };

    // Function to read words from a file
    auto readWordsFromFile = [&](const std::string& filename) {
        infile.open(filename);
        if (infile.is_open()) {
            std::string word;
            bool isFirstLine = isCSV(filename);
            while (std::getline(infile, word)) {
                if (isFirstLine) {
                    isFirstLine = false;
                    continue;
                }
                words.insert(word);
            }
            infile.close();
        } else
            std::cerr << "Unable to open file: " << filename << '\n';
    };

    readWordsFromFile(file1);
    readWordsFromFile(file2);

    // Write unique words to the output file
    if (outfile.is_open()) {
        for (const auto& word : words)
            outfile << word << '\n';
        outfile.close();
    } else
        std::cerr << "Unable to open output file: " << outputFile << '\n';
}

#endif