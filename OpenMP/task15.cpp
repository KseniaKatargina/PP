#include <iostream>
#include <vector>
#include <omp.h>

bool is_prime(int n) {
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;

    if (n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}

int main() {
    int start, end;

    std::cout << "Enter the start of the range: ";
    std::cin >> start;

    std::cout << "Enter the end of the range: ";
    std::cin >> end;

    int num_threads;
    std::cout << "Enter the number of threads: ";
    std::cin >> num_threads;

    omp_set_num_threads(num_threads);

    std::vector<int> primes;

#pragma omp parallel for
    for (int i = start; i <= end; ++i) {
        if (is_prime(i)) {
#pragma omp critical
            primes.push_back(i);
        }
    }

    std::cout << "Prime numbers in the range [" << start << ", " << end << "] are: ";
    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;

    return 0;
}
