# Arithmetic

Write a parser capable of computing the value of mathematical expressions comprised of `+`, `-`, `*`, `/`, and parentheses.
The parser should accept read an expression in for every line of `stdin` and output its value to `stdout`.
Some example expressions might include:

```
1 + 2 + 3
6 + 4 / 2
(1 - 2) / 3
```

Should division by zero occur, the problem should simply output `error`.
However, it should continue reading in expressions until the stream is closed.
