#include <iostream>
#include <vector>

constexpr int MAX_VALUE_FINAL = 40000000;
constexpr int TEST_VALUE = 100; 

std::vector<int> generate_primes(const int max) {
    std::vector<int> primes = {2, 3};
    int k = 4;
    while (k <= max) {
        bool add = true;
        for (auto const &prime : primes) {
            if (k % prime == 0) {
                add = false;
                break;
            }
        }
        if (add) { primes.push_back(k); }
    }
}


int main() {
    auto primes = generate_primes(TEST_VALUE);
    
}