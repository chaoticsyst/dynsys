#include <memory>
#include <vector>
#include <cmath>

#include "Lexer.hpp"
#include "Parser.hpp"

namespace Parser {

constexpr std::size_t X_VAR_POS = 0, Y_VAR_POS = 1, Z_VAR_POS = 2;

enum class BinaryOperation {
    Add, Subtract, Multiply, Divide
};

enum class UnaryOperation {
    Minus
};

struct NodeVariable final : Node {
    explicit NodeVariable(const long double *variableAddress) :
        value{variableAddress} {}

    const long double *value;

    long double calc() const noexcept override {
        return *value;
    }
};


struct NodeConstant final : Node {
    explicit NodeConstant(long double constantValue) :
        value{constantValue} {}

    long double value;

    long double calc() const noexcept override {
        return value;
    }
};

template<UnaryOperation>
struct NodeUnaryOperation final : Node {
    explicit NodeUnaryOperation(std::unique_ptr<Node> childNode) :
        child{std::move(childNode)} {}

    long double calc() const noexcept override;

    std::unique_ptr<Node> child;
};

template<>
long double NodeUnaryOperation<UnaryOperation::Minus>::calc() const noexcept {
    return -(child->calc());
}

template<BinaryOperation>
struct NodeBinaryOperation : Node {
    NodeBinaryOperation(std::unique_ptr<Node> leftNode, std::unique_ptr<Node> rightNode) :
        left{std::move(leftNode)}, right{std::move(rightNode)} {}

    long double calc() const noexcept override;

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

template<>
long double NodeBinaryOperation<BinaryOperation::Subtract>::calc() const noexcept {
    return left->calc() - right->calc();
}

template<>
long double NodeBinaryOperation<BinaryOperation::Add>::calc() const noexcept {
    return left->calc() + right->calc();
}

template<>
long double NodeBinaryOperation<BinaryOperation::Multiply>::calc() const noexcept {
    return left->calc() * right->calc();
}

template<>
long double NodeBinaryOperation<BinaryOperation::Divide>::calc() const noexcept {
    return left->calc() / right->calc();
}


class Parser final {
public:
    explicit Parser(const std::string &expression, const std::array<long double *, 3> &variableAddresses) :
        lexer{expression}, variables{variableAddresses} {}

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
                       lexer.getCurrentLexema() != Lexer::Lexema::X &&
                       lexer.getCurrentLexema() != Lexer::Lexema::Y &&
                       lexer.getCurrentLexema() != Lexer::Lexema::Z) {

                return leftPart;
            } else {
                throw ParserException("Unexpected character.");
            }
        }
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
                       lexer.getCurrentLexema() != Lexer::Lexema::X &&
                       lexer.getCurrentLexema() != Lexer::Lexema::Y &&
                       lexer.getCurrentLexema() != Lexer::Lexema::Z) {

                return leftPart;
            } else {
                throw ParserException("Unexpected character.");
            }
        }
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
        if (lexer.getCurrentLexema() == Lexer::Lexema::X) {
            std::unique_ptr<Node> leaf = std::make_unique<NodeVariable>(variables[X_VAR_POS]);
            lexer.goNextLexema();

            return  leaf;
        }
        if (lexer.getCurrentLexema() == Lexer::Lexema::Y) {
            std::unique_ptr<Node> leaf = std::make_unique<NodeVariable>(variables[Y_VAR_POS]);
            lexer.goNextLexema();

            return  leaf;
        }
        if (lexer.getCurrentLexema() == Lexer::Lexema::Z) {
            std::unique_ptr<Node> leaf = std::make_unique<NodeVariable>(variables[Z_VAR_POS]);
            lexer.goNextLexema();

            return  leaf;
        }
        if (lexer.getCurrentLexema() == Lexer::Lexema::OpenParens) {
            lexer.goNextLexema();
            std::unique_ptr<Node> leaf = parseAddSubtract();

            if (lexer.getCurrentLexema() != Lexer::Lexema::CloseParens) {
                throw ParserException("Close parenthesis was not found.");
            }
            lexer.goNextLexema();

            return leaf;
        }

        throw ParserException("Unexpected character was found.");
    }
};

std::shared_ptr<Node> parse_expression(const std::string &expression, const std::array<long double *, 3> &variableAddresses) {
    Parser parser(expression, variableAddresses);

    return parser.parse();
}

} // namespace Parser
