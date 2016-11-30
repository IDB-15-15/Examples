#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <utility>
#include <boost/function_output_iterator.hpp>
#include <boost/iterator/function_input_iterator.hpp>

class iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;

    iterator(int* ptr)
        : ptr {ptr}
    {}

    iterator& operator =(const iterator &other) {
        ptr = other.ptr;
        return *this;
    }

    int& operator *() {
        return *ptr;
    }

    iterator& operator ++() {
        ++ptr;
        return *this;
    }

    iterator& operator ++(int) {
        ++ptr;
        return *this;
    }

    iterator& operator --() {
        --ptr;
        return *this;
    }

    iterator& operator --(int) {
        --ptr;
        return *this;
    }

    friend bool operator ==(const iterator& lhs, const iterator& rhs) {
        return rhs.ptr == lhs.ptr;
    }

    friend bool operator !=(const iterator& a, const iterator& b) {
        return !(a == b);
    }

private:
    int* ptr;
};

int main() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Итерация по C-style массиву
    for (iterator it {arr}, end {arr + 10}; it != end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // range-based for
    for (auto x : arr) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    // Использование связки из std::copy и std::ostream_iterator для вывода чисел через запятую
    std::copy(arr, arr + 10, std::ostream_iterator<int> {std::cout, ", "});
    std::cout << std::endl;

    // Вывод чисел через пробел с помощью std::for_each
    std::for_each(
        arr, arr + 10,
        [] (int x) {
            std::cout << x << ' ';
        }
    );
    std::cout << std::endl;

    // Заполнение массива с помощью std::generate и лямбда-функции
    int i = 0;
    std::generate(arr, arr + 10, [&] { ++i; return i * 2; });
    std::copy(arr, arr + 10, std::ostream_iterator<int> {std::cout, " "});
    std::cout << std::endl;

    // std::iota заполняет массив числами 100-109
    std::iota(arr, arr + 10, 100);
    std::copy(arr, arr + 10, std::ostream_iterator<int> {std::cout, " "});
    std::cout << std::endl;

    auto f = [](int x) {
        std::cout << x << ' ';
    };

    std::copy(
        arr, arr + 10,
        // Создает output_iterator из фунции
        boost::make_function_output_iterator(f)
    );
    std::cout << std::endl;


    // генератор - объект с объявленным result_type и оператором ()
    struct generator {
        using result_type = int;

        result_type operator ()() const {
            return 100500;
        }
    };

    generator gen;
    std::copy(
        // Создает input_iterator из функтора
        boost::make_function_input_iterator(gen, 0), boost::make_function_input_iterator(gen, 10),
        arr
    );
    std::copy(arr, arr + 10, std::ostream_iterator<int> {std::cout, " "});
    std::cout << std::endl;


    // Использование std::begin и std::end (тоже самое, что и .begin(), .end(), но можно использовать на массивы)
    for (auto it = std::begin(arr), end = std::end(arr); it != end; ++it) {
        auto x = *it;

        std::cout << x << ' ';
    }
    std::cout << std::endl;

    std::cout << "Press Ctrl+D for end" << std::endl;
    // Пример использования istream_iterator-a (считываение из std::cin до EOF)
    int* p = arr;
    for (std::istream_iterator<int> it {std::cin}, end; it != end; ++it, ++p) {
        *p = *it;
    }
    std::copy(arr, arr + 10, std::ostream_iterator<int> {std::cout, " "});
    std::cout << std::endl;


    return 0;
}
