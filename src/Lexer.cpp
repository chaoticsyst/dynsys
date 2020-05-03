#include "Lexer.h"

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
        currentLexema = Lexema::OpenParenthesis;
        return;
    case ')':
        goNextChar();
        currentLexema = Lexema::CloseParenthesis;
        return;
    case 'x':
        goNextChar();
        currentLexema = Lexema::X;
        return;
    case 'y':
        goNextChar();
        currentLexema = Lexema::Y;
        return;
    case 'z':
        goNextChar();
        currentLexema = Lexema::Z;
        return;
    }

    if (isdigit(currentChar) || currentChar == '.') {
        std::string constant;
        bool wasDecimalPoint = false;
        while (isdigit(currentChar) || currentChar == '.') {
            if (currentChar == '.' && wasDecimalPoint) {
                throw LexerException("Invalid format of a number: too many decimal points.");
            }
            constant += currentChar;
            wasDecimalPoint |= (currentChar == '.');
            goNextChar();
        }

        currentConstant = std::stold(constant);
        currentLexema = Lexema::Constant;

        return;
    }

    throw LexerException("Invalid character.");
}

Lexema Lexer::getCurrentLexema() const noexcept {
    return currentLexema;
}

long double Lexer::getCurrentConstant() const noexcept {
    return currentConstant;
}

} //namespace Lexer
