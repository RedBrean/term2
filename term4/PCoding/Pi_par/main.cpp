#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <string>

int current_process_rank;

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

void init_calc(int first_process, int last_process)
{
    long long data[3];
    std::fstream file("N.dat", std::ios_base::in);
    file >> data[0];

    int k = last_process-first_process+1;
    for (int i = first_process; i<=last_process; i++)
    {
        data[1] = (i-first_process)*data[0]/k;
        data[2] = (i+1-first_process)*data[0]/k;
        mpi_check(MPI_Send(&data, 3, MPI_LONG_LONG, i, 1, MPI_COMM_WORLD));
        // printf ("send %lld, %lld, %lld to %d\n", data[0], data[1], data[2], i);
    }
}

void calculate_task()
{
    // printf("%d, listen to task\n", current_process_rank);
    long long data[3];
    mpi_check(MPI_Recv(&data , 3 , MPI_LONG_LONG, 0, 1 , MPI_COMM_WORLD, MPI_STATUS_IGNORE));   
    // printf("%d, recieved task; %lld, %lld, %lld\n", current_process_rank, data[0], data[1], data[2]);
    double result;
    result = calc(data[0], data[1], data[2]);
    mpi_check(MPI_Send(&result, 1 , MPI_DOUBLE, 0, 2, MPI_COMM_WORLD));
    // printf("%d, sended result = %f\n", current_process_rank, result);
}

double final_calc(int first_process, int last_process)
{
    int n_of_calculators = last_process-first_process;
    double recieved;
    double Pi;
    for(int k = 0; k<=n_of_calculators; k++)
    {
        mpi_check(MPI_Recv(&recieved ,1 , MPI_DOUBLE , MPI_ANY_SOURCE, 2, MPI_COMM_WORLD , MPI_STATUS_IGNORE));
        // printf("recieved %f\n", recieved);
        Pi+=recieved;
    }
    return Pi;
}

int main(int argc, char *argv[])
{
    int comm_size, rc;
    long long N;
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

    if(current_process_rank == 0)
    {
        init_calc(0, comm_size-1);
        calculate_task();
        double Pi = final_calc(0, comm_size-1);
        printf("Calculated Pi = %f\nThere were used %d processes\n", Pi, comm_size);
    }
    else{
        calculate_task();
    }

    MPI_Finalize(); 
    return 0;
}