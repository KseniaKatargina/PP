#include <iostream>
#include <omp.h>

int main() {
    int a = 10;
    int b = 20;

    std::cout << "Before Parallel Region 1: a = " << a << ", b = " << b << std::endl;

#pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        a = 10;
        int thread_id = omp_get_thread_num();
        a += thread_id;
        b += thread_id;

        std::cout << "Inside Parallel Region 1: Thread " << thread_id << " - a = " << a << ", b = " << b << std::endl;
    }

    std::cout << "After Parallel Region 1: a = " << a << ", b = " << b << std::endl;

    std::cout << "Before Parallel Region 2: a = " << a << ", b = " << b << std::endl;

#pragma omp parallel num_threads(4) shared(a) private(b)
    {
        b = 20;
        int thread_id = omp_get_thread_num();
        a -= thread_id;
        b -= thread_id;

        std::cout << "Inside Parallel Region 2: Thread " << thread_id << " - a = " << a << ", b = " << b << std::endl;
    }

    std::cout << "After Parallel Region 2: a = " << a << ", b = " << b << std::endl;

    return 0;
}

