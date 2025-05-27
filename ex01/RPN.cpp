#include "RPN.hpp"

RPN::RPN() {}

RPN::~RPN() {}

int RPN::evaluate(const std::string& expr) {
    std::stack<int> stack;
    std::istringstream iss(expr);
    std::string token;

    while (iss >> token) {
        if (token.length() == 1 && std::isdigit(token[0])) {
            stack.push(token[0] - '0');
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (stack.size() < 2)
                throw std::runtime_error("Error");
            int b = stack.top(); stack.pop();
            int a = stack.top(); stack.pop();
            int res;

            if (token == "+") {
                res = a + b;
                if (res < a)
                    throw std::runtime_error("Overflow_plus");
            }
            else if (token == "-") {
                res = a - b;
                if (res > a)
                    throw std::runtime_error("Overflow_minus");
            }
            else if (token == "*") {
                res = a * b;
                int tmp = res;
                if (b != 0 && ((tmp / b) != a))
                    throw std::runtime_error("Overflow_Multiplied");
            }
            else {
                if (b == 0)
                    throw std::runtime_error("Division by zero");
                res = a / b;
                int tmp = res;
                if (tmp * b != a)
                    throw std::runtime_error("Overflow_Division");
            }
            stack.push(res);
        } else {
            throw std::runtime_error("Error");
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Error");

    return stack.top();
}
