#include <any>
#include <iostream>
#include <string>

template <typename T>
bool try_print(std::any&  chto_eto)
{
    if(chto_eto.type() == typeid(T))
    {
        std::cout << std::any_cast<T>(chto_eto) << std::endl; 
        return true;
    }
    else 
    {return false;}
}   


void print(std::any&  chto_eto)
{
    if(try_print<std::string>(chto_eto) | try_print<int>(chto_eto) | try_print<float>(chto_eto)) {}
    else if (chto_eto.type() == typeid(const char*))
    {
        std::cout << "ну ты конч конечно строки так хранить\n сколько тебе букв вывести? ";
        int n;
        std::cin >> n;
        for(int i = 0; i<n; i++)
        {
            std::cout << std::any_cast<const char*>(chto_eto)[i];
        }
        std::cout << std::endl;
    }
    else{
        std::cout << "мы не знаем что это такое :(" << std::endl;
    }

}

int main()
{
    std::any ti_kto = std::string("кто ты");


    std::cout << std::any_cast<std::string>(ti_kto) << std::endl;

    ti_kto = 42.0;

    try
    {    
        std::cout << std::any_cast<int>(ti_kto) << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "мы ошиблись, это не тот кто мы думаем! и получили в лицо " << e.what() << '\n';
    }

    ti_kto = "Я Аянами Рей";

    print(ti_kto);
    
    ti_kto = std::string("имхо херня какая-то");
    print(ti_kto);
    




    return 0;
}