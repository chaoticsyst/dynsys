#pragma once

#include <array>
#include <stdexcept>
#include <string>

namespace Lexer {

enum class Lexema {
    Identifier,
    Constant,
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,
    OpenParens,
    CloseParens,
    End
};

class Lexer final {
public:
    explicit Lexer(const std::string &expression);

    Lexema getCurrentLexema() const noexcept;
    long double getCurrentConstant() const noexcept;
    const std::string &getCurrentIdentifier() const noexcept;

    void goNextLexema();

private:
    std::string::const_iterator curExprIterator;
    std::string::const_iterator endExprIterator;

    Lexema currentLexema;
    char currentChar;
    long double currentConstant;
    std::string currentIdentifier;

    void goNextChar();
};

} //namespace Lexer
