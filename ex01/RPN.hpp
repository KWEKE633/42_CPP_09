#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>

class RPN {
public:
    static int evaluate(const std::string& expr);

private:
    RPN();
    RPN(const RPN&);
    RPN& operator=(const RPN&);
    ~RPN();
};

#endif
