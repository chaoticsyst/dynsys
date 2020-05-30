#include <map>
#include <memory>
#include <vector>
#include <cmath>

#include "Parser/Lexer.hpp"
#include "Parser/Parser.hpp"
#include "Parser/ParserNodes.hpp"

namespace Parser {

constexpr std::size_t X_VAR_POS = 0, Y_VAR_POS = 1, Z_VAR_POS = 2;

class Parser final {
public:
    explicit Parser(const std::string &expression,
                    const std::array<long double *, 3> &variableAddresses,
                    const std::map<std::string, long double> &customConstVariables) :
        lexer{expression}, variables{variableAddresses}, constVariables{customConstVariables} {}

    Parser(const Parser &)            = delete;
    Parser(Parser &&)                 = delete;
    Parser &operator=(const Parser &) = delete;
    Parser &operator=(Parser &&)      = delete;

    std::unique_ptr<Node> parse() {
        return parseAddSubtract();
    }

private:
    Lexer::Lexer lexer;
    const std::array<long double *, 3> &variables;
    std::map<std::string, long double> constVariables;

    std::unique_ptr<Node> parseAddSubtract() {
        std::unique_ptr<Node> leftPart = parseMultiplyDivide();

        while (true) {
            if (lexer.getCurrentLexema() == Lexer::Lexema::add) {
                lexer.goNextLexema();
                std::unique_ptr<Node> rightPart = parseMultiplyDivide();
                leftPart = std::make_unique<NodeBinaryOperation<BinaryOperation::add>>(std::move(leftPart), std::move(rightPart));
            } else if (lexer.getCurrentLexema() == Lexer::Lexema::subtract) {
                lexer.goNextLexema();
                std::unique_ptr<Node> rightPart = parseMultiplyDivide();
                leftPart = std::make_unique<NodeBinaryOperation<BinaryOperation::subtract>>(std::move(leftPart), std::move(rightPart));
            } else if (lexer.getCurrentLexema() != Lexer::Lexema::openParens &&
                       lexer.getCurrentLexema() != Lexer::Lexema::identifier) {

                break;
            } else {
                throw ParserException("Unexpected character.");
            }
        }

        return leftPart;
    }

    std::unique_ptr<Node> parseMultiplyDivide() {
        std::unique_ptr<Node> leftPart = parsePower();

        while (true) {
            if (lexer.getCurrentLexema() == Lexer::Lexema::multiply) {
                lexer.goNextLexema();
                std::unique_ptr<Node> rightPart = parsePower();
                leftPart = std::make_unique<NodeBinaryOperation<BinaryOperation::multiply>>(std::move(leftPart), std::move(rightPart));
            } else if (lexer.getCurrentLexema() == Lexer::Lexema::divide) {
                lexer.goNextLexema();
                std::unique_ptr<Node> rightPart = parsePower();
                leftPart = std::make_unique<NodeBinaryOperation<BinaryOperation::divide>>(std::move(leftPart), std::move(rightPart));
            } else if (lexer.getCurrentLexema() != Lexer::Lexema::openParens &&
                       lexer.getCurrentLexema() != Lexer::Lexema::identifier) {

                break;
            } else {
                throw ParserException("Unexpected character.");
            }
        }

        return leftPart;
    }

    std::unique_ptr<Node> parsePower() {
        std::vector<std::unique_ptr<Node>> parts;
        parts.push_back(parseUnary());
        while (true) {
            if (lexer.getCurrentLexema() == Lexer::Lexema::power) {
                lexer.goNextLexema();
                parts.push_back(parseUnary());
            } else if (lexer.getCurrentLexema() != Lexer::Lexema::openParens &&
                       lexer.getCurrentLexema() != Lexer::Lexema::identifier) {

                break;
            } else {
                throw ParserException("Unexpected character.");
            }
        }

        std::unique_ptr<Node> operation = std::move(parts.back());
        for (int i = static_cast<int>(parts.size()) - 2; i >= 0; i--) {
            operation = std::make_unique<NodeBinaryOperation<BinaryOperation::power>>(std::move(parts[i]), std::move(operation));
        }

        return operation;
    }

    std::unique_ptr<Node> parseUnary() {
        if (lexer.getCurrentLexema() == Lexer::Lexema::subtract) {
            lexer.goNextLexema();

            return std::make_unique<NodeFunction<Function::negative>>(parseUnary());
        }

        return parseLeaf();
    }

