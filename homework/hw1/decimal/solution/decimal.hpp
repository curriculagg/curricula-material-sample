#include <string>
#include <iostream>
#include <cstdint>
#include <climits>
#include <cmath>

typedef std::int64_t value_type;

inline int8_t sign(value_type value)
{
    return (0 < value) - (value < 0);
}

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
    constexpr Decimal() : value(0) {}
    constexpr Decimal(const Decimal& decimal) : value(decimal.value) {}
    constexpr explicit Decimal(const std::string& value) : value(Decimal::from_string(value)) {}

private:
    value_type value;

    // Direct constructor
    Decimal(value_type value) : value(value) {}

    // Internal parsing
    static int64_t from_string(const std::string& value);
    std::string to_string() const;

    // Math
    static int64_t multiply(value_type lhs, value_type rhs);
    static int64_t divide(value_type lhs, value_type rhs);

public:
    // Output
    friend std::string to_string(Decimal decimal) { return decimal.to_string(); }
    friend std::ostream& operator<<(std::ostream &os, Decimal decimal) { return os << decimal.to_string(); }

    // Unary
    friend Decimal operator+(Decimal decimal) { return Decimal(decimal.value); }
    friend Decimal operator-(Decimal decimal) { return Decimal(-decimal.value); }

    // Comparison
    friend Decimal operator<(Decimal lhs, Decimal rhs) { return lhs.value < rhs.value; }
    friend Decimal operator>(Decimal lhs, Decimal rhs) { return lhs.value > rhs.value; }
    friend Decimal operator<=(Decimal lhs, Decimal rhs) { return lhs.value <= rhs.value; }
    friend Decimal operator>=(Decimal lhs, Decimal rhs) { return lhs.value >= rhs.value; }
    friend Decimal operator==(Decimal lhs, Decimal rhs) { return lhs.value == rhs.value; }
    friend Decimal operator!=(Decimal lhs, Decimal rhs) { return lhs.value != rhs.value; }

    // Arithmetic
    friend Decimal operator+(Decimal lhs, Decimal rhs) { return Decimal(lhs.value + rhs.value); }
    friend Decimal operator-(Decimal lhs, Decimal rhs) { return Decimal(lhs.value - rhs.value); }
    friend Decimal operator*(Decimal lhs, Decimal rhs) { return Decimal(Decimal::multiply(lhs.value, rhs.value)); }
    friend Decimal operator/(Decimal lhs, Decimal rhs) { return Decimal(Decimal::divide(lhs.value, rhs.value)); }
    friend Decimal& operator+=(Decimal lhs, Decimal rhs) { lhs.value += rhs.value; return lhs; }
    friend Decimal& operator-=(Decimal lhs, Decimal rhs) { lhs.value -= rhs.value; return lhs; }
    friend Decimal& operator*=(Decimal lhs, Decimal rhs) { lhs.value = Decimal<P>::multiply(lhs.value, rhs.value); return lhs; }
    friend Decimal& operator/=(Decimal lhs, Decimal rhs) { lhs.value = Decimal<P>::divide(lhs.value, rhs.value); return lhs; }
};

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

template<size_t P>
value_type Decimal<P>::from_string(const std::string& value)
{
    std::string buffer = value;
    std::size_t radix { value.find(".") };
    if (radix == std::string::npos)
    {
        return stoll(buffer) * E(P);
    }
    else
    {
        buffer.erase(radix, 1);
        if (buffer.size() > radix + P)
        {
            buffer.erase(radix + P);
            return stoll(buffer);
        }
        else
        {
            size_t padding { P - (buffer.size() - radix) };
            return stoll(buffer) * E(padding);
        }
    }
}

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
    const value_type rounding { fractional % E_P / E_PM >= 5 };

    // Sum
    return high + middle + low + rounding;
}

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
    if (abs(remainder * 10) / rhs >= 5)
    {
        result += sign(lhs) * sign(rhs);
    }

    return result;
}
