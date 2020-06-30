#include <string>
#include <iostream>
#include <cstdint>
#include <climits>
#include <cmath>

enum class Sign : std::uint8_t
{
    plus = 0,
    minus = 1,
};

typedef std::uint64_t Value;

constexpr Value E(size_t n)
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

    // Copy from other
    Decimal(const Decimal<P>& decimal) : sign(decimal.sign), value(decimal.value) {}

    // Internal constructor
    Decimal(Sign sign, std::uint64_t value) : sign(sign), value(value) {}

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
    Sign sign;
    Value value;
};

template<size_t P> Decimal<P>::Decimal(const std::string& value)
{
    std::string buffer = value;
    if (buffer[0] == '-')
    {
        this->sign = Sign::minus;
        buffer.erase(0, 1);
    }
    else
    {
        this->sign = Sign::plus;
    }
    std::size_t radix = value.find(".");
    if (radix == std::string::npos)
    {
        this->value = stoll(buffer) * pow(10, P);
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
            this->value = stoll(buffer) * std::pow(10, padding);
        }
    }
}

template<size_t P> std::string Decimal<P>::to_string() const
{
    std::string result = std::to_string(this->value);

    size_t size = result.size();
    if (P > size)
    {
        for (size_t i = 0; i < P - size; i++)
        {
            result.insert(0, "0");
        }
        size = P;
    }

    size_t radix = size - P;
    result.insert(radix, radix == 0 ? "0." : ".");
    if (this->sign == Sign::minus)
    {
        result.insert(0, "-");
    }
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
    return this->sign == other.sign && this->value == other.value;
}

template<size_t P> Decimal<P> Decimal<P>::operator!=(const Decimal<P>& other) const
{
    return this->sign != other.sign || this->value != other.value;
}

template<size_t P> Decimal<P> Decimal<P>::operator<(const Decimal<P>& other) const
{
    switch (this->sign)
    {
    case Sign::plus:
        switch (other.sign)
        {
        case Sign::plus:
            return this->value < other.value;
        case Sign::minus:
            return false;
        }
    case Sign::minus:
        switch (other.sign)
        {
        case Sign::plus:
            return true;
        case Sign::minus:
            return this->value > other.value;
        }
    }
}

template<size_t P> Decimal<P> Decimal<P>::operator>(const Decimal<P>& other) const
{
    switch (this->sign)
    {
    case Sign::plus:
        switch (other.sign)
        {
        case Sign::plus:
            return this->value > other.value;
        case Sign::minus:
            return true;
        }
    case Sign::minus:
        switch (other.sign)
        {
        case Sign::plus:
            return false;
        case Sign::minus:
            return this->value < other.value;
        }
    }
}

template<size_t P> Decimal<P> Decimal<P>::operator+(const Decimal<P>& other) const
{
    if (this->sign == other.sign)
    {
        return Decimal<P>(this->sign, this->value + other.value);
    }
    else if (this->value == other.value)
    {
        return Decimal<P>();
    }
    else
    {
        if (this->value > other.value)
        {
            return Decimal<P>(this->sign, this->value - other.value);
        }
        else
        {
            return Decimal<P>(other.sign, other.value - this->value);
        }
    }
}

template<size_t P> Decimal<P> Decimal<P>::operator-(const Decimal<P>& other) const
{
    return *this + -other;
}

template<size_t P> Decimal<P> Decimal<P>::operator-() const
{
    return Decimal<P>(Sign(this->sign == Sign::plus ? Sign::minus : Sign::plus), this->value);
}

template<size_t P> Decimal<P> Decimal<P>::operator*(const Decimal<P>& other) const
{
    Sign sign = this->sign == other.sign ? Sign::plus : Sign::minus;
    uint64_t exponent = E(P);
    uint64_t this_quotient = this->value / exponent;
    uint64_t this_remainder = this->value % exponent;
    uint64_t other_quotient = other.value / exponent;
    uint64_t other_remainder = other.value % exponent;
    uint64_t high = this_quotient * other_quotient;
    uint64_t mid = this_quotient * other_remainder + this_remainder * other_quotient;
    uint64_t low = this_remainder * other_remainder;
    return Decimal<P>(sign, high * exponent + mid / exponent + low / E(2 * P));
}

template<size_t P> Decimal<P> Decimal<P>::operator/(const Decimal<P>& other) const
{
    Value result = this->value / other.value * pow(10, P);
    Value remainder = this->value % other.value;
    Value exponent = pow(10, P);
    for (int i = 0; i < P && remainder != 0; i++)
    {
        exponent /= 10;
        remainder *= 10;
        result += remainder / other.value * exponent;
        remainder = remainder % other.value;
    }
    if (remainder * 10 / other.value >= 5)
    {
        result += 1;
    }
    Sign sign = this->sign == other.sign ? Sign::plus : Sign::minus;
    return Decimal<P>(sign, result);
}
