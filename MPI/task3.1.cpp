#include <iostream>
#include <mpi.h>
#include <cmath>

const int N = 100;

int main(int argc, char** argv) {
    int rank, size;
    double localNorm = 0.0;
    double globalNorm = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int localSize = N / size;
    double globalVector[N];

    if (rank == 0) {
        std::cout << "Initial vector: ";
        for (int i = 0; i < N; i++) {
            globalVector[i] = i * 8;
            std::cout << globalVector[i] << " ";
        }
        std::cout << std::endl;
    }

    double localVector[20];

    MPI_Scatter(globalVector, localSize, MPI_DOUBLE, localVector, localSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < localSize; i++) {
        localNorm += std::fabs(localVector[i]);
    }

    MPI_Reduce(&localNorm, &globalNorm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        globalNorm = globalNorm;
        std::cout << "Norm of the vector: " << globalNorm << std::endl;
    }

    MPI_Finalize();

    return 0;
}
