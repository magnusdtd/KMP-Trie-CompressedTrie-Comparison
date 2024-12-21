#ifndef CHART_HPP
#define CHART_HPP

#include "./matplotlibcpp.hpp"
#include "./statistic.hpp"
#include "./../model/Trie.hpp"
#include "./../model/CompressedTrie.hpp"
#include <random>
#include <map>

#define DATA_FILE_PATH "../data/merged_words.txt"


namespace plt = matplotlibcpp;

void draw_elapsed_time_chart(const int &dataSize, const int &limitWord, std::vector<std::string> algorithms, std::vector<std::vector<double>> allTimes, const std::string& filePath) {
    plt::figure_size(2400, 1200);
    std::map<std::string, std::string> keywords;
    plt::suptitle("Elapsed Time Comparison of Trie and KMP in " + std::to_string(dataSize) + " times\nLimit Word: " + std::to_string(limitWord), {{"fontsize", "26"}});
    for (size_t i = 0; i < algorithms.size(); ++i) {
        std::string algorithm = algorithms[i];

        // Subplot for Time
        plt::subplot(1, algorithms.size(), i + 1);
        plt::title("Elapsed Time for " + algorithm, {{"fontsize", "22"}});
        
        keywords = {{"label", "Time"}};
        plt::plot(allTimes[i], keywords);
        plt::xlabel("Query Index", {{"fontsize", "18"}});
        plt::ylabel("Time (seconds)", {{"fontsize", "18"}});

        // Mean line for Time
        double meanTime = calculateMean(allTimes[i]);
        double stdDev = calculateStdDev(allTimes[i], meanTime);
        std::vector<double> meanTimeLine(allTimes[i].size(), meanTime);
        keywords = {{"label", "Mean: " + std::to_string(meanTime) + "\n(Std: " + std::to_string(stdDev) + ")"}};
        plt::plot(meanTimeLine, keywords);

        // Median line for Time
        double medianTime = calculateMedian(allTimes[i]);
        std::vector<double> medianTimeLine(allTimes[i].size(), medianTime);
        keywords = {{"label", "Median: " + std::to_string(medianTime)}};
        plt::plot(medianTimeLine, keywords);

        // Add legend with increased font size
        plt::legend({{"fontsize", "18"}});
    }

    plt::save(filePath);
    plt::close();
}

void draw_comparison_counts_chart(const int &dataSize, const int &limitWord, std::vector<std::string> algorithms, std::vector<std::vector<long long>> allComparisonCounts, const std::string& filePath) {
    plt::figure_size(2400, 1200);
    std::map<std::string, std::string> keywords;
    plt::suptitle("Comparison Counts of Trie and KMP in " + std::to_string(dataSize) + " times\nLimit Word: " + std::to_string(limitWord), {{"fontsize", "26"}});
    for (size_t i = 0; i < algorithms.size(); ++i) {
        std::string algorithm = algorithms[i];

        // Subplot for Comparison Counts
        plt::subplot(1, algorithms.size(), i + 1);
        plt::title("Comparison Counts for " + algorithm, {{"fontsize", "22"}});
        keywords = {{"label", "Comparison Counts"}};
        plt::plot(allComparisonCounts[i], keywords);
        plt::xlabel("Query Index", {{"fontsize", "18"}});
        plt::ylabel("Comparison Counts", {{"fontsize", "18"}});

        // Mean line for Comparison Counts
        double meanComparisons = calculateMean(allComparisonCounts[i]);
        double stdDev = calculateStdDev(allComparisonCounts[i], meanComparisons);
        std::vector<double> meanComparisonsLine(allComparisonCounts[i].size(), meanComparisons);
        keywords = {{"label", "Mean: " + std::to_string(meanComparisons) + "\n(Std: " + std::to_string(stdDev) + ")"}};
        plt::plot(meanComparisonsLine, keywords);

        // Median line for Comparison Counts
        double medianComparisons = calculateMedian(allComparisonCounts[i]);
        std::vector<double> medianComparisonsLine(allComparisonCounts[i].size(), medianComparisons);
        keywords = {{"label", "Median: " + std::to_string(medianComparisons)}};
        plt::plot(medianComparisonsLine, keywords);

        // Add legend with increased font size
        plt::legend({{"fontsize", "18"}});
    }

    plt::save(filePath);
    plt::close();
}

