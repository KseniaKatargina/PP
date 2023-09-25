#include <iostream>
#include <omp.h>

int main() {
    int a[10] = {5, 2, 8, 1, 9, 3, 7, 6, 4, 0};
    int b[10] = {15, 22, 8, 11, 19, 33, 7, 6, 44, 10};

    int avg_a = 0;
    int avg_b = 0;

#pragma omp parallel for reduction(+:avg_a) reduction(+:avg_b)
    for (int i = 0; i < 10; i++) {
        avg_a += a[i];
        avg_b += b[i];
    }

    avg_a /= 10;
    avg_b /= 10;

    std::cout << "Average value of elements in array a: " << avg_a << std::endl;
    std::cout << "Average value of elements in array b: " << avg_b << std::endl;


    if (avg_a == avg_b) {
        std::cout << "The average values of a and b are equal." << std::endl;
    } else if (avg_a < avg_b) {
        std::cout << "The average value of a is less than the average value of b." << std::endl;
    } else {
        std::cout << "The average value of a is greater than the average value of b." << std::endl;
    }
}
