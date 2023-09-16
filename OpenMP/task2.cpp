#include <iostream>
#include <omp.h>

int main() {
    int num_threads_1 = 3;
    int num_threads_2 = 1;


#pragma omp parallel if(num_threads_1 > 1)
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        std::cout << "Thread " << thread_id << " of " << num_threads << " in the first parallel region." << std::endl;
    }
    if (num_threads_2 > 1)
    {
    #pragma omp parallel if(num_threads_2 > 1)
        {
            int thread_id = omp_get_thread_num();
            int num_threads = omp_get_num_threads();

            std::cout << "Thread " << thread_id << " of " << num_threads << " in the second parallel region."
                      << std::endl;
        }
    }
}
