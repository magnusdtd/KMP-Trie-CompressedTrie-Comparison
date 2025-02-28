#include "KMP.hpp"

std::vector<int> KMP::computeLPSArray(const std::string &prefix) {
    int M = prefix.size();
    std::vector<int> lps(M);
    int len = 0, i = 1;
    lps[0] = 0;

    while (i < M) {
        if (prefix[i] == prefix[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

std::vector<int> KMP::computeLPSArray(const std::string &prefix, long long &count_comparisons) {
    int M = prefix.size();
    std::vector<int> lps(M);
    int len = 0, i = 1;
    lps[0] = 0;

    while (++count_comparisons && i < M) {
        if (++count_comparisons && prefix[i] == prefix[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (++count_comparisons && len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

bool KMP::KMPSearch(const std::string &prefix, const std::string &text) {
    int M = prefix.size(), N = text.size();

    std::vector<int> lps = computeLPSArray(prefix);

    int i = 0; // index for text
    int j = 0; // index for prefix
    while (i < N) {
        if (prefix[j] == text[i]) {
            j++;
            i++;
        }

        if (j == M) {
            j = lps[j - 1];
            return true; // Found prefix at index i - j
        } else if (i < N && prefix[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return false;
}

bool KMP::KMPSearch(const std::string &prefix, const std::string &text, long long &count_comparisons) {
    int M = prefix.size(), N = text.size();

    std::vector<int> lps = computeLPSArray(prefix, count_comparisons);

    int i = 0; // index for text
    int j = 0; // index for prefix
    while (++count_comparisons && i < N) {
        if (++count_comparisons && prefix[j] == text[i]) {
            j++;
            i++;
        }

        if (++count_comparisons && j == M) {
            j = lps[j - 1];
            return true; // Found prefix at index i - j
        } else if ((++count_comparisons && i < N) && (++count_comparisons && prefix[j] != text[i])) {
            if (++count_comparisons && j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return false;
}

std::vector<std::string> KMP::search(const std::string& prefix) {
    std::vector<std::string> matches;
    for (std::unordered_set<std::string>::iterator it = strings.begin(); it != strings.end(); it++) {
        if (KMPSearch(prefix, *it)) {
            matches.emplace_back(*it);
            if (matches.size() >= limitWord) {
                break; // Early pause when enough words are found
            }
        }
    }

    std::sort(matches.begin(), matches.end(), [this](const std::string& a, const std::string& b) {
        if (userPreference[a] == userPreference[b]) {
            return a.length() < b.length();  // Ascending order by length
        }
        return userPreference[a] > userPreference[b];  // Descending order by user preference
    });

    if (matches.size() > limitWord) {
        matches.resize(limitWord);
    }

    return matches;
}

std::vector<std::string> KMP::search(const std::string& prefix, long long &count_comparisons) {
    std::vector<std::string> matches;
    for (std::unordered_set<std::string>::iterator it = strings.begin(); ++count_comparisons && it != strings.end(); it++) {
        if (++count_comparisons && KMPSearch(prefix, *it, count_comparisons)) {
            matches.emplace_back(*it);
            if (++count_comparisons && matches.size() >= limitWord) {
                break; // Early pause when enough words are found
            }
        }
    }

    std::sort(matches.begin(), matches.end(), [this, &count_comparisons](const std::string& a, const std::string& b) {
        if (++count_comparisons && userPreference[a] == userPreference[b]) {
            return ++count_comparisons && a.length() < b.length();  // Ascending order by length
        }
        return ++count_comparisons && userPreference[a] > userPreference[b];  // Descending order by user preference
    });

    if (matches.size() > limitWord) {
        matches.resize(limitWord);
    }

    return matches;
}

void KMP::readDataFromFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
        add(line);
}
