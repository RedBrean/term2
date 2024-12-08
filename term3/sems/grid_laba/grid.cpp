#include <cassert>
#include <vector>
template<typename T, unsigned dim = 2>
class Grid;

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

    T operator[](size_type idx)
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
        std::vector<Grid<T,dim-1>> vec_grid;
    public:
    template<typename... Suki>
    Grid(size_type size_last, Suki ...suki) 
    {
        vec_grid = std::vector<Grid<T,dim-1>(size_last, Grid<T,dim-1>(suki...));
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

    //g2 = g3[1];
    assert(1.0f == g2(1, 1));
    return 0;
}