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

#endif