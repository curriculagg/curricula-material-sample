#include <iostream>
#include "decimal.hpp"

using namespace std;

int main()
{
    Decimal<6> a("3.333333");
    Decimal<6> b("7.123456");
    Decimal<6> c("4121");
    Decimal<6> d("5");
//    cout << b << ", " << c << endl;
    cout << b / c << endl;
    cout << c / b << endl;
    cout << b * c << endl;
    cout << a << " * " << b << endl;
    cout << d * d << endl;

//    uint64_t x = 10000000000000;
//    Decimal<6> y("100000.00000000");
//    cout << x * x << endl;
//    cout << y * y << endl;
}
