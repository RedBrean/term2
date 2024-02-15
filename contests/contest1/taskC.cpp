#include <iostream>

int fuck(int n) //ФАКториал типа
{
    if(n <= 0) {return 1;}
    else{
        return fuck(n-1) * n;
    }
}


int main()
{
    int n;
    std::cin >> n;
    std::cout << fuck(n);

}