void compare_Trie_KMP(const int& dataSize, std::vector<int> limitWords) {

    // Read words from file
    std::vector<std::string> words;
    std::ifstream file(DATA_FILE_PATH);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            words.emplace_back(word);
        }
    }

    // Generate random queries
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
 

    for (int limitWord : limitWords) {
        std::vector<double> timesTrie, timesKMP;
        std::vector<long long> comparisonCountsTrie, comparisonCountsKMP;
        std::vector<std::string> Trie_Results, KMP_Results;

        std::vector<std::string> queries;
        std::uniform_int_distribution<int> distribution(0, words.size() - 1);

        // Ensure that the generated queries result in the algorithm 
        // returning a number of results greater than or equal to the limitWord
        for (int i = 0; i < dataSize; ++i) {
            std::string query;
            do {
                query = words[distribution(generator)];
            } while (std::count_if(words.begin(), words.end(), [&query](const std::string& word) {
                return word.find(query) == 0;
            }) < *std::max_element(limitWords.begin(), limitWords.end()));
            queries.emplace_back(query);
        }

        // Measure performance for Trie
        measurePerformance<Trie>(queries, DATA_FILE_PATH, "../result/Trie_result_" + std::to_string(limitWord) + ".txt", timesTrie, comparisonCountsTrie, limitWord);

        // Measure performance for KMP
        measurePerformance<KMP>(queries, DATA_FILE_PATH, "../result/KMP_result_" + std::to_string(limitWord) + ".txt", timesKMP, comparisonCountsKMP, limitWord);

        // Plotting result for Trie vs KMP
        std::vector<std::string> algorithmsTrieKMP = {"Trie", "KMP"};
        std::vector<std::vector<double>> allTimesTrieKMP = {timesTrie, timesKMP};
        std::vector<std::vector<long long>> allComparisonCountsTrieKMP = {comparisonCountsTrie, comparisonCountsKMP};
        draw_elapsed_time_chart(dataSize, limitWord, algorithmsTrieKMP, allTimesTrieKMP, "./../img/Trie_KMP_elapsedtime_" + std::to_string(limitWord) + ".png");
        draw_comparison_counts_chart(dataSize, limitWord, algorithmsTrieKMP, allComparisonCountsTrieKMP, "./../img/Trie_KMP_comparison_counts_" + std::to_string(limitWord) + ".png");
    }
}

void compare_Trie_CompressedTrie(const int& dataSize, std::vector<int> limitWords) {
    // Read words from file
    std::vector<std::string> words;
    std::ifstream file(DATA_FILE_PATH);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            words.emplace_back(word);
        }
    }

    // Generate random queries
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    for (int limitWord : limitWords) {
        std::vector<double> timesTrie, timesCompressedTrie;
        std::vector<long long> comparisonCountsTrie, comparisonCountsCompressedTrie;
        std::vector<std::string> Trie_Results, CompressedTrie_Results;

        std::vector<std::string> queries;
        std::uniform_int_distribution<int> distribution(0, words.size() - 1);


        std::unordered_set<int> selectedIndices;
        for (int i = 0; i < dataSize; ++i) {
            std::string query;
            int index;
            do {
                index = distribution(generator);
            } while (selectedIndices.find(index) != selectedIndices.end());
            selectedIndices.insert(index);
            query = words[index];
            queries.emplace_back(query);
        }

        // Measure performance for Trie
        measurePerformance<Trie>(queries, DATA_FILE_PATH, "../result/Trie_result_" + std::to_string(limitWord) + ".txt", timesTrie, comparisonCountsTrie, limitWord);

        // Measure performance for Compressed Trie
        measurePerformance<CompressedTrie>(queries, DATA_FILE_PATH, "../result/CompressedTrie_result_" + std::to_string(limitWord) + ".txt", timesCompressedTrie, comparisonCountsCompressedTrie, limitWord);

        // Plotting result for Trie vs Compressed Trie
        std::vector<std::string> algorithmsTrieCompressedTrie = {"Trie", "Compressed Trie"};
        std::vector<std::vector<double>> allTimesTrieCompressedTrie = {timesTrie, timesCompressedTrie};
        std::vector<std::vector<long long>> allComparisonCountsTrieCompressedTrie = {comparisonCountsTrie, comparisonCountsCompressedTrie};
        draw_elapsed_time_chart(dataSize, limitWord, algorithmsTrieCompressedTrie, allTimesTrieCompressedTrie, "./../img/Trie_CompressedTrie_elapsedtime_" + std::to_string(limitWord) + ".png");
        draw_comparison_counts_chart(dataSize, limitWord, algorithmsTrieCompressedTrie, allComparisonCountsTrieCompressedTrie, "./../img/Trie_CompressedTrie_comparison_counts_" + std::to_string(limitWord) + ".png");
    }
}


#endif