#include <iostream>
#include <vector>
#include <random>
#include <omp.h>

int main() {

    const int size = 100;
    const int num_threads = 4;

    int arr[size];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < size; ++i) {
        arr[i] = dist(gen);
    }

    int max_multiple_of_7 = 0;

#pragma omp parallel num_threads(num_threads)
    {
        int local_max = 0;

#pragma omp for
        for (int i = 0; i < size; ++i) {
            if (arr[i] % 7 == 0 && arr[i] > local_max) {
                local_max = arr[i];
            }
        }

#pragma omp critical
        {
            if (local_max > max_multiple_of_7) {
                max_multiple_of_7 = local_max;
            }
        }
    }

    std::cout << "the maximum number, a multiple of 7" << max_multiple_of_7 << std::endl;

}
