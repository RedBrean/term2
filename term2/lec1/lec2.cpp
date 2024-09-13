#include <iostream>


inline //Агрессивная оптимищация. Заменяет вызов функции сразу на ее тело
int my_max(int a, int b) { 
    return a>b ? a : b;
}

int main(){
    unsigned x = 3, y = 5;
    std::cout << my_max(x, y) << std::endl;

    return 0;

}