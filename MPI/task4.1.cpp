#include <iostream>
#include <mpi.h>

const int max_n = 40000;

void ping_pong(int rank, int message_size);
void ping_ping(int rank, int message_size);

int main(int argc, char** argv) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int message_size = 1000; message_size <= max_n; message_size += 10000) {
        //ping_pong(rank, message_size);
        ping_ping(rank, message_size);
    }

    MPI_Finalize();
}

void ping_pong(int rank, int message_size) {

    int* mess = new int[message_size];

    for (int i = 0; i < message_size; ++i) {
        mess[i] = i + 1;
    }


    if (rank == 0) {
        auto start = MPI_Wtime();

        MPI_Send(mess, message_size, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(mess, message_size, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        auto end = MPI_Wtime();

        float time_took = end - start;
        std::cout << "Ping Pong. Time consistent for size " << message_size << ": " << time_took << " seconds\n";
    }
    else {
        int* received = new int[message_size];

        MPI_Recv(received, message_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(received, message_size, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    delete[] mess;
}

void ping_ping(int rank, int message_size) {
    int* mess = new int[message_size];

    for (int i = 0; i < message_size; ++i) {
        mess[i] = i + 1;
    }

    auto start = MPI_Wtime();

    if (rank == 0) {
        MPI_Send(mess, message_size, MPI_INT, 1, 1, MPI_COMM_WORLD);
    }
    else {
        MPI_Send(mess, message_size, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        int* received = new int[message_size];

        MPI_Recv(received, message_size, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else {
        int* mess2 = new int[message_size];

        MPI_Recv(mess2, message_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    auto end = MPI_Wtime();

    float time_took = end - start;
    std::cout << "Ping Ping. Time consistent for size " << message_size << ": " << time_took << " seconds\n";

    delete[] mess;
}

