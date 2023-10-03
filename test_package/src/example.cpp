#include "qp.hpp"
#include <iostream>

int main() {
    auto res = qp::encode("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567");
    std::cout << res << std::endl;
}
