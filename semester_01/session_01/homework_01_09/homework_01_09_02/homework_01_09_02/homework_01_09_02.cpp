// homework_01_09_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

namespace bag {
    void swap(int*, int*);
    void swap(double*, double*);
}

int main()
{
    using std::cout;
    {
        int a{ 10 };
        int* pa{ &a };
        int b{ 100 };
        int* pb{ &b };

        cout << "Обмен данных типа 'int'\n";
        cout << "a = " << a << "\tb = " << b << '\n';
        bag::swap(pa, pb);
        cout << "a = " << a << "\tb = " << b << '\n';
    }

    {
        double a{ 10.01 };
        double* pa{ &a };
        double b{ 100.01 };
        double* pb{ &b };

        cout << "Обмен данных типа 'double'\n";
        cout << "a = " << a << "\tb = " << b << '\n';
        bag::swap(pa, pb);
        cout << "a = " << a << "\tb = " << b << '\n';
    }
    
    return 0;
}

namespace bag {
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