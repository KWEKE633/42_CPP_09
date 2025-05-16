#include "PmergeMe.hpp"
#include <algorithm>
#include <cmath>

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other){
    (void)(other);
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this == &other) {
        return *this; 
    }
    return *this;
}

// ヤコビアン数列のインデックスを生成
std::vector<size_t> PmergeMe::generateJacobsthalIndices(size_t size) {
    std::vector<size_t> indices;
    size_t j1 = 0;
    size_t j2 = 1;
    while (j2 < size) {
        indices.push_back(j2);
        size_t temp = j2;
        j2 = j2 + 2 * j1;
        j1 = temp;
    }
    return indices;
}

// std::vector に対する Ford-Johnson ソート
void PmergeMe::sortVector(std::vector<int>& sequence) {
    fordJohnsonSort(sequence);
}

// std::list に対する Ford-Johnson ソート
void PmergeMe::sortList(std::list<int>& sequence) {
    fordJohnsonSort(sequence);
}

// std::vector 用の Ford-Johnson ソート実装
void PmergeMe::fordJohnsonSort(std::vector<int>& sequence) {
    if (sequence.size() <= 1)
        return;

    // ペアリング
    std::vector<std::pair<int, int> > pairs;
    size_t i = 0;
    for (; i + 1 < sequence.size(); i += 2) {
        int a = sequence[i];
        int b = sequence[i + 1];
        if (a < b)
            std::swap(a, b);
        pairs.push_back(std::make_pair(a, b));
    }
    int last = -1;
    if (i < sequence.size())
        last = sequence[i];

    // メインチェーンの構築
    std::vector<int> mainChain;
    for (size_t j = 0; j < pairs.size(); ++j)
        mainChain.push_back(pairs[j].first);
    fordJohnsonSort(mainChain);

    // ペンディング要素の収集
    std::vector<int> pend;
    for (size_t j = 0; j < pairs.size(); ++j)
        pend.push_back(pairs[j].second);
    if (last != -1)
        pend.push_back(last);

    // ヤコビアン数列に従って挿入
    std::vector<size_t> indices = generateJacobsthalIndices(pend.size());
    std::vector<bool> inserted(pend.size(), false);
    for (size_t k = 0; k < indices.size(); ++k) {
        size_t idx = indices[k];
        if (idx >= pend.size())
            continue;
        int value = pend[idx];
        std::vector<int>::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), value);
        mainChain.insert(it, value);
        inserted[idx] = true;
    }
    for (size_t j = 0; j < pend.size(); ++j) {
        if (!inserted[j]) {
            int value = pend[j];
            std::vector<int>::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), value);
            mainChain.insert(it, value);
        }
    }

    sequence = mainChain;
}

// std::list 用の Ford-Johnson ソート実装
void PmergeMe::fordJohnsonSort(std::list<int>& sequence) {
    if (sequence.size() <= 1)
        return;

    // ペアリング
    std::list<std::pair<int, int> > pairs;
    std::list<int>::iterator it = sequence.begin();
    while (it != sequence.end()) {
        int first = *it;
        ++it;
        if (it != sequence.end()) {
            int second = *it;
            ++it;
            if (first < second)
                std::swap(first, second);
            pairs.push_back(std::make_pair(first, second));
        } else {
            pairs.push_back(std::make_pair(first, -1));
        }
    }

    // メインチェーンの構築
    std::list<int> mainChain;
    for (std::list<std::pair<int, int> >::iterator pit = pairs.begin(); pit != pairs.end(); ++pit) {
        mainChain.push_back(pit->first);
    }
    fordJohnsonSort(mainChain);

    // ペンディング要素の収集
    std::vector<int> pend;
    for (std::list<std::pair<int, int> >::iterator pit = pairs.begin(); pit != pairs.end(); ++pit) {
        if (pit->second != -1)
            pend.push_back(pit->second);
    }

    // ヤコビアン数列に従って挿入
    std::vector<size_t> indices = generateJacobsthalIndices(pend.size());
    std::vector<bool> inserted(pend.size(), false);
    for (size_t k = 0; k < indices.size(); ++k) {
        size_t idx = indices[k];
        if (idx >= pend.size())
            continue;
        int value = pend[idx];
        std::list<int>::iterator mit = mainChain.begin();
        while (mit != mainChain.end() && *mit < value)
            ++mit;
        mainChain.insert(mit, value);
        inserted[idx] = true;
    }
    for (size_t j = 0; j < pend.size(); ++j) {
        if (!inserted[j]) {
            int value = pend[j];
            std::list<int>::iterator mit = mainChain.begin();
            while (mit != mainChain.end() && *mit < value)
                ++mit;
            mainChain.insert(mit, value);
        }
    }

    sequence = mainChain;
}
