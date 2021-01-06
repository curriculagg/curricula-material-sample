Fixed-point decimals are particularly useful in cases where we are dealing with precise values, such as currency, time, or your grades (for which we do in fact use decimals).
"But what's wrong with floating point numbers?" you might be wondering.
Nothing intrinsically; `float` and `double` are both convenient and fairly accurate, and are sufficient for most application.
However, both fall victim to rounding error which you can read about [here](https://floating-point-gui.de/basic/), making them unsuitable for the aforementioned scenarios.
This is why, as you'll see later on, we don't offer constructors or operations for other numeric types.
Practically, there's little reason to; if you're working with decimals for their precision, there is no point at which you would want to introduce a stray floating point error.

In this problem you will implement a rudimentary fixed-point decimal.
All `Decimal` instances will be templated with a parameter `size_t P`, which will dictate how many places are to be reserved after the decimal point.
For example, a `Decimal<4>` would have 4 digits of precision, and could store `-12.3456` or `0.6667`.
Your `Decimal` will have two main capabilities: translation to and from `std::string`, and standard mathematical operations.

### Underlying Representation

You'll notice that the `Decimal` class skeleton provided to you has exactly one private member of type `value_type`.
This is, as defined in the first lines of the file, is an alias to `int64_t`, which is the widest size integer universally supported by C++.
Using an `int64_t` will give us the largest range with the least trouble.
Note that we `typedef` it as `value_type` for convenience; if we wish to change the type at a later date, we can do so in just one place.

In terms of underlying representation, our design is quite simple: our decimal `d` is stored as `d * pow(10, p)`.
If we had a `Decimal<4>` representing `0.3333`, its value would consist of the integer `3333`.
If we had a `Decimal<6>` representing `-12.345678`, its value would consist of the integer `-12345678`.
While this means that we cannot represent anything with more precision than `0.0001` for `Decimal<4>` and `0.000001` for `Decimal<6>`, it also means that we are storing exactly the values that we can represent.

### Convenience Methods

There are two convenience methods we highly recommend you write at the start.
The first is `inline int8_t sign(value_type value)`, which simply returns `-1` if the value is negative and `1` otherwise.
The second, `constexpr value_type E(size_t n)`, is a little bit more interesting.
 
All calls to `E(n)` with positive `n` should return 10 to the `n`th power.
You'll likely find yourself invoking `E` with constant functions of `P` for multiplication, division, and perhaps a few other tasks.  
Because `P` is a template parameter and therefore also available during compilation, marking `E` as `constexpr` will save us a few cycles during runtime by precomputing the result ahead of time. 

### String Translation

As with all other methods related to the `Decimal<P>`, a base structure is available in the problem resources.
The first thing you will want to implement is `std::string` parsing, as it will allow you to start testing.
Like `stoll`, which you may want to use, the overload of `Decimal<P>::value_from_string` you will implement should take the `std::string` from which you are to parse the decimal and an `index` reference, which should be left pointing to the next character in the string after those consumed by the decimal.

This method should return a `value_type` corresponding to the internal representation you would expect for a `Decimal<P>` matching the value of the string.
In other words, `Decimal<4>::value_from_string("0.6666")` should return `6666`, and `Decimal<4>::value_from_string("1.0")` should return `10000`.
Throughout this project we will round up from 5 to handle values more precise than we can represent.
Therefore, `Decimal<4>::value_from_string("0.66666")` should return 6667 and `Decimal<4>::value_from_string("0.33333")` should return `3333`.

Note that this signature also receives the `str` by value, as you will most likely want to modify the buffer.
By comparison, `Decimal<P>::to_string()` should be a breeze.
