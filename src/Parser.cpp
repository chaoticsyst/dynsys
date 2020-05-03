#include <map>
#include <memory>
#include <vector>
#include <cmath>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "ParserNodes.hpp"

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

    std::shared_ptr<Node> parse() {
        return parseAddSubtract();
    }

private:
    Lexer::Lexer lexer;
    const std::array<long double *, 3> &variables;
    std::map<std::string, long double> constVariables;

    std::unique_ptr<Node> parseAddSubtract() {
        std::unique_ptr<Node> leftPart = parseMultiplyDivide();

        while (true) {
            if (lexer.getCurrentLexema() == Lexer::Lexema::Add) {
                lexer.goNextLexema();
                std::unique_ptr<Node> rightPart = parseMultiplyDivide();
                leftPart = std::make_unique<NodeBinaryOperation<BinaryOperation::Add>>(std::move(leftPart), std::move(rightPart));
            } else if (lexer.getCurrentLexema() == Lexer::Lexema::Subtract) {
                lexer.goNextLexema();
                std::unique_ptr<Node> rightPart = parseMultiplyDivide();
                leftPart = std::make_unique<NodeBinaryOperation<BinaryOperation::Subtract>>(std::move(leftPart), std::move(rightPart));
            } else if (lexer.getCurrentLexema() != Lexer::Lexema::OpenParens &&
                       lexer.getCurrentLexema() != Lexer::Lexema::Identifier) {

                break;
            } else {
                throw ParserException("Unexpected character.");
            }
        }

        return leftPart;
    }

    std::unique_ptr<Node> parseMultiplyDivide() {
        std::unique_ptr<Node> leftPart = parseUnary();

        while (true) {
            if (lexer.getCurrentLexema() == Lexer::Lexema::Multiply) {
                lexer.goNextLexema();
                std::unique_ptr<Node> rightPart = parseUnary();
                leftPart = std::make_unique<NodeBinaryOperation<BinaryOperation::Multiply>>(std::move(leftPart), std::move(rightPart));
            } else if (lexer.getCurrentLexema() == Lexer::Lexema::Divide) {
                lexer.goNextLexema();
                std::unique_ptr<Node> rightPart = parseUnary();
                leftPart = std::make_unique<NodeBinaryOperation<BinaryOperation::Divide>>(std::move(leftPart), std::move(rightPart));
            } else if (lexer.getCurrentLexema() != Lexer::Lexema::OpenParens &&
                       lexer.getCurrentLexema() != Lexer::Lexema::Identifier) {

                break;
            } else {
                throw ParserException("Unexpected character.");
            }
        }

        return leftPart;
    }

    std::unique_ptr<Node> parseUnary() {
        if (lexer.getCurrentLexema() == Lexer::Lexema::Subtract) {
            lexer.goNextLexema();

            return std::make_unique<NodeUnaryOperation<UnaryOperation::Minus>>(parseUnary());
        }

        return parseLeaf();
    }

    std::unique_ptr<Node> parseLeaf() {
        if (lexer.getCurrentLexema() == Lexer::Lexema::Constant) {
            std::unique_ptr<Node> leaf = std::make_unique<NodeConstant>(lexer.getCurrentConstant());
            lexer.goNextLexema();

            return leaf;
        }
        if (lexer.getCurrentLexema() == Lexer::Lexema::Identifier) {
            std::string identifier = lexer.getCurrentIdentifier();
            lexer.goNextLexema();
            if (lexer.getCurrentLexema() == Lexer::Lexema::OpenParens) { //it is a function
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

                if (lexer.getCurrentLexema() != Lexer::Lexema::CloseParens) {
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
                    return std::make_unique<NodeConstVariable>(std::atan(1) * 4);
                } else if (identifier == "e") {
                    return std::make_unique<NodeConstVariable>(std::exp(1));
                } else if (constVariables.count(identifier) != 0) {
                    return std::make_unique<NodeConstVariable>(constVariables[identifier]);
                } else {
                    throw ParserException("Unexpected variable \'" + identifier + "\'.");
                }
            }
        }
        if (lexer.getCurrentLexema() == Lexer::Lexema::OpenParens) {
            lexer.goNextLexema();
            std::unique_ptr<Node> leaf = parseAddSubtract();

            if (lexer.getCurrentLexema() != Lexer::Lexema::CloseParens) {
                throw ParserException("Expected a close parenthesis.");
            }
            lexer.goNextLexema();

            return leaf;
        }

        throw ParserException("Unexpected character.");
    }
};

std::shared_ptr<Node> parse_expression(const std::string &expression,
                                       const std::array<long double *, 3> &variableAddresses,
                                       const std::map<std::string, long double> &customConstVariables) {

    Parser parser(expression, variableAddresses, customConstVariables);

    return parser.parse();
}

} // namespace Parser
