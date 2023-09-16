#include <iostream>
#include <omp.h>

int main() {
    omp_set_num_threads(8);

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        std::cout << "Thread " << thread_id << " of " << num_threads << ": Hello World" << std::endl;
    }
}
