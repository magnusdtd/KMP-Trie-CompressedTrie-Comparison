#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <iomanip>
#include <functional>
#include <chrono>
#include <vector>
#include "./statistic.hpp"

// Function to measure execution time
template <typename Func, typename... Args>
void measureExecutionTime(Func&& func, double& elapsedTime, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    func(std::forward<Args>(args)...); // Execute the function

    auto end = std::chrono::high_resolution_clock::now();
    elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
}

// Function to count comparisons
template <typename Func, typename... Args>
void measureComparisonCount(Func&& func, long long& comparisonCount, Args&&... args) {
    comparisonCount = 0;

    func(std::forward<Args>(args)..., comparisonCount); // Execute the function
}

// Template function to measure time and memory usage
template <typename T>
void measurePerformance(const std::vector<std::string>& queries, const std::string& inputFile, const std::string& outputFile,
                        std::vector<double>& times, std::vector<long long>& comparisonCounts, const int& limitWord) {
    T *instance;
    instance = new T;
    instance->readDataFromFile(inputFile);
    instance->setLimitWord(limitWord);
    instance->getLimitWord();

    std::ofstream ofs(outputFile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    for (const auto& query : queries) {
        std::vector<std::string> suggestions;
        double elapsedTime = 0;
        long long comparisonCount = 0;

        auto searchFunc = [&]() {
            suggestions = instance->search(query);
        };

        auto searchFuncWithCountComparisons = [&](long long& comparisonCount) {
            suggestions = instance->search(query, comparisonCount);
        };

        measureExecutionTime(searchFunc, elapsedTime);
        measureComparisonCount(searchFuncWithCountComparisons, comparisonCount);

        times.emplace_back(elapsedTime);
        comparisonCounts.emplace_back(comparisonCount);

        std::ofstream outputBuffer(outputFile, std::ios_base::app);
        outputBuffer << "===========================================================\n";
        for (const auto& suggestion : suggestions) {
            outputBuffer << suggestion << "\n";
            instance->userSelected(suggestion);
        }
    }

    delete instance;

    // Calculate and print mean and standard deviation
    double meanTime = calculateMean(times);
    double stdDevTime = calculateStdDev(times, meanTime);
    double meanComparisons = calculateMean(comparisonCounts);
    double stdDevComparisons = calculateStdDev(comparisonCounts, meanComparisons);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Algorithm: " << typeid(T).name() << '\n';
    std::cout << "Mean Time: " << meanTime << " seconds" << '\n';
    std::cout << "Standard Deviation of Time: " << stdDevTime << " seconds" << '\n';
    std::cout << "Mean Comparisons: " << meanComparisons << '\n';
    std::cout << "Standard Deviation of Comparisons: " << stdDevComparisons << '\n';
    std::cout << "===========================================================\n";
}

// Function to compare results and write differences to a file
void compareResults(const std::string& filePath, const std::vector<std::string>& results1, const std::vector<std::string>& results2) {
    std::vector<std::string> sortedResults1 = results1;
    std::vector<std::string> sortedResults2 = results2;
    
    std::sort(sortedResults1.begin(), sortedResults1.end());
    std::sort(sortedResults2.begin(), sortedResults2.end());
    
    std::ofstream readBuffer(filePath);
    if (!readBuffer.is_open()) {
        std::cerr << "Cannot open file " << filePath << "\n";
        return;
    }
    
    if (sortedResults1 == sortedResults2) {
        readBuffer << "No difference between two files (not considering order)" << std::endl;
    } else {
        readBuffer << "Differences between results:" << std::endl;
        for (size_t i = 0; i < std::max(sortedResults1.size(), sortedResults2.size()); ++i) {
            if (i < sortedResults1.size() && i < sortedResults2.size()) {
                if (sortedResults1[i] != sortedResults2[i]) {
                    readBuffer << "Line " << i + 1 << " differs: " << sortedResults1[i] << " vs " << sortedResults2[i] << std::endl;
                }
            } else if (i < sortedResults1.size()) {
                readBuffer << "Line " << i + 1 << " only in first file: " << sortedResults1[i] << std::endl;
            } else {
                readBuffer << "Line " << i + 1 << " only in second file: " << sortedResults2[i] << std::endl;
            }
        }
    }
    
    readBuffer.close();
}

#endif