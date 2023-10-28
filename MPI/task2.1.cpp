#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 10;
    int x[n];
    int y[n];
    int z[n];

    int a = 5;
    int b = 6;

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            x[i] = 1 + i;
            y[i] = 2 * i;
        }

        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;

        std::cout << "Initial vector x: ";
        for (int i = 0; i < n; i++) {
            std::cout << x[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Initial vector y: ";
        for (int i = 0; i < n; i++) {
            std::cout << y[i] << " ";
        }
        std::cout << std::endl;

        for (int i = 1; i < size; i++) {
            MPI_Send(x, n, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(y, n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(x, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(y, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < n; i++) {
        z[i] = a * x[i] + b * y[i];
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(z, n, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        std::cout << "Result on process 0:" << std::endl;
        for (int i = 0; i < n; i++) {
            std::cout << z[i] << " ";
        }
        std::cout << std::endl;
    }
    else {
        MPI_Send(z, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}



