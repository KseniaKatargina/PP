#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int myNumber = rank * 10; 

    if (rank == 0) {
        int* receivedNumbers = new int[size];
        receivedNumbers[0] = myNumber;

        std::cout << "Received numbers in ascending order of ranks:" << std::endl;
        for (int sender = 1; sender < size; ++sender) {
            MPI_Recv(&receivedNumbers[sender], 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Rank " << sender << ": " << receivedNumbers[sender] << std::endl;
        }

        delete[] receivedNumbers;
    }
    else {
        MPI_Send(&myNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
