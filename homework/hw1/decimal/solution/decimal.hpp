#include <string>
#include <iostream>
#include <cstdint>
#include <climits>
#include <cmath>

typedef std::int64_t Value;

constexpr const Value E(size_t n)
{
    switch (n)
    {
    case 0:
        return 1;
    case 1:
        return 10;
    default:
        return E(n / 2) * E(n / 2) * (n % 2 == 1 ? 10 : 1);
    }
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
    const Value exponent = E(P);
    const Value this_quotient = this->value / exponent;
    const Value this_remainder = this->value % exponent;
    const Value other_quotient = other.value / exponent;
    const Value other_remainder = other.value % exponent;
    const Value high = this_quotient * other_quotient;
    const Value mid = this_quotient * other_remainder + this_remainder * other_quotient;
    const Value low = this_remainder * other_remainder;
    return Decimal<P>(high * exponent + mid / exponent + low / E(2 * P));
}

template<size_t P> Decimal<P> Decimal<P>::operator/(const Decimal<P>& other) const
{
    Value result = this->value / other.value * E(P);
    Value remainder = this->value % other.value;

    // Build the fractional part
    Value exponent = E(P - 1);
    for (int i = 0; i < P && remainder != 0; i++)
    {
        result += remainder / other.value * exponent;
        remainder = remainder % other.value;
        remainder *= 10;
        exponent /= 10;
    }

    // Rounding
    if (remainder * 10 / other.value >= 5)
    {
        result += 1;
    }

    return Decimal<P>(result);
}
