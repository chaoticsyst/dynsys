#include <sstream>

#include "Lexer.hpp"
#include "ParserException.hpp"

namespace Lexer {

long double convertStringToLongDouble(const std::string &number) {
    std::stringstream stream(number);

    long double result = 0;
    stream >> result;

    return result;
}

Lexer::Lexer(const std::string &expression_) :
    curExprIterator{expression_.begin()}, endExprIterator{expression_.end()} {

    goNextChar();
    goNextLexema();
}

void Lexer::goNextChar() {
    if (curExprIterator == endExprIterator) {
        currentChar = '\0';
    } else {
        currentChar = *curExprIterator;
        curExprIterator++;
    }
}

void Lexer::goNextLexema() {
    while (isspace(currentChar)) {
        goNextChar();
    }

    switch (currentChar) {
    case '\0':
        currentLexema = Lexema::end;
        return;
    case '+':
        goNextChar();
        currentLexema = Lexema::add;
        return;
    case '-':
        goNextChar();
        currentLexema = Lexema::subtract;
        return;
    case '*':
        goNextChar();
        currentLexema = Lexema::multiply;
        return;
    case '/':
        goNextChar();
        currentLexema = Lexema::divide;
        return;
    case '^':
        goNextChar();
        currentLexema = Lexema::power;
        return;
    case '(':
        goNextChar();
        currentLexema = Lexema::openParens;
        return;
    case ')':
        goNextChar();
        currentLexema = Lexema::closeParens;
        return;
    }

    if (isdigit(currentChar) || currentChar == '.' || currentChar == ',') {
        std::string constant;
        bool wasDecimalPoint = false;
        while (isdigit(currentChar) || currentChar == '.' || currentChar == ',') {
            if ((currentChar == '.' || currentChar == ',') && wasDecimalPoint) {
                throw Parser::ParserException("Unexpected two or more decimal points.");
            }
            if (currentChar == '.' || currentChar == ',') {
                constant += '.';
                wasDecimalPoint = true;
            } else {
                constant += currentChar;
            }
            goNextChar();
        }

        currentConstant = convertStringToLongDouble(constant);
        currentLexema = Lexema::constant;

        return;
    }

    if (isalpha(currentChar)) {
        currentIdentifier.clear();
        while (isalpha(currentChar)) {
            currentIdentifier += currentChar;
            goNextChar();
        }

        currentLexema = Lexema::identifier;

        return;
    }

    throw Parser::ParserException("Unexpected character.");
}

Lexema Lexer::getCurrentLexema() const noexcept {
    return currentLexema;
}

long double Lexer::getCurrentConstant() const noexcept {
    return currentConstant;
}

const std::string &Lexer::getCurrentIdentifier() const noexcept {
    return currentIdentifier;
}

} //namespace Lexer
