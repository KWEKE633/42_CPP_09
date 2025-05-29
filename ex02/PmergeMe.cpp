#include "PmergeMe.hpp"
#include <algorithm>
#include <iterator>

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
	(void)other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	(void)other;
	return *this;
}

// Jacobsthal数列のインデックス列を生成
std::vector<size_t> PmergeMe::generateJacobsthalIndices(size_t size) {
	std::vector<size_t> indices;
	indices.push_back(0); // インデックス0は常に含む

	size_t j1 = 1, j2 = 1;
	while (j2 < size) {
		indices.push_back(j2);
		size_t temp = j2;
		j2 = j2 + 2 * j1;
		j1 = temp;
	}
	return indices;
}

// vector 用 Ford-Johnson
void PmergeMe::sortVector(std::vector<int>& sequence) {
	if (sequence.size() <= 1) return;

	// ペア分けと比較
	std::vector<std::pair<int, int> > pairs;
	size_t i = 0;
	for (; i + 1 < sequence.size(); i += 2) {
		int a = sequence[i], b = sequence[i + 1];
		if (a < b) std::swap(a, b);
		pairs.push_back(std::make_pair(a, b));
	}
	bool hasStraggler = (i < sequence.size());
	int straggler = hasStraggler ? sequence[i] : -1;

	// メインチェーン作成
	std::vector<int> mainChain;
	for (size_t j = 0; j < pairs.size(); ++j)
		mainChain.push_back(pairs[j].first);

	sortVector(mainChain); // 再帰

	// ペンディング挿入対象
	std::vector<int> pend;
	for (size_t j = 0; j < pairs.size(); ++j)
		pend.push_back(pairs[j].second);
	if (hasStraggler)
		pend.push_back(straggler);

	// Jacobsthalインデックス順に挿入
	std::vector<size_t> indices = generateJacobsthalIndices(pend.size());
	std::vector<bool> inserted(pend.size(), false);
	for (size_t k = 1; k < indices.size(); ++k) {
		size_t end = indices[k];
		size_t start = indices[k - 1] + 1;
		for (size_t idx = end; idx >= start && idx < pend.size(); --idx) {
			std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[idx]);
			mainChain.insert(pos, pend[idx]);
			inserted[idx] = true;
		}
	}

	// 残りを挿入
	for (size_t j = 0; j < pend.size(); ++j) {
		if (!inserted[j]) {
			std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[j]);
			mainChain.insert(pos, pend[j]);
		}
	}
	sequence = mainChain;
}

// list 用 Ford-Johnson
void PmergeMe::sortList(std::list<int>& sequence) {
	if (sequence.size() <= 1) return;

	// ペア分けと比較
	std::list<std::pair<int, int> > pairs;
	std::list<int>::iterator it = sequence.begin();
	while (it != sequence.end()) {
		int first = *it++;
		if (it != sequence.end()) {
			int second = *it++;
			if (first < second) std::swap(first, second);
			pairs.push_back(std::make_pair(first, second));
		} else {
			pairs.push_back(std::make_pair(first, -1));
		}
	}

	// メインチェーン作成
	std::list<int> mainChain;
	for (std::list<std::pair<int, int> >::iterator pit = pairs.begin(); pit != pairs.end(); ++pit)
		mainChain.push_back(pit->first);

	sortList(mainChain); // 再帰

	// ペンディング
	std::vector<int> pend;
	for (std::list<std::pair<int, int> >::iterator pit = pairs.begin(); pit != pairs.end(); ++pit) {
		if (pit->second != -1)
			pend.push_back(pit->second);
	}

	// Jacobsthal順に挿入
	std::vector<size_t> indices = generateJacobsthalIndices(pend.size());
	std::vector<bool> inserted(pend.size(), false);
	for (size_t k = 1; k < indices.size(); ++k) {
		size_t end = indices[k];
		size_t start = indices[k - 1] + 1;
		for (size_t idx = end; idx >= start && idx < pend.size(); --idx) {
			std::list<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[idx]);
			mainChain.insert(pos, pend[idx]);
			inserted[idx] = true;
		}
	}

	// 残りを挿入
	for (size_t j = 0; j < pend.size(); ++j) {
		if (!inserted[j]) {
			std::list<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[j]);
			mainChain.insert(pos, pend[j]);
		}
	}
	sequence = mainChain;
}
