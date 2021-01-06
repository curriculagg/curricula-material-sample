#include "arithmetic.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

int main()
{
    string line;
    while (getline(cin, line))
    {
        try
        {
            cout << evaluate(line) << endl;
        }
        catch (logic_error& e)
        {
            cout << e.what() << endl;
        }
    }
}
