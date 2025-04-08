#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>

int current_process_rank;
int comm_size;

void mpi_check(int rc) {
    if (rc != MPI_SUCCESS) {
        printf("MPI error in %dth process.\n", current_process_rank);
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
}

int main(int argc, char *argv[]) {
    int rc;
    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    rc = MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    mpi_check(rc);

    rc = MPI_Comm_rank(MPI_COMM_WORLD, &current_process_rank);
    mpi_check(rc);

    double *u, *unew, delta, maxdelta, globalmaxdelta;
    double eps = 1.e-6;
    double h, tau;

    int N, i, count = 0;

    if (argc != 2) {
        printf("Usage: exefile npoints\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    N = atoi(argv[1]);
    if (N == 0) {
        if (current_process_rank == 0)
            printf("Set N to 1000\n");
        N = 1000;
    } else {
        if (current_process_rank == 0)
            printf("Set N to %d\n", N);
    }

    u = (double *)malloc((N + 1) * sizeof(double));
    if (u == NULL) {
        printf("Can't allocate memory for u\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    unew = (double *)malloc((N + 1) * sizeof(double));
    if (unew == NULL) {
        printf("Can't allocate memory for unew\n");
        free(u);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    // begin & bound values
    for (i = 1; i < N; i++) {
        u[i] = 0;
    }

    unew[0] = u[0] = 1.0;
    unew[N] = u[N] = 0;

    h = 1.0 / N;
    tau = 0.5 * (h * h);

    int beg_j = 1 + (int)(((double)N - 1) / comm_size * current_process_rank);
    int end_j = (int)(((double)N - 1) / comm_size * (current_process_rank + 1));

    while (1) {
        for (i = beg_j; i <= end_j; i++) {
            unew[i] = u[i] + (tau / (h * h)) * (u[i - 1] - 2 * u[i] + u[i + 1]);
        }

        maxdelta = 0;
        for (i = beg_j; i <= end_j; i++) {
            delta = fabs(unew[i] - u[i]);
            if (delta > maxdelta) maxdelta = delta;
        }

        mpi_check(MPI_Reduce(&maxdelta, &globalmaxdelta, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD));
        mpi_check(MPI_Bcast(&globalmaxdelta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD));
        if (globalmaxdelta < eps) {
            break;
        }

        count++;

        if (current_process_rank != 0) {
            mpi_check(MPI_Send(&unew[beg_j], 1, MPI_DOUBLE, current_process_rank - 1, 0, MPI_COMM_WORLD));
            mpi_check(MPI_Recv(&unew[beg_j - 1], 1, MPI_DOUBLE, current_process_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
        }
        if (current_process_rank != comm_size - 1) {
            mpi_check(MPI_Send(&unew[end_j], 1, MPI_DOUBLE, current_process_rank + 1, 0, MPI_COMM_WORLD));
            mpi_check(MPI_Recv(&unew[end_j + 1], 1, MPI_DOUBLE, current_process_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
        }

        for (i = 1; i < N; i++) {
            u[i] = unew[i];
        }
    }

    int size = end_j - beg_j + 1;

    printf("Process %d: %d %d %d\n", current_process_rank, beg_j, end_j, size);

    int *sizes = NULL;
    if (current_process_rank == 0) {
        sizes = (int *)malloc(comm_size * sizeof(int));
    }

    mpi_check(MPI_Gather(&size, 1, MPI_INT, sizes, 1, MPI_INT, 0, MPI_COMM_WORLD));

    int *displs = NULL;
    if (current_process_rank == 0) {
        displs = (int *)malloc(comm_size * sizeof(int));

        displs[0] = 0;
        for (int p = 1; p < comm_size; p++) {
            displs[p] = displs[p - 1] + sizes[p - 1];
        }
    }

    mpi_check(MPI_Gatherv(unew + beg_j, size, MPI_DOUBLE, u + 1, sizes, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD));

    if (current_process_rank == 0) {
        free(displs);
        free(sizes);
    }

    if (current_process_rank == 0) {
        FILE *ff;
        printf("%d steps\n",count);
        if ((ff = fopen("seqres", "w+")) == NULL){
           printf("Can't open file\n"); free(u); free(unew);
           exit(-1);
        }
        
        for(i=0; i<N+1; i++)
           fprintf(ff, "%f\n", u[i]);
           
        fclose(ff); free(u); free(unew);
        return 0;  
    }

    free(u);
    free(unew);

    MPI_Finalize();
    return 0;
}
