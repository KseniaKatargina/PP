#include <iostream>
#include <cmath>
#include <omp.h>

int main() {
    int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
    int p = 2;

    int decimal_number = 0;

#pragma omp parallel for reduction(+:decimal_number)
    for (int i = 0; i < 30; ++i) {
        int digit = a[i];
        int power = 29 - i;

        int value = digit * pow(p, power);
        decimal_number += value;
    }

    std::cout << "Result: " << decimal_number << std::endl;

    return 0;
}

