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

#endif
