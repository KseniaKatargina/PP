#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    int count;

    srand(time(nullptr));
    const int N = 10;
    const int elements_per_process = 2;

    if (rank == 0) {
        int matrix1[N][N];
        int matrix2[N][N];
        int result[N][N];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix1[i][j] = i * 3 + j;
                matrix2[i][j] = j * 5 + i;
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

        int n = 1;
        for (int i = 0; i < N; i += elements_per_process, n++) {
            MPI_Send(&matrix1[i][0], elements_per_process * N, MPI_INT, n, 10, MPI_COMM_WORLD);
            MPI_Send(&matrix2[0][0], N * N, MPI_INT, n, 10, MPI_COMM_WORLD);
        }

        n = 1;
        for (int i = 0; i < N; i += elements_per_process, n++) {
            MPI_Recv(&result[i][0], elements_per_process * N, MPI_INT, n, 10, MPI_COMM_WORLD, &status);
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
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int local_matrix1[elements_per_process][N];
        int local_matrix2[N][N];
        int local_result[elements_per_process][N];

        for (int i = 0; i < elements_per_process; i++) {
            for (int j = 0; j < N; j++) {
                local_result[i][j] = 0;
            }
        }

        MPI_Recv(&local_matrix1[0][0], elements_per_process * N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_matrix2[0][0], N * N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < elements_per_process; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    local_result[i][j] += local_matrix1[i][k] * local_matrix2[k][j];
                }
            }
        }
        MPI_Send(&local_result, elements_per_process * N, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}