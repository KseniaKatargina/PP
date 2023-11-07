#include <iostream>
#include <mpi.h>

const int MATRIX_SIZE = 100;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    int local_max = 0;

    if (rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = 0; j < MATRIX_SIZE; ++j) {
                matrix[i][j] = i * MATRIX_SIZE + j;
                
            }
        }
    }

    
    int local_rows = MATRIX_SIZE / size;

    int local_matrix[20][MATRIX_SIZE];


    
    MPI_Scatter(matrix, local_rows * MATRIX_SIZE, MPI_INT, local_matrix, local_rows * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    
    for (int i = 0; i < MATRIX_SIZE; i++) {
        int row_sum = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            row_sum += std::abs(matrix[j][i]);

        }
        if (row_sum > local_max) {
            local_max = row_sum;
        }
    }

    int global_max = 0;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Norm of the matrix: " << global_max << std::endl;
    }

    MPI_Finalize();
    return 0;
}




