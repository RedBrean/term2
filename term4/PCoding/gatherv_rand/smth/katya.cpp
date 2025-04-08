#include <mpi.h>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <stdlib.h>


int main(int argc, char *argv[])
{
   
    int rc;
    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        printf ("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    int size, nc;
    nc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (nc != MPI_SUCCESS) {
        printf ("Error in Comm_size.\n");
        MPI_Abort(MPI_COMM_WORLD, nc);
    }
    int rank;
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rc != MPI_SUCCESS) {
        printf ("Error in Comm_rank.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    

    int* counts = new int [size];
    

    counts[rank] = rand()%7+1;

    printf("I'm %d, rand is sucessful\n", rank);
    double* part = new double [counts[rank]];
    for (int i = 0; i < counts[rank]; i++){
        part[i]=sqrt(rank);
    }
 
 

    int* displ = new int [size];
    displ[0] = 0;
    int N = 0;
    
    MPI_Gather(&counts[rank], 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            N += counts[i];
        }
        std::cout << "Total data size N is " << N << std::endl;
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "Total data size N is " << N << std::endl;
    double* out = new double[N];


    int gath  = MPI_Gatherv(part, counts[rank], MPI_DOUBLE, out, counts, displ, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (gath != MPI_SUCCESS) {
        printf ("Error in Gatherv.\n");
        MPI_Abort(MPI_COMM_WORLD, gath);
    }
 
    if (rank == 0){
        std::fstream file("output3.txt", std::ios_base::out);
        if (!file) {
            std::cerr << "Не удалось открыть файл для записи." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1); // Или любой другой код ошибки
        }
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < counts[i]; j++)
            {
                file << out[displ[i] + j] << " ";
            }
            file << "\n";
        }
    
        file.close();
    }
    delete[] displ;
    delete[] counts;
    delete[] out;
    delete[] part;
    MPI_Finalize(); 
    return 0;
}