template <typename T, unsigned N>
class Array
{
public: 
    // Список операций:
    //
    explicit Array(const T&);
    //   конструктор класса, который создает
    //   Array размера size, заполненный значениями
    //   value типа T. Считайте что у типа T есть
    //   конструктор, который можно вызвать без
    //   без параметров, либо он ему не нужен.
    //
    Array(const Array &);
    //   конструктор копирования, который создает
    //   копию параметра. Считайте, что для типа
    //   T определен оператор присваивания.
    //
    ~Array();
    //   деструктор, если он вам необходим.
    //
    operator=;
    //   оператор копирующего присваивания.
    //
    size_t size() const;
    //   возвращает размер массива (количество
    //                              элементов).
    //
    operator[];
    operator[];
    //   две версии оператора доступа по индексу.
    front();
    // получение первого элемента
    back();
    // получение последнего элемента
    empty();
    // проверка пустой ли массив
    fill(const T& value);
    // заполение массива значением value
private:
    // поля определите сами
};