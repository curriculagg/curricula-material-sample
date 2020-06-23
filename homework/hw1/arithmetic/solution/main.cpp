#include "arithmetic.hpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string line;
    while (getline(cin, line))
    {
        std::cout << line << " : " << compute(line) << std::endl;
    }
}
