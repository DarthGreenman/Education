// homework_01_03_02.cpp :
// Напишите программу, которая запрашивает у пользователя три целых числа и
// выводит их на экран в порядке убывания

#include <iostream>
#include <locale>

using namespace std;

struct value_weight {
	int max;
	int mid;
	int min;
	value_weight () : max{0}, mid{0}, min{0} {}
};

int main()
{
	setlocale(LC_ALL, "Russian");

	int A{ 0 };
	int B{ 0 };
	int C{ 0 };

	cout << "Введите три разных, целых числа.\n";
	cout << "Введите первое число : ";
	cin >> A;
	
	do {
		cout << "Введите второе число: ";
		cin >> B;
	} while (B == A);
	
	do
	{
		cout << "Введите третье число: ";
		cin >> C;
	} while (C == B || C == A);
	
	value_weight val;

	cout << "\nРезультат :\t";
	if (A > B && B > C) {
		val.max = A;
		val.mid = B;
		val.min = C;
	} 
	else if (B > C && C > A) {
		val.max = B;
		val.mid = C;
		val.min = A;
	}
	else if (C > A && A > B) {
		val.max = C;
		val.mid = A;
		val.min = B;
	}
	else if (A > C && C > B) {
		val.max = A;
		val.mid = C;
		val.min = B;
	}
	else if (B > A && A > C) {
		val.max = B;
		val.mid = A;
		val.min = C;
	}
	else /* if (c > b && b > a) */ {
		val.max = C;
		val.mid = B;
		val.min = A;
	}
	cout << val.max << '\t' << val.mid << '\t' << val.min << '\n';
	return 0;
}