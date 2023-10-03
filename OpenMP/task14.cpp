#include <iostream>
#include <omp.h>

int main() {
    int number = 210;
    int square = 0;

#pragma omp parallel for reduction(+:square)
    for (int i = 0; i < number; i++) {
        square += 2 * i + 1;
    }

    std::cout << "The square of " << number << " is " << square << std::endl;

    return 0;
}
