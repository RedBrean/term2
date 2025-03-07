#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rc;
    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        printf ("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    int n, nc;
    nc = MPI_Comm_size(MPI_COMM_WORLD, &n);
    if (nc != MPI_SUCCESS) {
        printf ("Error in Comm_size.\n");
        MPI_Abort(MPI_COMM_WORLD, nc);
    }
    int r;
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &r);
    if (rc != MPI_SUCCESS) {
        printf ("Error in Comm_rank.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    int msg_r = 0;
    int msg_send = msg_r;

    if(r == 0) {
        MPI_Send(&msg_send , 1, MPI_INT, 1, 0, MPI_COMM_WORLD);    
    }
    else
    {
        MPI_Status status;
        MPI_Recv(&msg_r , 1 , MPI_INT , int MPI_ANY_SOURCE , MPI_ANY_TAG , MPI_COMM_WORLD, &status);
        msg_send = msg_r+r;
        if(r!=n-1)
        {
            MPI_Send(&msg_r , 1, MPI_INT, r+1, 0, MPI_COMM_WORLD);  
        }
    }

    printf("Hello World. There is %d processes. My number is %d. I recieve %d and send %d.\n", n, r, msg_r, msg_send);
    fflush(stdout);
    MPI_Finalize(); 
    return 0;
}
