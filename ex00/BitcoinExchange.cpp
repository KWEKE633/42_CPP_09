#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <climits>
#include <limits>
#include <ctime>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _rateMap(other._rateMap) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other)
        _rateMap = other._rateMap;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file)
        throw std::runtime_error("Error: could not open database file.");

    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string date, valueStr;
        if (std::getline(ss, date, ',') && std::getline(ss, valueStr)) {
            float rate = std::atof(valueStr.c_str());
            _rateMap[date] = rate;
        }
    }
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    int year, month, day;
    std::istringstream iss(date);
    char dash1, dash2;

    if (!(iss >> year >> dash1 >> month >> dash2 >> day))
        return false;
    if (dash1 != '-' || dash2 != '-')
        return false;

    // 範囲チェック
    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31)
        return false;

    std::tm timeinfo = {};
    timeinfo.tm_year = year - 1900; // tm_year: 年 - 1900
    timeinfo.tm_mon = month - 1;    // tm_mon: 0 = Jan
    timeinfo.tm_mday = day;

    // mktime() が日付を補正するか確認
    std::time_t t = std::mktime(&timeinfo);
    if (t == -1)
        return false;

    // 元の値と一致するか確認（補正されてたら偽）
    if (timeinfo.tm_year != year - 1900 ||
        timeinfo.tm_mon != month - 1 ||
        timeinfo.tm_mday != day)
        return false;

    return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr) const {
    std::istringstream iss(valueStr);
    float val;
    iss >> val;
    return !iss.fail() && iss.eof() && val >= 0 && val <= 1000;
}

float BitcoinExchange::parseValue(const std::string& valueStr) const {
    return static_cast<float>(std::atof(valueStr.c_str()));
}

std::string BitcoinExchange::getClosestDate(const std::string& inputDate) const {
    std::map<std::string, float>::const_iterator it = _rateMap.lower_bound(inputDate);
    if (it != _rateMap.begin() && (it == _rateMap.end() || it->first != inputDate))
        --it;
    return it->first;
}

void BitcoinExchange::processInput(const std::string& filename) const {
    std::ifstream file(filename.c_str());
    if (!file) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string date, valueStr;

        size_t sep = line.find(" | ");
        if (sep == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        date = line.substr(0, sep);
        valueStr = line.substr(sep + 3);

        if (!isValidDate(date)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (!isValidValue(valueStr)) {
            float value = parseValue(valueStr);
            if (value < 0)
                std::cerr << "Error: not a positive number." << std::endl;
            else
                std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        float value = parseValue(valueStr);
        std::string closestDate = getClosestDate(date);
        float rate = _rateMap.find(closestDate)->second;
        std::cout << date << " => " << value << " = " << value * rate << std::endl;
    }
}
