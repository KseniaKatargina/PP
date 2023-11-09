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
        int matrix[N][N];
        int vector[N];
        int localResult[N];

        std::cout << "Matrix: ";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = 5 * i + j;
                std::cout << matrix[i][j] << " ";
            }
            vector[i] = i + 2;
        }
        std::cout << std::endl;

        std::cout << "Vector: ";
        for (int i = 0; i < N; i++) {
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;

       

        int n = 1;
        for (int i = 0; i < N; i += 2, n++) {
            MPI_Send(&matrix[i][0], elements_per_process * N, MPI_INT, n, 10, MPI_COMM_WORLD);
            MPI_Send(&vector[0], N, MPI_INT, n, 10, MPI_COMM_WORLD);
        }

        n = 1;
        for (int i = 0; i < N; i += 2, n++) {
            MPI_Recv(&localResult[i], elements_per_process, MPI_INT, n, 10, MPI_COMM_WORLD, &status);
        }

        std::cout << "Result: " << std::endl;
        for (int i = 0; i < N; i++) {
            std::cout << localResult[i] << " ";
        }
        std::cout << std::endl;
    }
    else {
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int local_matrix[elements_per_process][N];
        int local_vector[N];
        int local_result[elements_per_process];

        for (int i = 0; i < elements_per_process; i++) {
            local_result[i] = 0;
        }

        MPI_Recv(&local_matrix[0][0], elements_per_process * N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_vector[0], N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < elements_per_process; i++) {
            for (int j = 0; j < N; j++) {
                local_result[i] += local_matrix[i][j] * local_vector[j];
            }
        }
        MPI_Send(&local_result, elements_per_process, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}