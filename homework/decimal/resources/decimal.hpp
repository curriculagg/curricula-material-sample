#ifndef DECIMAL_HPP
#define DECIMAL_HPP

#include <string>
#include <iostream>
#include <cstdint>
#include <climits>
#include <cmath>

namespace decimal
{
    // The type we'll use to represent the decimal value internally
    typedef std::int64_t value_type;

    // Determine the sign of a given value in terms of -1, 0, and 1
    inline int8_t sign(value_type value)
    {
        // TODO
    }

    // Raise 10 to the n
    constexpr value_type E(size_t n)
    {
        // TODO
    }

    template<size_t P>
    class Decimal
    {
    public:
        Decimal() : value(0) {}
        Decimal(const Decimal& decimal) : value(decimal.value) {}
        Decimal(const std::string& str) : value(Decimal::value_from_string(str)) {}

    private:
        value_type value;

        // Direct constructor
        Decimal(value_type value) : value(value) {}

        // Internal deserialization
        class Parser;
        static value_type value_from_string(const std::string& str);
        static value_type value_from_string(std::string str, size_t& index);

        // Math
        static value_type multiply(value_type lhs, value_type rhs);
        static value_type divide(value_type lhs, value_type rhs);

    public:
        // String conversion
        std::string to_string() const;
        static Decimal from_string(const std::string& value);
        static Decimal from_string(const std::string& value, size_t& index);

        // Output
        friend std::string to_string(Decimal decimal) { return decimal.to_string(); }
        friend std::ostream& operator<<(std::ostream &os, Decimal decimal) { return os << decimal.to_string(); }

        // Unary
        friend Decimal operator+(Decimal decimal) { return Decimal(decimal.value); }
        friend Decimal operator-(Decimal decimal) { return Decimal(-decimal.value); }

        // Comparison
        friend bool operator<(Decimal lhs, Decimal rhs) { return lhs.value < rhs.value; }
        friend bool operator>(Decimal lhs, Decimal rhs) { return lhs.value > rhs.value; }
        friend bool operator<=(Decimal lhs, Decimal rhs) { return lhs.value <= rhs.value; }
        friend bool operator>=(Decimal lhs, Decimal rhs) { return lhs.value >= rhs.value; }
        friend bool operator==(Decimal lhs, Decimal rhs) { return lhs.value == rhs.value; }
        friend bool operator!=(Decimal lhs, Decimal rhs) { return lhs.value != rhs.value; }

        // Arithmetic
        friend Decimal operator+(Decimal lhs, Decimal rhs) { return Decimal(lhs.value + rhs.value); }
        friend Decimal operator-(Decimal lhs, Decimal rhs) { return Decimal(lhs.value - rhs.value); }
        friend Decimal operator*(Decimal lhs, Decimal rhs) { return Decimal(Decimal::multiply(lhs.value, rhs.value)); }
        friend Decimal operator/(Decimal lhs, Decimal rhs) { return Decimal(Decimal::divide(lhs.value, rhs.value)); }
        friend Decimal operator%(Decimal lhs, Decimal rhs) { return Decimal(lhs.value % rhs.value); }

        friend Decimal& operator+=(Decimal& lhs, Decimal rhs) { lhs.value += rhs.value; return lhs; }
        friend Decimal& operator-=(Decimal& lhs, Decimal rhs) { lhs.value -= rhs.value; return lhs; }
        friend Decimal& operator*=(Decimal& lhs, Decimal rhs) { lhs.value = Decimal::multiply(lhs.value, rhs.value); return lhs; }
        friend Decimal& operator/=(Decimal& lhs, Decimal rhs) { lhs.value = Decimal::divide(lhs.value, rhs.value); return lhs; }
        friend Decimal& operator%=(Decimal& lhs, Decimal rhs) { lhs.value %= rhs.value; return lhs; }
    };

    // Convert the decimal to a string, respecting sign, radix, and zeros
    template<size_t P>
    std::string Decimal<P>::to_string() const
    {
        // TODO
    }

    // Proxy to below
    template<size_t P>
    Decimal<P> Decimal<P>::from_string(const std::string& str)
    {
        size_t index;
        return Decimal<P>::from_string(str, index);
    }

    // Parse a decimal from a string, expect valid input
    template<size_t P>
    Decimal<P> Decimal<P>::from_string(const std::string& str, size_t& index)
    {
        return Decimal<P>(Decimal<P>::value_from_string(str, index));
    }

    template<size_t P>
    value_type Decimal<P>::value_from_string(const std::string& str)
    {
        size_t index;
        return Decimal<P>::value_from_string(str, index);
    }

    template<size_t P>
    value_type Decimal<P>::value_from_string(std::string str, size_t& index)
    {
        // TODO
    }

    // Multiply two value_type's together, respecting radix and trying to avoid overflow
    template<size_t P>
    value_type Decimal<P>::multiply(value_type lhs, value_type rhs)
    {
        // TODO
    }

    // Long divide two integers, respecting radix and rounding
    template<size_t P>
    value_type Decimal<P>::divide(value_type lhs, value_type rhs)
    {
        // TODO
    }
}

#endif
