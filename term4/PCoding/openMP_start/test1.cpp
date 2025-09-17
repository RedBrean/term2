#include <stdio.h>
int x0 = 2;
int main(){
    int x1 = 2;
#pragma omp parallel shared(x0) private(x1)
    { 
      	int x2 = 0; static int x3 = 0;
	x0++; x1++; x2++; x3++;
	printf("Par x0=%d x1=%d x2=%d x3=%d\n",x0,x1,x2,x3); 
    }
    printf("Seq x0=%d x1=%d\n",x0,x1); 
}
