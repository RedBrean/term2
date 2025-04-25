#include <stdio.h>
#include <omp.h>

int main(){

#pragma omp parallel num_threads(5)
    {

        #pragma omp sections
        {
            #pragma omp section
            {
                printf("Section 1; Hello from thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("Section 2; Hello from thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("Section 3; Hello from thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("Section 4; Hello from thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("Section 5; Hello from thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("Section 6; Hello from thread %d\n", omp_get_thread_num());
            }
        }
        printf("Not sections; Hello from thread %d\n", omp_get_thread_num());

    }
    return 0;
}
