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
        int transposedMatrix[N][N];

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                matrix[i][j] = 4 * i + j;
                std::cout << matrix[i][j] << " ";

            }
            std::cout << std::endl;
        }

     

        int n = 1;
      
        int temp_a[N][elements_per_process];
        for (int i = 0; i < N; i += elements_per_process, n++) {
            for (int j = 0; j < N; j++) {
                for (int g = 0; g < elements_per_process; g++) {
                    temp_a[j][g] = matrix[j][g + elements_per_process * (n - 1)];
                }
            }
            MPI_Send(&temp_a[0][0], elements_per_process * N, MPI_INT, n, 10, MPI_COMM_WORLD);
        }

        n = 1;
        for (int i = 0; i < N; i += elements_per_process, n++) {
            MPI_Recv(&transposedMatrix[i][0], elements_per_process * N, MPI_INT, n, 10, MPI_COMM_WORLD, &status);
        }

        std::cout << "Resulting matrix:\n";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << transposedMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else {
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int local_matrix[N][elements_per_process];
        int transMatrix[elements_per_process][N];

        for (int i = 0; i < elements_per_process; i++) {
            for (int j = 0; j < N; j++) {
                transMatrix[i][j] = 0;
            }
        }

        MPI_Recv(&local_matrix[0][0], elements_per_process * N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < elements_per_process; i++) {
            for (int j = 0; j < N; j++) {
                transMatrix[i][j] = local_matrix[j][i];
            }
        }
        MPI_Send(&transMatrix, elements_per_process * N, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}