#ifndef GRID3_HPP
#define GRID3_HPP

#include <vector>
#include <cassert>
#include <initializer_list>

template <typename T, std::size_t D>
class Grid;

//для одномерного массива (D == 1)
template <typename T>
class Grid<T, 1> {
public:
    using size_type = unsigned;
private:
    std::vector<T>* data;
public:
    Grid(size_type size, T const& value = T()) {data=new std::vector<T>(size,value);}
    Grid(const Grid& other) {data=new std::vector<T>(*other.data);}
    Grid& operator=(const Grid& other){
        if (this != &other){
            *data = *other.data;
        }
        return *this;
    }
    Grid(Grid&& other) noexcept : data(std::move(other.data))
    {
    }
    Grid& operator=(Grid&& other) noexcept {
        if (this != &other){
            delete(data);
            data = std::move(other.data);
        }
        return (*this);
    }
    ~Grid()
    {
        delete(data);
    };

    T& operator()(size_type idx) { return (*data)[idx]; }
    T& operator[](size_type idx) { return (*data)[idx]; }
    const T& operator()(size_type idx) const { return (*data)[idx]; }
    const T& operator[](size_type idx) const { return (*data)[idx]; }

    size_type get_size() { return data->size(); }
};



// Шаблонный класс Grid для многомерных массивов произвольной размерности
template <typename T, std::size_t D>
class Grid {
public:
    using size_type = unsigned;  // Определяем тип для размера

private:
    std::vector<Grid<T, D - 1>> data;  // Вектор для хранения срезов массива

public:
    // Конструктор, который инициализирует многомерный массив значениями
//    explicit Grid(size_type size, T const& value = T()) {
//         // data.resize(size);
//         // for(int i = 0; i<data.size(); i++)
//         // {
//         //     data[i] = Grid<T, D - 1>(size, value);
//         // }
//         data.resize(size, Grid<T, D - 1>(size, value));
//     } 
    template<typename... Mazurki>
       explicit Grid(size_type size, Mazurki ...mazurki) {
        data.resize(size, Grid<T, D - 1>(mazurki...));
    } 


    // Оператор индексации для доступа к срезам массива
    Grid<T, D - 1>& operator[](size_type idx) {
        if (idx >= data.size()) throw std::out_of_range("Index out of range"); // Проверка выхода за пределы
        return data[idx];  // Возврат ссылки на срез
    }

    const Grid<T, D - 1>& operator[](size_type idx) const {
        if (idx >= data.size()) throw std::out_of_range("Index out of range"); // Проверка выхода за пределы
        return data[idx];  // Возврат ссылки на срез
    }
    auto& operator()(size_type idx) {
        if (idx >= data.size()) throw std::out_of_range("Index out of range"); // Проверка выхода за пределы
        return data[idx];  // Возврат ссылки на срез
    }
    template<typename...  Polki>
    auto& operator() (size_type idx, Polki  ...polki) {
        if (idx >= data.size()) throw std::out_of_range("Index out of range"); // Проверка выхода за пределы
        return data[idx](polki...);

    } 
    // Метод для получения размера массива
    size_type get_size() const {
        return data.size();
    }
};




#endif