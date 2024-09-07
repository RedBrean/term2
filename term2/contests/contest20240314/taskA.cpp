#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::cout;
using std::cin;

struct Student
{
    string name;
    int ball;
};


int main()
{
    int n;
    cin >> n;
    string name;
    int ball;
    
    vector<Student> students(n);

    for (int i = 0; i<n; i++)
    {
        cin >> name >> ball;
        students[i].name = name;
        students[i].ball = ball;
    }

    long long sum = 0;
    for (int i = 0; i<n; i++)
    {
        sum += students[i].ball;
    }
    cout << sum/n;
}