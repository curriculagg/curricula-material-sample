#include "arithmetic.hpp"
#include <vector>
#include <cctype>
#include <iostream>
#include <tuple>
#include <stdexcept>

using namespace std;

struct Token
{
    enum class Kind
    {
        number,
        plus = '+',
        minus = '-',
        times = '*',
        divide = '/',
        left = '(',
        right = ')',
    };

    Kind kind;
    decimal_type value;
    int precedence;
    bool is_binary;

    inline decimal_type apply(const Token& left, const Token& right)
    {
        switch (this->kind)
        {
        case Token::Kind::plus:
            return left.value + right.value;
        case Token::Kind::minus:
            return left.value - right.value;
        case Token::Kind::times:
            return left.value * right.value;
        case Token::Kind::divide:
            if (right.value == decimal_type())
            {
                throw logic_error("divide by zero");
            }
            return left.value / right.value;
        default:
            throw invalid_argument("invalid state");
        }
    }

    static int get_precedence(Kind kind)
    {
        switch (kind)
        {
        case Token::Kind::plus:
        case Token::Kind::minus:
            return 1;
            break;
        case Token::Kind::times:
        case Token::Kind::divide:
            return 2;
            break;
        default:
            return 0;
        }
    }

    static bool get_is_binary(Kind kind)
    {
        switch (kind)
        {
        case Token::Kind::plus:
        case Token::Kind::minus:
        case Token::Kind::times:
        case Token::Kind::divide:
            return true;
        default:
            return false;
        }
    }

    static Token from(Kind kind, decimal_type value)
    {
        return Token { kind, value, Token::get_precedence(kind), Token::get_is_binary(kind) };
    }

    static Token from(Kind kind)
    {
        return Token::from(kind, decimal_type());
    }
};


void consume_whitespace(string::const_iterator& it, string::const_iterator end)
{
    while (it != end && isspace(*it))
    {
        it++;
    }
}

typedef vector<Token> token_vector;
typedef vector<Token>::const_iterator token_iterator;

token_vector tokenize(const string& input)
{
    vector<Token> tokens;
    string::const_iterator it { input.cbegin() };
    consume_whitespace(it, input.cend());

    while (it != input.cend())
    {
        switch (*it)
        {
        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
            tokens.push_back(Token::from(static_cast<Token::Kind>(*it++)));
            break;
        default:
            size_t length { };
            decimal_type value = decimal_type::from_string(input.substr(distance(input.begin(), it)), length);
            tokens.push_back(Token::from(Token::Kind::number, value));
            advance(it, length);
            break;
        }
        consume_whitespace(it, input.cend());
    }

    return tokens;
}

Token parse_expression(token_iterator& it, token_iterator end);
Token parse_expression(token_iterator& it, token_iterator end, Token left, int minimum_precedence);

Token parse_primary(token_iterator& it, token_iterator end)
{
    Token result;
    switch (it->kind)
    {
    case Token::Kind::left:
        it++;
        result = parse_expression(it, end);
        it++;
        return result;
    case Token::Kind::minus:
        it++;
        result = parse_primary(it, end);
        result.value = -result.value;
        return result;
    case Token::Kind::number:
        return *it++;
    default:
        throw invalid_argument("invalid state");
    }
}

Token parse_expression(token_iterator& it, token_iterator end, Token left, int minimum_precedence)
{
    while (it != end && it->is_binary && it->precedence >= minimum_precedence)
    {
        Token operation { *it++ };
        Token right { parse_primary(it, end) };
        while (it != end && it->is_binary && it->precedence > operation.precedence)
        {
            right = parse_expression(it, end, right, it->precedence);
        }
        left.value = operation.apply(left, right);
    }
    return left;
}

Token parse_expression(token_iterator& it, token_iterator end)
{
    return parse_expression(it, end, parse_primary(it, end), 0);
}

decimal_type evaluate(const string& input)
{
    vector<Token> tokens { tokenize(input) };
    token_iterator cursor { tokens.cbegin() };
    return parse_expression(cursor, tokens.cend()).value;
}
