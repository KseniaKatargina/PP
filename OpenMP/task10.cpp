#include <iostream>
#include <omp.h>
#include <random>

int main() {
    const int size = 30;
    int a[size];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < size; i++) {
        a[i] = dist(gen);
    }

    int count = 0;

    const int num_threads = 4;

#pragma omp parallel num_threads(num_threads)
    {
        int local_count = 0;
#pragma omp for
        for (int i = 0; i < size; i++) {
            if (a[i] % 9 == 0) {
                local_count++;
            }
        }
#pragma omp atomic
        count += local_count;
    }

    std::cout << "Number of elements divisible by 9: " << count << std::endl;

}

