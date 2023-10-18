#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank; 
    int size; 

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 10; 
    int data[n];

    if (rank == 1) {
        for (int i = 0; i < n; ++i) {
            data[i] = i;
        }
    }

    if (rank != 1) {
        int recv_count;
        MPI_Status status;

        MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &recv_count);

        int* recv_buffer = new int[recv_count];

        MPI_Recv(recv_buffer, recv_count, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

       
        std::cout << "Process " << rank << " received the array: ";
        for (int i = 0; i < recv_count; ++i) {
            std::cout << recv_buffer[i] << " ";
        }
        std::cout << std::endl;

        
        delete[] recv_buffer;
    }

    if (rank == 1) {
        
        for (int dest = 0; dest < size; ++dest) {
            if (dest != 1) {
                MPI_Send(data, n, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();

    return 0;

}
