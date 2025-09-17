#include <stdio.h>
#include <omp.h>

int main(){

#pragma omp parallel
    {
        #pragma omp single nowait
        {
            printf("Single 1; Hello from thread %d\n", omp_get_thread_num());
        }
        printf("Not single; Hello from thread %d\n", omp_get_thread_num());
        #pragma omp single
        {
            printf("Single 2; Hello from thread %d\n", omp_get_thread_num());
        }


    }
    return 0;
}
