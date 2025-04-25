#include <math.h>
#include <iostream>
#include <omp.h>
#include <fstream>
long long N;



int main()
{
    int N;

    std::fstream file("N.dat", std::ios_base::in);
    file >> N;
    
    int number_of_threads = omp_get_max_threads();
    std::cout << "Number of threads: " << number_of_threads << std::endl;
    omp_set_num_threads(number_of_threads);
    double result = 0.0;

    double dx = 2.0/N;
    #pragma omp parallel for reduction(+:result)
        for (int i = 0; i<N; i++) {
        double x=dx*i;
        result+=dx*(sqrt(4-x*x)+sqrt(4-(x+dx)*(x+dx)))/2;
    }

    printf("Result: %f\n", result);
    return 0;
}
