#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int arr[12];
    int block = 12 / size + 1;

    if (rank == 0) {
        for (int i = 0; i < 12; ++i) {
            arr[i] = i + 1;
        }

        for (int i = 1; i < size; ++i) {
            int send_count;
            if (i < 12 % size) {
                send_count = block;
            }
            else {
                send_count = block - 1;
            }
            MPI_Send(arr + i * (block - 1), send_count, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        int recv_count;
        if (rank < 12 % size) {
            recv_count = block;
        }
        else {
            recv_count = block - 1;
        }
        MPI_Recv(arr, recv_count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    std::cout << "Process " << rank << ": ";
    int count;
    if (rank < 12 % size) {
        count = block;
    }
    else {
        count = block - 1;
    }
    for (int i = 0; i < count; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    MPI_Finalize();
    return 0;
}
