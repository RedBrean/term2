#include <stdio.h>
#include <omp.h>

int main(){
    printf("Number of procs: %d\n", omp_get_num_procs());
    omp_set_num_threads(6); 
    printf("Max number of threads: %d", omp_get_max_threads());
#pragma omp parallel
    {
        printf("Hello from thread %d\n", omp_get_thread_num());
        printf("Number of threads: %d\n", omp_get_num_threads());
    }
    printf("Sequential part 2\n");
    printf("Hello from thread %d\n", omp_get_thread_num());
    printf("Number of threads: %d\n", omp_get_num_threads());

}
