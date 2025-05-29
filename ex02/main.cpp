#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>
#include <sstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./PmergeMe <sequence of positive integers>" << std::endl;
        return 1;
    }

    std::vector<int> vec;
    std::list<int> lst;
    for (int i = 1; i < argc; ++i) {
        std::stringstream ss(argv[i]);
        long value;
        if (!(ss >> value) || !(ss.eof()) || value <= 0 || value >= 2147483647) {
            std::cerr << "Error: Invalid input '" << argv[i] << "'" << std::endl;
            return 1;
        }
        vec.push_back(value);
        lst.push_back(value);
    }

    PmergeMe sorter;

    // BEFORE
    std::cout << "Before: ";
    for (size_t i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << " ";
    std::cout << std::endl;

    // VECTOR SORT
    std::clock_t start = std::clock();
    sorter.sortVector(vec);
    std::clock_t end = std::clock();
    double vec_time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;

    // LIST SORT
    start = std::clock();
    sorter.sortList(lst);
    end = std::clock();
    double list_time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;

    // AFTER
    std::cout << "After: ";
    for (size_t i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << " ";
    std::cout << std::endl;

    // TIME RESULTS
    std::cout << "Time to process a range of " << vec.size()
              << " elements with std::vector : " << vec_time << " us" << std::endl;
    std::cout << "Time to process a range of " << lst.size()
              << " elements with std::list   : " << list_time << " us" << std::endl;

    return 0;
}

// ./PmergeMe $(jot -r 3000 1 100000 | tr '\n' ' ')
