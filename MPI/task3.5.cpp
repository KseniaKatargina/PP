#include <iostream>
#include <mpi.h>

const int N = 100;

struct ValueWithIndex {
    int value;
    int index;
};

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int elements_per_process = 20;
    int localData[elements_per_process];
    ValueWithIndex localMaxValue;
    localMaxValue.value = 0;
    localMaxValue.index = -1;

    int globalVector[N];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            globalVector[i] = i * 2 + 1;
        }
    }

    MPI_Scatter(globalVector, elements_per_process, MPI_INT, localData, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < elements_per_process; i++) {
        if (localData[i] > localMaxValue.value) {
            localMaxValue.value = localData[i];
            localMaxValue.index = i + rank * elements_per_process;
        }
    }

    ValueWithIndex globalMaxValue;
    globalMaxValue.value = 0;
    globalMaxValue.index = -1;

    MPI_Reduce(&localMaxValue, &globalMaxValue, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    std::cout << "Process " << rank << ": Local Max = " << localMaxValue.value << " at index " << localMaxValue.index << std::endl;

    if (rank == 0) {
        std::cout << "Initial Vector: ";
        for (int i = 0; i < N; i++) {
            std::cout << globalVector[i] << " ";
        }
        std::cout << std::endl;
    }

    if (rank == 0) {
        std::cout << "Global Max = " << globalMaxValue.value << " at index " << globalMaxValue.index << std::endl;
    }

    MPI_Finalize();

    return 0;
}

