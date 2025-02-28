#include "Trie.hpp"

bool Trie::deleteString(Node* p, const std::string& s, int i) {
    if (i != (int)s.size()) {
        char c = s[i];
        bool isChildDeleted = deleteString(p->child[c], s, i + 1);
        if (isChildDeleted) 
            p->child.erase(c);
    } else
        p->exist--;

    if (p != root) {
        p->cnt--;
        if (p->cnt == 0) {
            delete p;
            return true;
        }
    }
    return false;
}

void Trie::demolish(Node* node) {
    if (node) {
        for (auto& pair : node->child)
            demolish(pair.second);
        delete node;
    }
}

std::vector<std::string> Trie::weightedBfs(Node* startNode, const std::string& prefix) {
    std::vector<std::string> result;
    if (!startNode) {
        std::cerr << "Start node is null\n";
        return result;
    }

    // Custom comparator for the priority queue
    auto comparator = [this](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
        if ((a.first == b.first))
            return (a.second.length() > b.second.length());  // Ascending order by length
        return (a.first < b.first);  // Descending order by weight
    };

    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, decltype(comparator)> pq(comparator);
    std::queue<std::pair<Node*, std::string>> q;

    q.push({startNode, prefix});
    while (!q.empty()) {
        Node* current = q.front().first;
        std::string str = q.front().second;
        q.pop();

        if (!current)
            continue;

        if (current->exist > 0) {
            int weight = current->weight;
            if (userPreference.find(str) != userPreference.end())
                weight += userPreference[str];
            pq.push({weight, str});  // Rank by weight and length
        }

        for (std::unordered_map<char, Node*>::iterator it = current->child.begin(); it != current->child.end(); ++it)
            q.push({it->second, str + it->first});

    }

    while ((!pq.empty()) && (result.size() < limitWord)) {
        result.emplace_back(pq.top().second);
        pq.pop();
    }

    return result;
}

std::vector<std::string> Trie::weightedBfs(Node* startNode, const std::string& prefix, long long &count_comparisons) {
    std::vector<std::string> result;
    if (++count_comparisons && !startNode) {
        std::cerr << "Start node is null\n";
        return result;
    }

    // Custom comparator for the priority queue
    auto comparator = [this, &count_comparisons](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
        if (++count_comparisons && (a.first == b.first))
            return ++count_comparisons && (a.second.length() > b.second.length());  // Ascending order by length
        return ++count_comparisons && (a.first < b.first);  // Descending order by weight
    };

    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, decltype(comparator)> pq(comparator);
    std::queue<std::pair<Node*, std::string>> q;

    q.push({startNode, prefix});
    while (++count_comparisons && !q.empty()) {
        Node* current = q.front().first;
        std::string str = q.front().second;
        q.pop();

        if (++count_comparisons && !current)
            continue;

        if (++count_comparisons && current->exist > 0) {
            int weight = current->weight;
            if (++count_comparisons && userPreference.find(str) != userPreference.end())
                weight += userPreference[str];
            pq.push({weight, str});  // Rank by weight and length
        }

        for (std::unordered_map<char, Node*>::iterator it = current->child.begin(); ++count_comparisons && it != current->child.end(); ++it)
            q.push({it->second, str + it->first});

    }

    while ((++count_comparisons && !pq.empty()) && (++count_comparisons && (result.size() < limitWord))) {
        result.emplace_back(pq.top().second);
        pq.pop();
    }

    return result;
}

void Trie::addString(const std::string& s) {
    Node* p = root;
    for (char c : s) {
        if (!p->child[c]) {
            p->child[c] = new Node();
            if (!p->child[c]) {
                std::cerr << "Cannot allocate new memory for trie\n";
                exit(1);
            }
        }
        p = p->child[c];
        p->cnt++;
    }
    p->exist++;
}

void Trie::deleteString(const std::string& s) {
    if (!findString(s))
        return;
    deleteString(root, s, 0);
}

bool Trie::findString(const std::string& s) {
    Node* p = root;
    for (char c : s) {
        if (!p->child[c])
            return false;
        p = p->child[c];
    }
    return (p->exist != 0);
}

std::vector<std::string> Trie::search(const std::string& prefix) {
    Node* current = root;
    for (int i = 0; i < prefix.length(); i++) {
        if (!current->child[prefix[i]])
            return {};
        current = current->child[prefix[i]];
        current->weight++;
    }

    return weightedBfs(current, prefix);
}

std::vector<std::string> Trie::search(const std::string& prefix, long long &count_comparisons) {
    Node* current = root;
    for (int i = 0; ++count_comparisons && i < prefix.length(); i++) {
        if (++count_comparisons && !current->child[prefix[i]])
            return {};
        current = current->child[prefix[i]];
        current->weight++;
    }

    return weightedBfs(current, prefix, count_comparisons);
}

void Trie::readDataFromFile(const std::string &filePath) {
    std::fstream readBuffer(filePath, std::ios::in);
    if (!readBuffer.is_open()) {
        std::cout << "Can not open file at path: " << filePath << '\n';
        exit(1);
    }
    std::string line;

    while (getline(readBuffer, line)) {
        if (line.empty())
            continue;
        this->addString(line);            
    }
    
    readBuffer.close();
}

void Trie::userSelected(const std::string& word) {
    userPreference[word]++;
    Node* p = root;
    for (char c : word) {
        p = p->child[c];
        if (p)
            p->weight++;
    }
}
