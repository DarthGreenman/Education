// tridiagonal_matrix.h

#ifndef TRIDIAGONAL_MATRIX_H
#define TRIDIAGONAL_MATRIX_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>

class tridiagonal_matrix
{
public:
    using point = std::vector<double>;

    friend std::ostream& operator<<(std::ostream& Os,
        const tridiagonal_matrix& Matrix);

    tridiagonal_matrix(const point& down, const point& upper, const point& middle) :
        m_down{ down }, m_upper{ upper }, m_middle{ middle }
    {};
    ~tridiagonal_matrix() { std::cout << "destructor called\n"; }

    std::unique_ptr<tridiagonal_matrix> clone() const {        
        return 
            std::make_unique<tridiagonal_matrix>(m_down, m_upper, m_middle);
    }

private:
    point m_down{};
    point m_upper{};
    point m_middle{};
};

void point_print(const typename tridiagonal_matrix::point& Point)
{
    using Value_type = 
        typename tridiagonal_matrix::point::value_type;
    std::for_each(std::cbegin(Point), std::cend(Point),
        [](const Value_type& Elem) {
        std::cout << std::setw(4) << Elem << ' ';
    });
}

std::ostream& operator<<(std::ostream& Os, const tridiagonal_matrix& Matrix)
{
    using std::cout;
    cout << "Adress: " << &Matrix;
    cout << "\nupper:\t";   point_print(Matrix.m_upper);
    cout << "\ndown:\t";    point_print(Matrix.m_down);
    cout << "\nmiddle:\t";  point_print(Matrix.m_middle);
    return Os;
}

#endif // TRIDIAGONAL_MATRIX_H