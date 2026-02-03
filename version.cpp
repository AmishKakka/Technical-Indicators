#include <iostream>

int main() {
    if (__cplusplus == 202100L) {
        std::cout << "C++ version: C++23" << std::endl;
    } else if (__cplusplus == 202002L) {
        std::cout << "C++ version: C++20" << std::endl;
    } else if (__cplusplus == 201703L) {
        std::cout << "C++ version: C++17" << std::endl;
    } else if (__cplusplus == 201402L) {
        std::cout << "C++ version: C++14" << std::endl;
    } else if (__cplusplus == 201103L) {
        std::cout << "C++ version: C++11" << std::endl;
    } else if (__cplusplus == 199711L) {
        std::cout << "C++ version: C++98" << std::endl;
    } else {
        std::cout << "C++ version: Unknown or older than C++98 (" << __cplusplus << ")" << std::endl;
    }
    return 0;
}
