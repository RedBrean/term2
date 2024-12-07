#include <iostream>
#include <thread>
#include <iomanip>
#include <string>
#include <chrono>

using std::string;


int main()
{
    int perc = 0;
    int cont = 0;
    string load = "";

    std::cout << "\033[?25l";

    for(int i=0;i<200;i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        perc += 1;
        if(perc > load.length()*10 + 5)
        {
            load += "|";
        }

        std::cout << "\r\033[36m" << std::left << std::setw(10) << std::setfill('-') << load << "   " << perc << "%" <<"\033[0m";

        if(perc>100)
        {
            std::cout << "\n \033[31mOh shit, I had to stop...\033[0m" << "\033[A";
        }

        if(perc>150)
        {
            std::cout << "\n \n \033[33mIm trying\033[0m" << "\033[A" << "\033[A";
        }

        std::cout << std::flush;

    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "\033[2K \n \033[2K \n \033[2K \033[A \033[A \r"; 

    std::cout << "\033[35m Yey!\033[0m \033[?25h" << std::endl;
    return 0;
}