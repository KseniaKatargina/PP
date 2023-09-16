#include <iostream>
#include <omp.h>

int main() {
    int a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int b[10] = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95};

    int min_a = INT_MAX;
    int max_b = INT_MIN;

#pragma omp parallel num_threads(2)
    {
        int thread_id = omp_get_thread_num();

        if (thread_id == 0) {
            int local_min_a = INT_MAX;
#pragma omp for
            for (int i = 0; i < 10; ++i) {
                if (a[i] < local_min_a) {
                    local_min_a = a[i];
                }
            }
#pragma omp critical
            {
                if (local_min_a < min_a) {
                    min_a = local_min_a;
                }
            }
        } else if (thread_id == 1) {
            int local_max_b = INT_MIN;
#pragma omp for
            for (int i = 0; i < 10; ++i) {
                if (b[i] > local_max_b) {
                    local_max_b = b[i];
                }
            }
#pragma omp critical
            {
                if (local_max_b > max_b) {
                    max_b = local_max_b;
                }
            }
        }
    }

    std::cout << "Min value in array a: " << min_a << std::endl;
    std::cout << "Max value in array b: " << max_b << std::endl;

    return 0;
}

