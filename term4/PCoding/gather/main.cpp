#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <fstream>

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


    int N=120;
    if(N%comm_size!=0)
    {
        printf("N не делится на comm size");
        MPI_Finalize();
        return 0;
    }
    int k = N/comm_size;

    int* data = new int[k];

    for(int i=0; i<k; i++)
    {
        data[i] = current_process_rank;
    }

    int* out_data = new int[N];
    mpi_check(MPI_Gather(data, k, MPI_INT, out_data, k, MPI_INT, 0, MPI_COMM_WORLD));


    if(current_process_rank == 0)
    {
        std::fstream file("output.txt", std::ios_base::out);
        for(int i=0; i<N; i++)
        {
            file<<out_data[i] << " ";
        }
        file << "\n";
    }



    delete[] data;
    MPI_Finalize(); 
    return 0;
}