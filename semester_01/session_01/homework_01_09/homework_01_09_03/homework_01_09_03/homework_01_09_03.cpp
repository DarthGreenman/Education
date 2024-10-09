// homework_01_09_03.cpp

#include <iostream>

namespace bag {
    void print(const int[], const size_t);
    void print(const double[], const size_t);
    
    void reverse(int[], const size_t);
    void reverse(double[], const size_t);
    
    void swap(int&, int&);
    void swap(double&, double&);
    void swap(int*, int*);
    void swap(double*, double*);
}

int main()
{
    using std::cout;
    double arr[]{ 2.6, 6.7, -10.0, 1.05, 0.567, 6.7, 356.1 };
    const size_t size{ sizeof(arr) / sizeof(arr[0]) };

    cout << "До функции reverse:\t";
    bag::print(arr, size);

    bag::reverse(arr, size);

    cout << "\nПосле функции reverse:\t";
    bag::print(arr, size);

    return 0;
}

namespace bag {
    void print(const int array[], const size_t size) {
        for (size_t i{ 0 }; i < size; ++i) {
            std::cout << array[i] << ' ';
        }
    }

    void print(const double array[], const size_t size) {
        for (size_t i{ 0 }; i < size; ++i) {
            std::cout << array[i] << ' ';
        }
    }

    void reverse(int array[], const size_t size) {
        for (size_t is{ 0 }, it{ size }, N{ size / 2 }; is < N; ++is, --it) {
            if (array[is] != array[it - 1]) {
                int tmp{ array[is] };
                bag::swap(array[is], array[it - 1]);
            }
        }
    }

    void reverse(double array[], const size_t size) {
        for (size_t is{ 0 }, it{ size }, N{ size / 2 }; is < N; ++is, --it) {
            if (array[is] != array[it - 1]) {
                double tmp{ array[is] };
                bag::swap(array[is], array[it - 1]);
            }
        }
    }

    void swap(int& lhs, int& rhs) {
        int tmp{ lhs };
        lhs = rhs;
        rhs = tmp;
    }

    void swap(double& lhs, double& rhs) {
        double tmp{ lhs };
        lhs = rhs;
        rhs = tmp;
    }

    void swap(int* lhs, int* rhs) {
        int tmp{ *lhs };
        *lhs = *rhs;
        *rhs = tmp;
    }

    void swap(double* lhs, double* rhs) {
        double tmp{ *lhs };
        *lhs = *rhs;
        *rhs = tmp;
    }
}