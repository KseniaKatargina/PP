#include <iostream>
#include <omp.h>
#include <random>

int main() {
    const int rows = 6;
    const int cols = 8;
    int d[rows][cols];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            d[i][j] = dist(gen);
        }
    }

    int min_value = d[0][0];
    int max_value = d[0][0];

    const int num_threads = 4;

#pragma omp parallel for num_threads(num_threads) reduction(min:min_value) reduction(max:max_value)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (d[i][j] < min_value) {
                min_value = d[i][j];
            }
            if (d[i][j] > max_value) {
                max_value = d[i][j];
            }
        }
    }

    std::cout << "Minimum value: " << min_value << std::endl;
    std::cout << "Maximum value: " << max_value << std::endl;

}
