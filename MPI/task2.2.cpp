#include <iostream>
#include <mpi.h>

const int N = 10;  // Заменим SIZE на N

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vector[N];
    int matrix[N][N];
    int localResult[N];

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            vector[i] = i + 2;
            for (int j = 0; j < N; ++j) {
                matrix[i][j] = 5 * i + j;
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Vector:\n";
        for (int i = 0; i < N; ++i) {
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;

        for (int i = 1; i < size; ++i) {
            MPI_Send(vector, N, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(matrix, N * N, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(vector, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(matrix, N * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < N; ++i) {
        localResult[i] = 0;
        for (int k = 0; k < N; ++k) {
            localResult[i] += matrix[i][k] * vector[k];
        }
    }

    if (rank == 0) {
        int result[N];

        for (int i = 1; i < size; ++i) {
            MPI_Recv(result, N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int j = 0; j < N; ++j) {
                result[j] = localResult[j];
            }
        }

        std::cout << "Resulting vector:\n";
        for (int i = 0; i < N; ++i) {
            std::cout << result[i] << " ";
        }
        std::cout << std::endl;
    }
    else {
        MPI_Send(localResult, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}




