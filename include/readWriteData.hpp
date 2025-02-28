#ifndef READ_WRITE_DATA_HPP
#define READ_WRITE_DATA_HPP

#include <fstream>
#include <iostream>
#include <vector>

// Function to write results to a file
void writeResultsToFile(const std::string& filename, const std::vector<std::string>& results) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }
    for (const std::string& result : results) {
        outFile << result << "\n";
    }
    outFile.close();
}

// Function to read results from a file
std::vector<std::string> readResultsFromFile(const std::string& filename) {
    std::vector<std::string> results;
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return results;
    }
    std::string line;
    while (std::getline(inFile, line))
        results.emplace_back(line);
    inFile.close();
    return results;
}

void writeQueriesToFile(const std::vector<std::string>& queries, const std::string& filePath) {
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << "\n";
    }
    for (const auto& query : queries) {
        outFile << query << "\n";
    }
    outFile.close();
}

#endif
