/*
define streak(n) = k to be the smallest positive integer k such that
n + k is not divisible by k + 1

ex.
13 is divisble by 1
14 is divisible by 2
15 by 3
16 by 4
but 17 not by 5

therefore, streak(13) = 4

define P(s, N) to be number of integers 1 < n < N for which streak(n) = s.
So, P(3, 14) = 1 and P(6, 10^6) = 14286

find the sum, as i ranges from 1 to 31, of P(i, 4^i)
*/


#include <iostream>

long long s(int i){
    int div = 1;
    while (true) {
        if (i % div == 0) {
            div += 1;
            i += 1;
        }
        else {
            return div - 1;
        }
    }
}

long long p(const int s_, const int n) {
    long long num = 0;
    for (int i = 2; i < n; i++){
        if (s(i) == s_) num += 1; 
    }
    return num;
}

int main(){
    long long sum = 0;
    for (int i = 1; i <= 31; i++) {
        std::cout << "Current status -> i = " << i << std::endl;
        sum += p(i, pow(4, i));
    }
    std::cout << sum << std::endl;
}