#include <iostream>
#include <mpi.h>

const int N = 10;

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix1[N][N];
    int matrix2[N][N];
    int localResult[N][N];

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                matrix1[i][j] = i * 2 + j;
                matrix2[i][j] = i * 6 + j;
            }
        }

        std::cout << "Matrix1:\n";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << matrix1[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Matrix2:\n";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << matrix2[i][j] << " ";
            }
            std::cout << std::endl;
        }

        for (int i = 1; i < size; ++i) {
            MPI_Send(&matrix1[0][0], N * N, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&matrix2[0][0], N * N, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(&matrix1[0][0], N * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&matrix2[0][0], N * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            localResult[i][j] = matrix1[i][j] * matrix2[i][j];
        }
    }

    if (rank == 0) {
        int result[N][N];

        for (int i = 1; i < size; ++i) {
            MPI_Recv(&result[0][0], N * N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    if (result[j][k] == 0) {
                        result[j][k] = localResult[j][k];
                    }
                }
            }
        }

        std::cout << "Resulting matrix:\n";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << result[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else {
        MPI_Send(&localResult[0][0], N * N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}


