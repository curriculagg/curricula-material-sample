#include "arithmetic.hpp"
#include <vector>
#include <cctype>
#include <iostream>
#include <tuple>

using namespace std;

class Token
{
public:
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
    float value;
    int precedence;
    bool binary;

    Token(Kind kind, float value) : kind(kind), value(value)
    {
        this->precedence = this->get_precedence();
        this->binary = this->is_binary();
    }

private:
    int get_precedence() const
    {
        switch (this->kind)
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

    bool is_binary() const
    {
        switch (this->kind)
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
    string::const_iterator it = input.cbegin();
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
            tokens.push_back(Token { static_cast<Token::Kind>(*it++), 0 });
            break;
        default:
            size_t length;
            float value = stof(input.substr(distance(input.begin(), it)), &length);
            tokens.push_back(Token { Token::Kind::number, value });
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
    Token result(Token::Kind::number, 0);
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
        return Token(result.kind, -result.value);
    case Token::Kind::number:
        result = *it;
        it++;
        return result;
    default:
        throw "invalid state";
    }
}

Token parse_expression(token_iterator& it, token_iterator end, Token left, int minimum_precedence)
{
    float result = left.value;
    while (it != end && it->binary && it->precedence > minimum_precedence)
    {
        Token operation = *it++;
        Token right = parse_primary(it, end);
        while (it != end && it->binary && it->precedence > operation.precedence)
        {
            int new_precedence = it->precedence;
            it++;
            right = parse_expression(it, end, right, new_precedence);
        }
        switch (operation.kind)
        {
        case Token::Kind::plus:
            result = result + right.value;
            break;
        case Token::Kind::minus:
            result = result - right.value;
            break;
        case Token::Kind::times:
            result = result * right.value;
            break;
        case Token::Kind::divide:
            result = result / right.value;
            break;
        default:
            break;
        }
    }
    return Token(Token::Kind::number, result);
}

Token parse_expression(token_iterator& it, token_iterator end)
{
    return parse_expression(it, end, parse_primary(it, end), 0);
}

float compute(const string& input)
{
    vector<Token> tokens = tokenize(input);
    token_iterator cursor = tokens.cbegin();
    return parse_expression(cursor, tokens.cend()).value;
}
