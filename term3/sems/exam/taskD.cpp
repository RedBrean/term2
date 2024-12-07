template <typename T, typename... Args>
T polynom(T x0, Args... c);

template <typename T, typename U>
T polynom(T x0, U c) {
    return c;
}

template <typename T, typename U, typename... Args>
T polynom(T x0, U c, Args... rest) {
    return c + x0 * polynom(x0, rest...);  
}



#include <iostream>
int main() {
    std::cout << polynom(2, 1) << std::endl;       // 1
    std::cout << polynom(2, 1, 2) << std::endl;    // 5
    std::cout << polynom(2, 1, 2, 1) << std::endl; // 9
    return 0;
}
