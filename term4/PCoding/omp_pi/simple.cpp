#include <math.h>
#include <iostream>
#include <omp.h>
#include <fstream>
long long N;

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


int main()
{
    int N;

    std::fstream file("N.dat", std::ios_base::in);
    file >> N;
    
    int number_of_threads = omp_get_num_procs();
    std::cout << "Number of threads: " << number_of_threads << std::endl;
    omp_set_num_threads(number_of_threads);
    double result = 0.0;
    #pragma omp parallel reduction(+:result)
    {
    int n = omp_get_thread_num();

    int beg = (int)(((double)N)/number_of_threads*n);
    int end = (int)(((double)N)/number_of_threads*(n+1));
    result += calc(N, beg, end);
    }
    printf("Result: %f\n", result);
    return 0;
}