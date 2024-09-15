#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::cout;
using std::cin;

struct Time
{
    int hh = 0;
    int mm = 0;
    int ss = 0;
    Time operator + (Time B)
    {
        Time output = {.hh = hh+B.hh, .mm = mm + B.mm, .ss = ss + B.ss};
        if(output.ss > 59){
            output.ss -= 60;
            output.mm++;
        }
        if(output.mm > 59){
            output.mm -= 60;
            output.hh++;
        }
        if(output.hh > 23){
            output.hh = output.hh%24;
        }
        return output;
    }
    void print()
    { 
        if(hh < 10) {cout << '0'; }
        cout << hh << ':';
        if(mm < 10) {cout << '0'; }
        cout << mm << ':';
        if(ss < 10) {cout << '0'; }
        cout << ss;
    }
};



int main()
{
    Time now;
    Time until;
    cin >> now.hh >> now.mm >> now.ss >> until.hh >> until.mm >> until.ss;
    (now + until).print();
}