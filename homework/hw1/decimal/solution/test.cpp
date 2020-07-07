#include "decimal.hpp"
#include <iostream>

using namespace std;
using namespace decimal;

int main()
{
    Decimal<4> a = Decimal<4>("1234");
    Decimal<4> b = Decimal<4>("-50");
    cout << Decimal<4>("0.6666") << endl;
    cout << Decimal<4>("6") << endl;
    cout << b - a << endl;
    cout << Decimal<4>("-1284")  << endl;
    Decimal<4> c = Decimal<4>("-0.04051863857374392220421393841");
    cout << b / a << endl;
    cout << c << endl;
}
