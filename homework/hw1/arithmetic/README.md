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

Should division by zero occur, the problem should simply output `error` on the corresponding line.
However, it should continue reading in expressions until the stream is closed.

### Theory

We recommend you use an [operator precedence parser](https://en.wikipedia.org/wiki/Operator-precedence_parser), which may necessitate some research.
We also recommended that you tokenize the input prior to parsing, as it should separate your code into more manageable pieces.
Optionally, we suggest that you use iterators to consume tokens from the input.

### Specifications

The entry point for the automated grader, relative to the root of your submission directory, will be `arithmetic/Makefile`.
This makefile will be executed with no arguments, invoking the default task, and should produce a binary `arithmetic/main` that will be tested. 
