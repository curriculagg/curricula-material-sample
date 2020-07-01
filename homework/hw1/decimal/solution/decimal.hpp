#include <string>
#include <iostream>
#include <cstdint>
#include <climits>
#include <cmath>

typedef std::int64_t Value;

int8_t sign(Value value)
{
    return (0 < value) - (value < 0);
}

constexpr Value E(size_t n)
{
    Value k = 1;
    for (size_t i = 0; i < n; i++)
    {
        k *= 10;
    }
    return k;
}

template<size_t P>
class Decimal
{
public:
    // Default constructor creates zero
    Decimal() : value(0) {}
    Decimal(const Decimal<P>& decimal) : value(decimal.value) {}
    Decimal(std::uint64_t value) : value(value) {}

    // Parsing
    Decimal(const std::string& value);

    // Formatting
    std::string to_string() const;

    // Operators
    Decimal<P> operator==(const Decimal<P>& other) const;
    Decimal<P> operator!=(const Decimal<P>& other) const;
    Decimal<P> operator<(const Decimal<P>& other) const;
    Decimal<P> operator>(const Decimal<P>& other) const;
    Decimal<P> operator+(const Decimal<P>& other) const;
    Decimal<P> operator-(const Decimal<P>& other) const;
    Decimal<P> operator-() const;
    Decimal<P> operator*(const Decimal<P>& other) const;
    Decimal<P> operator/(const Decimal<P>& other) const;

private:
    Value value;
};

template<size_t P> Decimal<P>::Decimal(const std::string& value)
{
    std::string buffer = value;
    std::size_t radix = value.find(".");
    if (radix == std::string::npos)
    {
        this->value = stoll(buffer) * E(P);
    }
    else
    {
        buffer.erase(radix, 1);
        if (buffer.size() > radix + P)
        {
            buffer.erase(radix + P);
            this->value = stoll(buffer);
        }
        else
        {
            size_t padding = P - (buffer.size() - radix);
            this->value = stoll(buffer) * E(padding);
        }
    }
}

template<size_t P> std::string Decimal<P>::to_string() const
{
    std::string result = std::to_string(this->value);
    size_t start = this->value < 0 ? 1 : 0;
    size_t size = result.size();
    if (P > size - start)
    {
        for (size_t i = 0; i < P - size + start; i++)
        {
            result.insert(start, "0");
        }
        size = P + start;
    }

    size_t radix = size - P;
    result.insert(radix, radix == start ? "0." : ".");
    return result;
}

template<size_t P> std::string to_string(const Decimal<P>& decimal)
{
    return decimal.to_string();
}

template<size_t P> std::ostream& operator<<(std::ostream &os, const Decimal<P>& decimal)
{
    os << decimal.to_string();
    return os;
}

template<size_t P> Decimal<P> Decimal<P>::operator==(const Decimal<P>& other) const
{
    return this->value == other.value;
}

template<size_t P> Decimal<P> Decimal<P>::operator!=(const Decimal<P>& other) const
{
    return this->value != other.value;
}

template<size_t P> Decimal<P> Decimal<P>::operator<(const Decimal<P>& other) const
{
    return this->value < other.value;
}

template<size_t P> Decimal<P> Decimal<P>::operator>(const Decimal<P>& other) const
{
    return this->value > other.value;
}

template<size_t P> Decimal<P> Decimal<P>::operator+(const Decimal<P>& other) const
{
    return Decimal<P>(this->value + other.value);
}

template<size_t P> Decimal<P> Decimal<P>::operator-(const Decimal<P>& other) const
{
    return Decimal<P>(this->value - other.value);
}

template<size_t P> Decimal<P> Decimal<P>::operator-() const
{
    return Decimal<P>(-this->value);
}

template<size_t P> Decimal<P> Decimal<P>::operator*(const Decimal<P>& other) const
{
    constexpr Value E_P = E(P);
    constexpr Value E_M = E(P - 1);
    constexpr Value E_L = E(2 * P - 1);

    // Separate into integral and fractional
    const Value this_quotient = this->value / E_P;
    const Value this_remainder = this->value % E_P;
    const Value other_quotient = other.value / E_P;
    const Value other_remainder = other.value % E_P;

    // Do multiplication
    const Value high = this_quotient * other_quotient;
    const Value middle = this_quotient * other_remainder + this_remainder * other_quotient;
    const Value low = this_remainder * other_remainder;

    // Round
    Value rounding = middle / E_M + low / E_L;
    if (rounding % 10 >= 5)
    {
        rounding += 10;
    }

    // Sum
    return Decimal<P>(high * E_P + rounding / 10);
}

template<size_t P> Decimal<P> Decimal<P>::operator/(const Decimal<P>& other) const
{
    Value left = this->value;
    Value right = other.value;

    constexpr Value E_P = E(P);
    Value result = left / right * E_P;
    Value remainder = left % right;
    Value exponent = E_P;

    // Build the fractional part
    for (int i = 0; i < P && remainder != 0; i++)
    {
        remainder *= 10;
        exponent /= 10;
        result += remainder / right * exponent;
        remainder %= right;
    }

    // Rounding
    if (abs(remainder * 10) / right >= 5)
    {
        result += sign(left) * sign(right);
    }

    return Decimal<P>(result);
}
