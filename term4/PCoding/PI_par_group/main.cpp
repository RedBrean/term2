#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <string>

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
double calc(long long N, long long start, long long finish)
{
    double dx = 2.0/N;
    double result = 0.0;
    double x_start = dx*start; 
    double x_finish = dx*finish;
    for (double x = x_start; x<x_finish-dx/2; x+=dx)
    {
        result+=dx*(sqrt(4-x*x)+sqrt(4-(x+dx)*(x+dx)))/2;
    }
    // printf("%d, calcuted result = %f; from %f to %f\n", current_process_rank, result, x_start, x_finish);
    return result;
}


void calculate_Pi()
{

    long long N;
    if(current_process_rank == 0)
    {
        std::fstream file("N.dat", std::ios_base::in);
        file >> N;
    }
    mpi_check(MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD));

    double result;
    result = calc(N, current_process_rank*N/comm_size, (current_process_rank+1)*N/comm_size);
    double Pi;

    mpi_check(MPI_Reduce(&result, &Pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD));
    // printf("%d, sended result = %f\n", current_process_rank, result);
    if(current_process_rank == 0)
    {
        printf("calculated Pi = %f, used %d processes\n", Pi, comm_size);
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

    calculate_Pi();

    MPI_Finalize(); 
    return 0;
}