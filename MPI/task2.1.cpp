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

    int elements_per_process = n / size;
    int local_x[2];
    int local_y[2];
    int local_z[2];

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
            MPI_Send(x + i * elements_per_process, elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(y + i * elements_per_process, elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Оставшиеся данные для процесса 0
        for (int i = 0; i < elements_per_process; i++) {
            local_x[i] = x[i];
            local_y[i] = y[i];
        }
    }
    else {
        // Принимаем данные
        MPI_Recv(local_x, elements_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(local_y, elements_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Вычисления на локальных данных
    for (int i = 0; i < elements_per_process; i++) {
        local_z[i] = a * local_x[i] + b * local_y[i];
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(z + i * elements_per_process, elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Оставшиеся данные для процесса 0
        for (int i = 0; i < elements_per_process; i++) {
            z[i] = a * local_x[i] + b * local_y[i];
        }

        std::cout << "Result on process 0:" << std::endl;
        for (int i = 0; i < n; i++) {
            std::cout << z[i] << " ";
        }
        std::cout << std::endl;
    }
    else {
        // Отправляем результат на процесс 0
        MPI_Send(local_z, elements_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}




