#include "curricula.hpp"
#include "decimal.hpp"

using namespace std;
using namespace decimal;

TESTS_BEGIN

TEST(initialize_empty)
{
    Decimal<4> a;
    PASS;
}

TEST(initialize_string_zero)
{
    Decimal<4> b = Decimal<4>("0");
    PASS;
}

TEST(initialize_string_integer)
{
    Decimal<4> a = Decimal<4>("12345");
    Decimal<4> b = Decimal<4>("1000000");
    Decimal<4> c = Decimal<4>("999");
    PASS;
}

TEST(initialize_string_integer_negative)
{
    Decimal<4> a = Decimal<4>("-1");
    Decimal<4> b = Decimal<4>("-1234567");
    PASS;
}

TEST(initialize_string_decimal)
{
    Decimal<4> a = Decimal<4>("123.0000");
    Decimal<4> b = Decimal<4>("1234.1234");
    Decimal<4> c = Decimal<4>("-3.3333");
    PASS;
}

TEST(initialize_string_decimal_short)
{
    Decimal<4> a = Decimal<4>("5.0");
    Decimal<4> b = Decimal<4>("-3.333");
    PASS;
}

TEST(initialize_string_decimal_long)
{
    Decimal<4> a = Decimal<4>("1.00000");
    Decimal<4> b = Decimal<4>("3.12345");
    Decimal<4> c = Decimal<4>("-9.99999999");
    PASS;
}

TEST(initialize_string_decimal_round)
{
    Decimal<4> a = Decimal<4>("6.66666");
    Decimal<4> b = Decimal<4>("6.6667");
    EXPECT_TRUE(a == b);
    Decimal<4> c = Decimal<4>("-6.66666");
    Decimal<4> d = Decimal<4>("-6.6667");
    EXPECT_TRUE(c == d);
    PASS;
}

TEST(initialize_decimal)
{
    Decimal<4> a = Decimal<4>("6.6666");
    Decimal<4> b(a);
    EXPECT_EQUAL(a, b);
    PASS;
}

TEST(operator_equal)
{
    Decimal<4> a = Decimal<4>("1.0000");
    Decimal<4> b = Decimal<4>("1.0000");
    EXPECT_TRUE(a == b);
    PASS;
}

TEST(operator_equal_different_radix)
{
    Decimal<4> a = Decimal<4>("1.00");
    EXPECT_TRUE(a == a);
    Decimal<4> b = Decimal<4>("1.0000");
    EXPECT_TRUE(a == b);
    Decimal<4> c = Decimal<4>("-1.00000");
    EXPECT_FALSE(a == c);
    Decimal<4> d = Decimal<4>("-1");
    EXPECT_TRUE(c == d);
    PASS;
}

TEST(operator_inequal)
{
    Decimal<4> a = Decimal<4>("1.0000");
    EXPECT_FALSE(a != a);
    Decimal<4> b = Decimal<4>("2.0000");
    EXPECT_TRUE(a != b);
    Decimal<4> c = Decimal<4>("-1.0000");
    EXPECT_TRUE(a != c);
    Decimal<4> d = Decimal<4>("10.000");
    EXPECT_TRUE(a != d);
    PASS;
}

TEST(operator_greater)
{
    Decimal<4> a = Decimal<4>("10.0000");
    Decimal<4> b = Decimal<4>("-5.0000");
    EXPECT_TRUE(a > b);
    EXPECT_FALSE(b > a);
    PASS;
}

TEST(operator_less)
{
    Decimal<4> a = Decimal<4>("-1.0000");
    Decimal<4> b = Decimal<4>("1.0000");
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
    PASS;
}

TEST(operator_greater_equal)
{
    Decimal<4> a = Decimal<4>("10.0000");
    Decimal<4> b = Decimal<4>("-5.0000");
    EXPECT_TRUE(a >= a);
    EXPECT_TRUE(a >= b);
    EXPECT_FALSE(b >= a);
    PASS;
}

TEST(operator_less_equal)
{
    Decimal<4> a = Decimal<4>("-1.0000");
    Decimal<4> b = Decimal<4>("1.0000");
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(b <= a);
    PASS;
}

TEST(operator_plus)
{
    Decimal<4> a = Decimal<4>("1234");
    Decimal<4> b = Decimal<4>("-50");
    Decimal<4> c = Decimal<4>("3.3333");
    Decimal<4> d = Decimal<4>("-9.9999");
    EXPECT_EQUAL(a + a, Decimal<4>("2468"));
    EXPECT_EQUAL(a + b, Decimal<4>("1184"));
    EXPECT_EQUAL(a + c, Decimal<4>("1237.3333"));
    EXPECT_EQUAL(a + d, Decimal<4>("1224.0001"));
    EXPECT_EQUAL(b + b, Decimal<4>("-100"));
    EXPECT_EQUAL(b + c, Decimal<4>("-46.6667"));
    EXPECT_EQUAL(b + d, Decimal<4>("-59.9999"));
    EXPECT_EQUAL(c + d, Decimal<4>("-6.6666"));
    PASS;
}

TESTS_END
