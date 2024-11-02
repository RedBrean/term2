#include <cassert>

template<typename T, unsigned dim = 2>
class Grid;

template<typename T>
class Grid<T, 2>
{
public:
    using value_type = T;
    using size_type = unsigned;
private:
    T* data;
    size_type y_size, x_size;
public:
    Grid(T *data , size_type y_size , size_type x_size)
    : data(data)
    , y_size(y_size)
    , x_size(x_size)
    {
    }

    Grid(T const &t)
    {
        data = new T[1];
        *data = t;
        y_size, x_size = 1;
    }
    Grid(size_type y_size, size_type x_size) : data(new T[x_size*y_size]), x_size(x_size), y_size(y_size)
    {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it) {
            *it = T();
        }
    }

    Grid(size_type y_size, size_type x_size, T const &t) : data(new T[x_size*y_size]), x_size(x_size), y_size(y_size)
    {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it){
            *it = t;
        }
    }

    Grid(Grid<T> const& other) : data(new T[x_size*y_size]), x_size(x_size), y_size(y_size){
        for(size_type x_idx = 0; x_idx!=x_size; x_idx++){
        for(size_type y_idx = 0; y_idx!=y_size; y_idx++)
        {
            data(y_idx,x_idx) = other(y_idx, x_idx);
        }
        }
    }
    Grid(Grid<T>&& other)  : data(other.data), x_size(other.x_size), y_size(other.y_size) {
        other.data = nullptr;        
    }

    ~Grid(){
        delete[] data;
    }


    Grid<T>& operator=(Grid<T>& other){
        x_size = other.x_size;
        y_size = other.y_size;
        data = new T[x_size*y_size];
        for(size_type x_idx = 0; x_idx!=x_size; x_idx++){
        for(size_type y_idx = 0; y_idx!=y_size; y_idx++)
        {
            data(y_idx,x_idx) = other(y_idx, x_idx);
        }
        }
    }
    Grid<T>& operator=(Grid<T>&& other){
        x_size = other.x_size;
        y_size = other.y_size;
        data = new T[x_size*y_size];
        for(size_type x_idx = 0; x_idx!=x_size; x_idx++){
        for(size_type y_idx = 0; y_idx!=y_size; y_idx++)
        {
            data(y_idx,x_idx) = other(y_idx, x_idx);
        }
        }
    }

    T* operator[](size_type y_idx)
    {
        return &data[x_size*y_idx];
    }

    T operator()(size_type y_idx , size_type x_idx) const
    {
        return data[y_idx * x_size + x_idx];
    }

    T& operator()(size_type y_idx, size_type x_idx)
    {
        return data [y_idx * x_size + x_idx];
    }

    Grid<T>& operator=(T const& t)
    {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it) *it = t;
        return *this;
    }

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
};


template<typename T>
class Grid<T, 1>
{
public:
    using value_type = T;
    using size_type = unsigned;
private:
    T* data;
    size_type size;
public:
    Grid(T *data , size_type size)
    : data(data)
    , size(size)
    {
    }

    Grid(T const &t)
    {
        data = new T[1];
        *data = t;
        size = 1;
    }
    Grid(size_type size) : data(new T[size]), size(size)
    {
        for (auto it = data, end = data + size; it != end; ++it) {
            *it = T();
        }
    }

    Grid(size_type size, T const &t) : data(new T[size]), size(size)
    {
        for (auto it = data, end = data + size; it != end; ++it){
            *it = t;
        }
    }

    Grid(Grid<T> const& other) : data(new T[size]), size(size){
        for(size_type idx = 0; idx!=size; idx++){
            data(idx) = other(idx);
        }
    }
    Grid(Grid<T>&& other)  : data(other.data), size(other.size) {
        other.data = nullptr;        
    }

    ~Grid(){
        delete[] data;
    }


    Grid<T>& operator=(Grid<T>& other){
        size = other.size;
        data = new T[size];
        for(size_type idx = 0; idx!=size; idx++){
            data(idx) = other(idx);
        }
    }
    Grid<T>& operator=(Grid<T>&& other){
        size = other.size;
        data = new T[size];
        for(size_type idx = 0; idx!=size; idx++){
            data(idx) = other(idx);
        }
    }

    T* operator[](size_type idx)
    {
        return &data[idx];
    }

    T operator()(size_type idx) const
    {
        return data[idx];
    }

    T& operator()(size_type idx)
    {
        return data [idx];
    }

    Grid<T>& operator=(T const& t)
    {
        for (auto it = data, end = data + size; it != end; ++it) *it = t;
        return *this;
    }

    size_type get_size() const { return size; }
};

template <typename T, unsigned dim>
class Grid
{
    public:
        using value_type = T;
        using size_type = unsigned;
    private:
        Grid<Grid<T,dim-1>,1> grid_gridov;
    public:
    template<typename... Suki>
    Grid(size_type size_last, Suki ...suki) 
    {
        grid_gridov = Grid<Grid<T,dim-1>,1>(size_last, Grid<T,dim-1>(suki...));
    }
    
    
    auto& operator[](size_type idx)
    {
        return *grid_gridov[idx];
    }

    template<typename... Suki>
    T operator()(size_type idx, Suki ...suki) const
    {
        return grid_gridov[idx](suki...);
    }
    template<typename... Suki>
    T& operator()(size_type idx,  Suki ...suki) 
    {
        return grid_gridov[idx](suki...);
    }
    T operator()(size_type idx) const
    {
        return grid_gridov[idx];
    }
    T& operator()(size_type idx) 
    {
        return grid_gridov[idx];
    }
    
};


int main()
{
    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());

    using gsize_t = Grid<int>::size_type;

    for(gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for(gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
        assert(0.0f == g[y_idx][x_idx]);

    for(gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for(gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
        g[y_idx][x_idx] = 1.0f;

    for(gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for(gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
        assert(1.0f == g(y_idx , x_idx));

    Grid<float, 3> const g3(2, 3, 4, 1.0f);
    assert(1.0f == g3(1, 1, 1));

    Grid<float, 2> g2(2, 5, 2.0f);
    assert(2.0f == g2(1, 1));

    g2 = g3[1];
    assert(1.0f == g2(1, 1));
    return 0;
}