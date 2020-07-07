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
        return value < 0 ? -1 : 1;
    }

    // Raise 10 to the n
    constexpr value_type E(size_t n)
    {
        value_type k {1};
        for (size_t i {}; i < n; i++)
        {
            k *= 10;
        }
        return k;
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
        std::string result = std::to_string(this->value);
        size_t start { this->value < 0 ? 1ul : 0ul };
        size_t size { result.size() };
        if (P > size - start)
        {
            for (size_t i {}; i < P - size + start; i++)
            {
                result.insert(start, "0");
            }
            size = P + start;
        }

        size_t radix { size - P };
        result.insert(radix, radix == start ? "0." : ".");
        return result;
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
        size_t radix = str.find('.');
        if (radix != std::string::npos)
        {
            str.erase(radix, 1);
            if (str.length() > radix + P + 1)
            {
                str.erase(radix + P + 1);
            }
        }

        size_t idx {};
        value_type value = stoll(str, &idx);

        index = idx;
        if (radix != std::string::npos && index > radix)
        {
            if (index > radix + P)
            {
                value_type rounding = abs(value) % 10 >= 5;
                value = value / 10 + sign(value) * rounding;
            }
            else if (index < radix + P)
            {
                value = value * E(radix + P - index);
            }
           index++;
        } else {
            constexpr value_type EP = E(P);
            value *= EP;
        }

        return value;
    }

    // Multiply two value_type's together, respecting radix and trying to avoid overflow
    template<size_t P>
    value_type Decimal<P>::multiply(value_type lhs, value_type rhs)
    {
        constexpr int64_t E_P { E(P) };
        constexpr int64_t E_PM { E(P - 1) };

        // Separate into integral and fractional
        const value_type this_integral { lhs / E_P };
        const value_type this_fractional { lhs % E_P };
        const value_type other_integral { rhs / E_P };
        const value_type other_fractional { rhs % E_P };

        // Do multiplication
        const value_type high { this_integral * other_integral * E_P };
        const value_type middle { this_integral * other_fractional + this_fractional * other_integral };

        // Low is handled separately for rounding
        const value_type fractional { this_fractional * other_fractional };
        const value_type low { fractional / E_P };
        bool rounding { abs((fractional % E_P) / E_PM) >= 5 };

        // Sum
        value_type value {high + middle + low};
        value += rounding ? sign(value) : 0;
        return value;
    }

    // Long divide two integers, respecting radix and rounding
    template<size_t P>
    value_type Decimal<P>::divide(value_type lhs, value_type rhs)
    {
        constexpr int64_t E_P { E(P) };
        value_type result { lhs / rhs * E_P };
        value_type remainder { lhs % rhs };
        value_type exponent { E_P };

        // Build the fractional part
        for (size_t i {}; i < P && remainder != 0; i++)
        {
            remainder *= 10;
            exponent /= 10;
            result += remainder / rhs * exponent;
            remainder %= rhs;
        }

        // Rounding
        if (abs(remainder * 10 / rhs) >= 5)
        {
            result += sign(lhs) * sign(rhs);
        }

        return result;
    }
}

#endif
