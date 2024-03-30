#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

void vector_print(vector<int> vec)
{
    cout << "[";
    for (int i = 0; i<vec.size(); i++)
    {
        cout << vec[i]<<" ";
    }
    cout<< "]\n";
}

vector<int> monets{1, 7, 28, 49, 63, 98};

int main()
{
    int M;
    cin >> M;
    int answer = 0;
    for(int i = 5; i>=0; i--){
        answer+=M/monets[i];
        M = M%monets[i];
    }
    cout << answer;
}