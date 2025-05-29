#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <list>
#include <string>

class PmergeMe {
public:
    PmergeMe();
    ~PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);

    void sortVector(std::vector<int>& sequence);
    void sortList(std::list<int>& sequence);

private:
    // void fordJohnsonSort(std::vector<int>& sequence);
    // void fordJohnsonSort(std::list<int>& sequence);

    std::vector<size_t> generateJacobsthalIndices(size_t size);
};

#endif
