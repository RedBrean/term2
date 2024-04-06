#include <iostream>
#include <math.h>
#include <string>
using std::cout;
using std::cin;
using std::string;

int main() {
    string s1;
    string s2;
    std::cin >> s1 >> s2;
    cout << s2.substr(6, -1);
}