    std::unique_ptr<Node> parseLeaf() {
        if (lexer.getCurrentLexema() == Lexer::Lexema::constant) {
            std::unique_ptr<Node> leaf = std::make_unique<NodeConstant>(lexer.getCurrentConstant());
            lexer.goNextLexema();

            return leaf;
        }
        if (lexer.getCurrentLexema() == Lexer::Lexema::identifier) {
            std::string identifier = lexer.getCurrentIdentifier();
            lexer.goNextLexema();
            if (lexer.getCurrentLexema() == Lexer::Lexema::openParens) { //it is a function
                lexer.goNextLexema();

                std::unique_ptr<Node> func;
                std::unique_ptr<Node> argument = parseAddSubtract();
                if (identifier == "sin") {
                    func = std::make_unique<NodeFunction<Function::sin>>(std::move(argument));
                } else if (identifier == "cos") {
                    func = std::make_unique<NodeFunction<Function::cos>>(std::move(argument));
                } else if (identifier == "tan") {
                    func = std::make_unique<NodeFunction<Function::tan>>(std::move(argument));
                } else if (identifier == "asin") {
                    func = std::make_unique<NodeFunction<Function::asin>>(std::move(argument));
                } else if (identifier == "acos") {
                    func = std::make_unique<NodeFunction<Function::acos>>(std::move(argument));
                } else if (identifier == "atan") {
                    func = std::make_unique<NodeFunction<Function::atan>>(std::move(argument));
                } else if (identifier == "sinh") {
                    func = std::make_unique<NodeFunction<Function::sinh>>(std::move(argument));
                } else if (identifier == "cosh") {
                    func = std::make_unique<NodeFunction<Function::cosh>>(std::move(argument));
                } else if (identifier == "tanh") {
                    func = std::make_unique<NodeFunction<Function::tanh>>(std::move(argument));
                } else if (identifier == "asinh") {
                    func = std::make_unique<NodeFunction<Function::asinh>>(std::move(argument));
                } else if (identifier == "acosh") {
                    func = std::make_unique<NodeFunction<Function::acosh>>(std::move(argument));
                } else if (identifier == "atanh") {
                    func = std::make_unique<NodeFunction<Function::atanh>>(std::move(argument));
                } else if (identifier == "sqrt") {
                    func = std::make_unique<NodeFunction<Function::sqrt>>(std::move(argument));
                } else if (identifier == "exp") {
                    func = std::make_unique<NodeFunction<Function::exp>>(std::move(argument));
                } else if (identifier == "abs") {
                    func = std::make_unique<NodeFunction<Function::abs>>(std::move(argument));
                } else if (identifier == "log") {
                    func = std::make_unique<NodeFunction<Function::log>>(std::move(argument));
                } else if (identifier == "ln") {
                    func = std::make_unique<NodeFunction<Function::ln>>(std::move(argument));
                } else {
                    throw ParserException("Unexpected function \'" + identifier + "\'.");
                }

                if (lexer.getCurrentLexema() != Lexer::Lexema::closeParens) {
                    throw ParserException("Expected a close parenthesis after a function call \'" + identifier + "\'.");
                }
                lexer.goNextLexema();

                return func;
            } else { //it is a variable
                if (identifier == "x") {
                    return std::make_unique<NodeVariable>(variables[X_VAR_POS]);
                } else if (identifier == "y") {
                    return std::make_unique<NodeVariable>(variables[Y_VAR_POS]);
                } else if (identifier == "z") {
                    return std::make_unique<NodeVariable>(variables[Z_VAR_POS]);
                } else if (identifier == "pi") {
                    return std::make_unique<NodeConstant>(std::atan(1) * 4);
                } else if (identifier == "e") {
                    return std::make_unique<NodeConstant>(std::exp(1));
                } else if (constVariables.count(identifier) != 0) {
                    return std::make_unique<NodeConstant>(constVariables[identifier]);
                } else {
                    throw ParserException("Unexpected variable \'" + identifier + "\'.");
                }
            }
        }
        if (lexer.getCurrentLexema() == Lexer::Lexema::openParens) {
            lexer.goNextLexema();
            std::unique_ptr<Node> leaf = parseAddSubtract();

            if (lexer.getCurrentLexema() != Lexer::Lexema::closeParens) {
                throw ParserException("Expected a close parenthesis.");
            }
            lexer.goNextLexema();

            return leaf;
        }

        throw ParserException("Unexpected character.");
    }
};

std::unique_ptr<Node> parseExpression(const std::string &expression,
                                      const std::array<long double *, 3> &variableAddresses,
                                      const std::map<std::string, long double> &customConstVariables) {

    Parser parser(expression, variableAddresses, customConstVariables);

    return parser.parse();
}

} // namespace Parser
