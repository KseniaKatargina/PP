#include <iostream>
#include <mpi.h>


const int MATRIX_SIZE = 100;
const int VECTOR_SIZE = 100;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double matrix[MATRIX_SIZE][MATRIX_SIZE];
    double vector[VECTOR_SIZE];
    double local_result[MATRIX_SIZE] = { 0 };

    if (rank == 0) {
        
        
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = 0; j < MATRIX_SIZE; ++j) {
                matrix[i][j] = i + 1;
                
            }
            
        }
        
        for (int i = 0; i < VECTOR_SIZE; ++i) {
            vector[i] = i + 6;
            
        }
    }

    
    MPI_Bcast(vector, VECTOR_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    
    int local_rows = MATRIX_SIZE / size;

    double local_matrix[20][MATRIX_SIZE];

    
    MPI_Scatter(matrix, local_rows * MATRIX_SIZE, MPI_DOUBLE, local_matrix, local_rows * MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    
    for (int i = 0; i < local_rows; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            local_result[i] += local_matrix[i][j] * vector[j];
        }
    }

    
    double global_result[MATRIX_SIZE];
    MPI_Gather(local_result, local_rows, MPI_DOUBLE, global_result, local_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        
        std::cout << "Resulting vector:" << std::endl;
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            std::cout << global_result[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}




