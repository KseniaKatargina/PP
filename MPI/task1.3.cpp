#include <iostream>
#include <mpi.h>
#include <random>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);

    const int n = 10;
    int a[n];

    if (rank == 0) {
        for (int i = 0; i < n; ++i) {
            a[i] = dist(gen);
        }

        std::cout << "Process " << rank << " sending the array: ";
        for (int i = 0; i < n; ++i) {
            std::cout << a[i] << " ";
        }
        std::cout << std::endl;

        MPI_Send(a, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        MPI_Recv(a, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "Process " << rank << " received the array: ";
        for (int i = 0; i < n; ++i) {
            std::cout << a[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();

    return 0;
}

