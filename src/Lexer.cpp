#include "Lexer.hpp"
#include "ParserException.h"

namespace Lexer {

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
        currentLexema = Lexema::End;
        return;
    case '+':
        goNextChar();
        currentLexema = Lexema::Add;
        return;
    case '-':
        goNextChar();
        currentLexema = Lexema::Subtract;
        return;
    case '*':
        goNextChar();
        currentLexema = Lexema::Multiply;
        return;
    case '/':
        goNextChar();
        currentLexema = Lexema::Divide;
        return;
    case '(':
        goNextChar();
        currentLexema = Lexema::OpenParens;
        return;
    case ')':
        goNextChar();
        currentLexema = Lexema::CloseParens;
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

        currentConstant = std::stold(constant);
        currentLexema = Lexema::Constant;

        return;
    }

    if (isalpha(currentChar)) {
        currentIdentifier.clear();
        while (isalpha(currentChar)) {
            currentIdentifier += currentChar;
            goNextChar();
        }

        currentLexema = Lexema::Identifier;

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
