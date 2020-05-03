#pragma once

#include <array>
#include <stdexcept>
#include <string>

namespace Lexer {

enum class Lexema {
    X, Y, Z,
    Constant,
    Add,
    Subtract,
    Multiply,
    Divide,
    OpenParenthesis,
    CloseParenthesis,
    End
};

class Lexer final {
public:
    Lexer(const std::string &expression);

    Lexema getCurrentLexema() const noexcept;
    long double getCurrentConstant() const noexcept;

    void goNextLexema();

private:
    std::string::const_iterator curExprIterator;
    std::string::const_iterator endExprIterator;

    Lexema currentLexema;
    char currentChar;
    long double currentConstant;

    void goNextChar();
};

class LexerException final : public std::logic_error {
    using std::logic_error::logic_error;
};

} //namespace Lexer
