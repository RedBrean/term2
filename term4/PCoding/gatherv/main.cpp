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

void root_function()
{
    int N = comm_size*comm_size;
    int* out_data = new int[N];
    int* displ = new int[comm_size];
    int* rev_count = new int[comm_size];
    
    for(int i = 0; i<comm_size; i++)
    {
        displ[i] = i*(i+1);
        rev_count[i] = i+1;
    }

    for(int i=0; i<N; i++)
    {
        out_data[i] = 0;
    }

    int root_data = 1;
    mpi_check(MPI_Gatherv(&root_data, 1, MPI_INT, out_data, rev_count, displ, MPI_INT, 0, MPI_COMM_WORLD));
    std::fstream file("output.txt", std::ios_base::out);
    for(int i=0; i<N; i++)
    {
        file<<out_data[i] << " ";
    }
    file << "\n";
    delete[] displ, rev_count, out_data;
}

void send_function()
{
    int* data = new int[current_process_rank+1];
    for(int i=0; i<current_process_rank+1; i++)
    {
        data[i] = current_process_rank+1;
    }

    mpi_check(MPI_Gatherv(data, current_process_rank+1, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD));

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

    if(current_process_rank == 0) {
        root_function();
    }
    else{
        send_function();
    }



    MPI_Finalize(); 
    return 0;
}