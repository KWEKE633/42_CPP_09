#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>

class BitcoinExchange {
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();

    void loadDatabase(const std::string& filename);
    void processInput(const std::string& filename) const;

private:
    std::map<std::string, float> _rateMap;
    bool isValidDate(const std::string& date) const;
    bool isValidValue(const std::string& valueStr) const;
    float parseValue(const std::string& valueStr) const;
    std::string getClosestDate(const std::string& inputDate) const;
};

#endif
