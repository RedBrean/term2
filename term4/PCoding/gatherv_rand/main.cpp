#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <stdlib.h>

int current_process_rank;
int comm_size;

void mpi_check(int rc)
{
    if (rc != MPI_SUCCESS) {
        printf ("MPI error in %dth process.\n", current_process_rank);
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
}

void mpi_check(const char* message, int rc, int current_process_rank)
{
    if (rc != MPI_SUCCESS) {
        printf("MPI error in %dth process: %s\n", current_process_rank, message);
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
}

void root_function()
{
    std::srand(current_process_rank + 1);
    int k = std::rand() % 7 + 1;

    int* rev_count = new int[comm_size];
    int* displ = new int[comm_size];
    
    mpi_check(MPI_Gather(&k, 1, MPI_INT, rev_count, 1, MPI_INT, 0, MPI_COMM_WORLD));

    displ[0] = 0;
    int N = k;
    for (int i = 1; i < comm_size; i++)
    {
        displ[i] = displ[i - 1] + rev_count[i - 1];
        N += rev_count[i];
    }
    double* out_data = new double[N];
    double* data = new double[k];
    
    for (int i = 0; i < k; i++)
    {
        data[i] = sqrt(current_process_rank + 1);
    }

    mpi_check(MPI_Gatherv(data, k, MPI_DOUBLE, out_data, rev_count, displ, MPI_DOUBLE, 0, MPI_COMM_WORLD));

    std::fstream file("output.txt", std::ios_base::out);
    for (int i = 0; i < comm_size; i++)
    {
        for (int j = 0; j < rev_count[i]; j++)
        {
            file << out_data[displ[i] + j] << " ";
        }
        file << "\n";
    }

    delete[] displ;
    delete[] rev_count;
    delete[] out_data;
    delete[] data;
}

void send_function()
{
    std::srand(current_process_rank + 1);
    int k = std::rand() % 7 + 1; 
    mpi_check(MPI_Gather(&k, 1, MPI_INT, NULL, 1, MPI_INT, 0, MPI_COMM_WORLD));
    double* data = new double[k];
    
    for (int i = 0; i < k; i++)
    {
        data[i] = sqrt(current_process_rank + 1);
    }

    mpi_check(MPI_Gatherv(data, k, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD));

    delete[] data;
}

int main(int argc, char *argv[])
{
    int rc;
    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        printf ("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    rc = MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if (rc != MPI_SUCCESS) {
        printf ("Error in Comm_size.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    rc = MPI_Comm_rank(MPI_COMM_WORLD, &current_process_rank);
    if (rc != MPI_SUCCESS) {
        printf ("Error in Comm_rank.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    if (current_process_rank == 0) {
        root_function();
    }
    else {
        send_function();
    }

    MPI_Finalize(); 
    return 0;
}
