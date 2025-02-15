// homework_01_08_02.cpp 

#include <iostream>

namespace math {
    namespace add {
        int calc(int, int);
    }

    namespace sub {
        int calc(int, int);
    }

    namespace mul {
        int calc(int, int);
    }

    namespace div {
        int calc(int, int);
    }
}

int main()
{
    int x{ 6 };
    int y{ 9 };

    using std::cout;
    cout << "Исходные данные:\n";
    cout << "x = " << x << ", y = " << y;

    using namespace math;
    cout << "\nСложение:\t" << add::calc(x, y) ;
    cout << "\nВычитание:\t" << sub::calc(x, y);
    cout << "\nУмножение:\t" << mul::calc(x, y);
    cout << "\nДеление:\t" << div::calc(x, y);

    return 0;
}


int math::add::calc(int a, int b) {
    return a + b;
}

int math::sub::calc(int a, int b) {
    return a - b;
}

int math::mul::calc(int a, int b) {
    return a * b;
}

int math::div::calc(int a, int b) {
    return a / b;
}