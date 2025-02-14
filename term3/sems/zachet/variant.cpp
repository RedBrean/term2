#include <any>
#include <variant>
#include <iostream>
#include <string>
#include <math.h>


// helper type for the visitor #4
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

long long encode_string(const std::string& str) {
    long long result = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        result += static_cast<long long>(str[i]) * static_cast<long long>(std::pow(256, i));  // Преобразуем каждый символ
    }
    return result;
}

std::string decode_string(long long encoded) {
    std::string decoded;
    while (encoded > 0) {
        decoded.push_back(static_cast<char>(encoded % 256));  // Восстанавливаем символы
        encoded /= 256;
    }
    return decoded;
}

void check_eva(const std::variant<std::string, long long>& eva)
{
    std::visit([](auto&& evangelion)
    {std::cout << "В Евангелионе сидит " << evangelion << std::endl;}, eva);
}

void check_eva_2(const std::variant<std::string, long long>& eva)
{
    std::visit(overloaded{
    [](std::string evangelion) {std::cout << "В Евангелионе сидит " << evangelion << std::endl;},
    [](long long evangelion) {std::cout << "В Евангелионе сидит " << decode_string(evangelion) << std::endl;}
    }, eva);
}

void repair_pilot(std::variant<std::string, long long>& eva)
{
    if(std::holds_alternative<long long>(eva))
    {
        eva = decode_string(std::get<long long>(eva)); 
    }
}

int main()
{
    std::variant<std::string, long long> eva_01;
    std::variant<std::string, long long> eva_02;
    eva_01 = "Shinji";
    eva_02 = "Asuka";
    check_eva(eva_02);
    check_eva(eva_01);
    std::cout << "Добъемся 100% синхронизации? \n Y/N: ";
    char sync;
    std::cin >> sync;

    if(sync == 'Y' || sync == 'y'){
        eva_01 = encode_string(std::get<std::string>(eva_01));
    }
    std::string* pilot_adress = std::get_if<std::string>(&eva_01);

    check_eva_2(eva_01);
    check_eva(eva_01);

    if(pilot_adress == nullptr)
    {
        std::cout << "Емае, он расстворился\n Ща починим\n";
        repair_pilot(eva_01);
        repair_pilot(eva_02);
        check_eva(eva_01);
        
    }
    else
    {
        std::cout << "Вроде все живы, в еве-01 все еще сидит " << *pilot_adress << std::endl; 
    }

    return 0;
}