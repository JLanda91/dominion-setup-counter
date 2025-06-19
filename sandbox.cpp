#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmpxx.h>

#include <boost/multiprecision/integer.hpp>

#include <iostream>
#include <chrono>


int main() {
    std::cout << "Bits per limb: " << mp_bits_per_limb << "\n";

    {
        const auto t1 = std::chrono::steady_clock::now();
        mpz_class big_num = 1;
        for (std::size_t i = 0; i < 15; ++i) {
            big_num *= (2*i + 1) * (i+1);
        }
        const auto t2 = std::chrono::steady_clock::now();
        std::cout << big_num << "\n";
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << "ns\n\n";
    }
    {
        using result_t = boost::multiprecision::uint256_t;
        const auto t1 = std::chrono::steady_clock::now();
        result_t big_num = 1;
        for (std::size_t i = 0; i < 15; ++i) {
            big_num *= (2*i + 1) * (i+1);
        }
        const auto t2 = std::chrono::steady_clock::now();
        std::cout << big_num << "\n";
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << "ns\n";
    }
    return 0;
}