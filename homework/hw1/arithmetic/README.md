Write a parser capable of computing the value of mathematical expressions comprised of `+`, `-`, `*`, `/`, and parentheses.
The parser should accept read an expression in for every line of `stdin` and output its value to `stdout` on the corresponding line.
Some example expressions might include:

```
1 + 2.2 + 3
6 + 4 / 2 - 3
(1 - 2) / 3
12.34 * -2 * 4 + 0.5
-2 - -5 / 5
```

Should division by zero occur, the problem should simply output `divide by zero` on the corresponding line.
However, it should continue reading in expressions until the stream is closed.
This program will use the decimal class you wrote in the first part of the assignment.

### Implementation

We recommend you use an [operator precedence parser](https://en.wikipedia.org/wiki/Operator-precedence_parser), which we will discuss in detail below.
We also recommended that you tokenize the input prior to parsing, as it will separate your code into more manageable pieces.
Optionally, we suggest that you use iterators to consume tokens from the input.

- [ ] Skim operator precedence parsing

### Tokenization

Tokenization for our arithmetic parser should be fairly straightforward.
We recommend you represent tokens with an `enum class Kind` and a `Decimal<4>` value.
While all of the symbols, `+-*/()` will have their own `Kind`, all numeric tokens can share a `number` kind to simplify cases in `parse_primary`.

Skipping whitespace and consuming single character tokens should be no problem. 
For decimals, we recommend you use the static helper `Decimal<P>::from_string` you already implemented.

- [ ] Finish `enum class Kind`
- [ ] Create a `Token` struct with `Kind kind` and `decimal_type`
- [ ] Write a tokenizer that takes a string and returns some collection of tokens
    - [ ] Implement single character token parsing
    - [ ] Use `decimal_type::from_string` to implement decimal parsing

### Parsing

Wikipedia provides an extensive review of the theory behind the operator precedence parser, and includes such valuable insights as:

> An operator-precedence parser is a simple shift-reduce parser that is capable of parsing a subset of LR(1) grammars.
> More precisely, the operator-precedence parser can parse all LR(1) grammars where two consecutive nonterminals and epsilon never appear in the right-hand side of any rule.

Most of us will likely read this and walk away somehow knowing less.
However, upon translation to English, there are a couple things we can gleam.
In general, a grammar is a set of patterns defining the structure of language.
An operator precedence parser implements a certain grammar to be able to parse things like arithmetic expressions, which can have ambiguities resolved by operator precedence.
Consider `a + b + c`, which is pretty easy to parse from left to right, and compare it to `a + b * c`, which cannot be parsed the same way.

With all of that said, instead of trying to understand it without all of the requisite linguistic knowledge, we'll simply walk through the algorithm.
Fortunately, it's fairly concise, and there are two main functions for us to think about.

#### Primaries

The first thing we need to know how to parse is a `primary`, which for operator precedence parsers is defined as a value and any associated unary operators.
Things like `5.0` and `-20` and `(3.3)` are all primaries, and combinations of them, such as `-(-20)` or even `--4`, are as well.
It might seem daunting to have to consider all of these possibilities, but it turns out that if we approach the problem recursively, it's nearly painless.
We will implement four cases in our recursive method:

```
parse_primary(tokens)
    if next token is number
        token = pop next token
        return token
    if next token is -
        pop next token
        token = parse_primary(tokens)
        negate token value
        return token
    if next token is +
        pop next token
        token = parse_primary(tokens)
        return token
    if next token is (
        pop next token
        token = parse_expression(tokens)
        pop next token  # closing paren
        return token
```

- [ ] Implement `parse_primary` however you see fit

#### Expressions

Once this is done, we can implement the aforementioned `parse_expression`.
Essentially, the idea here is that we only progress from left to right if the next operator is of equal or greater precedence, and if it's not, we first evaluate what we've got then continue.

```
parse_expression(tokens, lhs, minimum precedence)
    lookahead = peek next token
    while lookahead is a binary operator whose precedence is >= minimum precedence
        operator = pop next token
        rhs = parse_primary(tokens)
        lookahead = peek next token
        while lookahead is a binary operator whose precedence is greater than operator's, 
                or a right-associative operator whose precedence is equal to operator's
            rhs = parse_expression(tokens, rhs, lookahead's precedence)
            lookahead = peek next token
        lhs = the result of applying operator with operands lhs and rhs
    return lhs
```

Note that this implementation associates a precedence with operators.
For us, it is sufficient for `+` and `-` to have precedence 1 and `*` and `/` to have precedence 2, with the default precedence being 0.

- [ ] Add a `int precedence` and `bool is_binary` to `Token`
- [ ] Write a method that applies a binary operator to two `Token`s and returns a `Token`
- [ ] Implement `parse_expression` with pseudocode arguments
- [ ] Add an overload of `parse_expression` that only takes the tokens for convenience
- [ ] Use `parse_expression` in `evaluate`

Good luck!

### Specifications

The entry point for the automated grader, relative to the root of your submission directory, will be `arithmetic/Makefile`.
This makefile will be executed with no arguments, invoking the default task, and should produce a binary `arithmetic/main` that will be tested. 
