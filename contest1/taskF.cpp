#include <iostream>
#include <string>
#include <algorithm>

int main()
{
    int a;
    std::cin >> a;
    std::string str = std::to_string(a);
    std::string strR = str;
    std::reverse(strR.begin(), strR.end()); //Как прикольно что я это помню с 8 класса школы
    std::cout << (strR.compare(str) == 0);

}