#include "curricula.hpp"
#include "decimal.hpp"

using namespace std;
using namespace decimal;

TESTS_BEGIN

TEST(initialize_empty)
{
    Decimal<4> a;
    PASS
}

TEST(initialize_string_zero)
{
    Decimal<4> b = Decimal<4>::from_string("0");
    PASS
}

TEST(initialize_string_integer)
{
    Decimal<4> a = Decimal<4>::from_string("12345");
    Decimal<4> b = Decimal<4>::from_string("1000000");
    Decimal<4> c = Decimal<4>::from_string("999");
    PASS
}

TEST(initialize_string_integer_negative)
{
    Decimal<4> a = Decimal<4>::from_string("-1");
    Decimal<4> b = Decimal<4>::from_string("-1234567");
    PASS
}

TEST(initialize_string_decimal)
{
    Decimal<4> a = Decimal<4>::from_string("123.0000");
    Decimal<4> b = Decimal<4>::from_string("1234.1234");
    Decimal<4> c = Decimal<4>::from_string("-3.3333");
    PASS
}

TEST(initialize_string_decimal_short)
{
    Decimal<4> a = Decimal<4>::from_string("5.0");
    Decimal<4> b = Decimal<4>::from_string("-3.333");
    PASS
}

TEST(initialize_string_decimal_long)
{
    Decimal<4> a = Decimal<4>::from_string("1.00000");
    Decimal<4> b = Decimal<4>::from_string("3.12345");
    Decimal<4> c = Decimal<4>::from_string("-9.99999999");
    PASS
}

TESTS_END
