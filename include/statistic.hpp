#ifndef STATISTIC_HPP
#define STATISTIC_HPP

#include <vector>
#include <algorithm>
#include <cmath>

// Function to calculate mean
template <typename T>
double calculateMean(const std::vector<T>& data) {
    double sum = 0.0;
    for (T value : data)
        sum += value;
    return sum / data.size();
}

// Function to calculate standard deviation
template <typename T>
double calculateStdDev(const std::vector<T>& data, double mean) {
    double sum = 0.0;
    for (T value : data)
        sum += (value - mean) * (value - mean);
    return std::sqrt(sum / data.size());
}

// Function to calculate median
template <typename T>
double calculateMedian(std::vector<T> data) {
    std::sort(data.begin(), data.end());
    size_t size = data.size();
    if (size % 2 == 0) {
        return (data[size / 2 - 1] + data[size / 2]) / 2;
    } else {
        return data[size / 2];
    }
}

// Function to calculate p-value for one-tailed t-test comparing if mean of sample1 is greater than mean of sample2
template <typename T>
double calculatePValue(const std::vector<T>& sample1, const std::vector<T>& sample2) {
    if (sample1.size() < 2 || sample2.size() < 2) {
        throw std::invalid_argument("Samples must have at least two values each.");
    }

    double mean1 = calculateMean(sample1);
    double mean2 = calculateMean(sample2);
    double stdDev1 = calculateStdDev(sample1, mean1);
    double stdDev2 = calculateStdDev(sample2, mean2);

    double pooledStdDev = std::sqrt(((stdDev1 * stdDev1) / sample1.size()) + ((stdDev2 * stdDev2) / sample2.size()));
    double tStatistic = (mean1 - mean2) / pooledStdDev;

    // Calculate p-value using the cumulative distribution function (CDF) of the t-distribution
    long double pValue = 1 - std::erf(std::abs(tStatistic) / std::sqrt(2));

    return pValue;
}

#endif