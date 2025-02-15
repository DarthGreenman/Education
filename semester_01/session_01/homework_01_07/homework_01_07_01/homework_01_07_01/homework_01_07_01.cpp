// homework_01_07_01.cpp

#include <iostream>
using namespace std;

int main()
{
    int         x_in{ 0 };
    short       x_sh{ 0 };
    long        x_lg{ 0 };;
    long long   x_ll{ 0 };
    float       x_fl{ 0 };
    double      x_dl{ 0 };
    long double x_ld{ 0 };
    bool        x_bl{ false };

    cout << typeid(x_in).name() << "\t\t" << &x_in << '\t' << sizeof(x_in) << '\n';
    cout << typeid(x_sh).name() << "\t\t" << &x_sh << '\t' << sizeof(x_sh) << '\n';
    cout << typeid(x_lg).name() << "\t\t" << &x_lg << '\t' << sizeof(x_lg) << '\n';
    cout << typeid(x_ll).name() << "\t\t" << &x_ll << '\t' << sizeof(x_ll) << '\n';
    cout << typeid(x_fl).name() << "\t\t" << &x_fl << '\t' << sizeof(x_fl) << '\n';
    cout << typeid(x_dl).name() << "\t\t" << &x_dl << '\t' << sizeof(x_dl) << '\n';
    cout << typeid(x_ld).name() << '\t' << &x_ld << '\t' << sizeof(x_ld) << '\n';
    cout << typeid(x_bl).name() << "\t\t" << &x_bl << '\t' << sizeof(x_bl) << '\n';

    return 0;
}