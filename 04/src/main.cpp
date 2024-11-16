#include <iostream>

#include "BigInt.h"

int main() {
    std::string a, b, sign;
    while (std::cin >> a >> b >> sign) {
        BigInt first(a), second(b);
        if (sign == "+") {
            std::cout << (first + second) << std::endl;
        } else if (sign == "-") {
            if (first < second) {
                std::cout << "Error" << std::endl;
            } else {
                std::cout << (first - second) << std::endl;
            }
        } else if (sign == "*") {
            std::cout << (first * second) << std::endl;
        } else if (sign == "^") {
            if (first == 0 and second == 0) {
                std::cout << "Error" << std::endl;
            } else {
                std::cout << (first ^ second) << std::endl;
            }
        } else if (sign == "/") {
            if (second == 0) {
                std::cout << "Error" << std::endl;
            } else {
                std::cout << (first / second) << std::endl;
            }
        } else if (sign == ">") {
            std::string ans = (first > second) ? "true" : "false";
            std::cout << ans << std::endl;
        } else if (sign == "<"){
            std::string ans = (first < second) ? "true" : "false";
            std::cout << ans << std::endl;
        } else if (sign == "="){
            std::string ans = (first == second) ? "true" : "false";
            std::cout << ans << std::endl;
        }
    }
    return 0;
}
