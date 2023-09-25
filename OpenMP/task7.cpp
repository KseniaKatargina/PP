#include <iostream>
#include <omp.h>

int main() {
    const int size = 12;
    int a[size], b[size], c[size];

    // Первая параллельная область
#pragma omp parallel num_threads(3)
    {
        int thread_num = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Распределение итераций статически
#pragma omp for schedule(static, 4)
        for (int i = 0; i < size; ++i) {
            a[i] = i;
            b[i] = i;
            std::cout << "Thread " << thread_num << " of " << num_threads<< " in the first parallel region." << std::endl;
        }
    }
#pragma omp critical
    {
        for (int i = 0; i < size; i++) {
            std::cout << "a[" << i << "] = " << a[i] << ", ";
            std::cout << "b[" << i << "] = " << b[i] << std::endl;
        }
    }

    // Вторая параллельная область
#pragma omp parallel num_threads(4)
    {
        int thread_num = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Распределение итераций динамически
#pragma omp for schedule(dynamic, 3)
        for (int i = 0; i < size; ++i) {
            c[i] = a[i] + b[i];
            std::cout << "Thread " << thread_num << " of " << num_threads << " in the second parallel region." << std::endl;
        }
    }

#pragma omp critical
    {
        for (int i = 0; i < size; i++) {
            std::cout << "c[" << i << "] = " << c[i] << std::endl;
        }
    }
}
