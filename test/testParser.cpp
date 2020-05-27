#include <cmath>

#include "gtest/gtest.h"
#include "Parser/Parser.hpp"
#include "DynamicSystemParser/DynamicSystemParser.hpp"

TEST(parser, simple_integer_arithmetic) {
    auto func = Parser::parseExpression("2+1", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 3);
    func = Parser::parseExpression("6 - 1", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 5);
    func = Parser::parseExpression("1 + 2 - 6 - 1", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), -4);
    func = Parser::parseExpression("2 * 12", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 24);
    func = Parser::parseExpression("16/8", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 2);
    func = Parser::parseExpression("2+2*3", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 8);
    func = Parser::parseExpression("2/2*3", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 3);
    func = Parser::parseExpression("2*(1+2)/3", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 2);
    func = Parser::parseExpression("1 + 2 * ((3) + (2 * 3)) / 3", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 7);
    func = Parser::parseExpression("2^3", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 8);
    func = Parser::parseExpression("2^3^2 - (2^3)^2", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 448);
}

TEST(parser, simple_float_arithmetic) {
    auto func = Parser::parseExpression("1 / 2", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), 0.5, 0.0000001);
    func = Parser::parseExpression("1.12 + 2.34", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), 3.46, 0.0000001);
    func = Parser::parseExpression("1.567", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), 1.567, 0.0000001);
    func = Parser::parseExpression("1,567 + 1,1", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), 2.667, 0.0000001);
    func = Parser::parseExpression("1.12 + 2.3*1.1 - 1.2", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), (1.12 + 2.3 * 1.1 - 1.2), 0.0000001);
    func = Parser::parseExpression("1.12*(2 + 3.7) - 8.456", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), (1.12 * (2 + 3.7) - 8.456), 0.0000001);
    func = Parser::parseExpression("3.72*(2.1345 + 3.7)/1.32 - 1.2453*8.456", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), (3.72 * (2.1345 + 3.7) / 1.32 - 1.2453 * 8.456), 0.0000001);
    func = Parser::parseExpression("1.12^4 - 2.5^3.6", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), -25.502, 1e-3);
}

TEST(parser, simple_variables_arithmetic) {
    long double x = 1, y = 1, z = 1;
    auto func = Parser::parseExpression("(1 + x) / 2", {&x, &y, &z});
    EXPECT_NEAR(func->calc(), 1, 0.0000001);
    x = -4;
    z = y = 20;
    EXPECT_NEAR(func->calc(), -1.5, 0.0000001);
    func = Parser::parseExpression("(y + x) / z", {&x, &y, &z});
    EXPECT_NEAR(func->calc(), (y + x) / z, 0.0000001);
    x = 12.34;
    y = -765.1;
    EXPECT_NEAR(func->calc(), (y + x) / z, 0.0000001);
    z = 1.2;
    EXPECT_NEAR(func->calc(), (y + x) / z, 0.0000001);
}

TEST(parser, parse_errors) {
    long double x = 1, y = 1, z = 1;
    auto func = Parser::parseExpression("(1)", {&x, &y, &z});
    func = Parser::parseExpression("1 / 0", {&x, &y, &z});
    long double val = func->calc();
    EXPECT_EQ(val, INFINITY);
    bool caught = false;
    try {
        func = Parser::parseExpression("(1++1)", {&x, &y, &z});
    } catch (const Parser::ParserException &) {
        caught = true;
    }
    EXPECT_TRUE(caught);
    caught = false;
    try {
        func = Parser::parseExpression("()((1+)1)", {&x, &y, &z});
    } catch (const Parser::ParserException &) {
        caught = true;
    }
    EXPECT_TRUE(caught);
    caught = false;
    try {
        func = Parser::parseExpression("2*(21 - 1?1)", {&x, &y, &z});
    } catch (const Parser::ParserException &) {
        caught = true;
    }
    EXPECT_TRUE(caught);
    caught = false;
    try {
        func = Parser::parseExpression("2(21 - 1)", {&x, &y, &z});
    } catch (const Parser::ParserException &) {
        caught = true;
    }
    EXPECT_TRUE(caught);
    caught = false;
    try {
        func = Parser::parseExpression("2z - x", {&x, &y, &z});
    } catch (const Parser::ParserException &) {
        caught = true;
    }
    EXPECT_TRUE(caught);
    caught = false;
    try {
        func = Parser::parseExpression("sin(5", {&x, &y, &z});
    } catch (const Parser::ParserException &) {
        caught = true;
    }
    EXPECT_TRUE(caught);
    caught = false;
    try {
        func = Parser::parseExpression("a + 3 - 2 * c", {&x, &y, &z}, {{"a", 3}});
    } catch (const Parser::ParserException &) {
        caught = true;
    }
    EXPECT_TRUE(caught);
}

TEST(parser, lambda_test) {
    std::vector<long double> emptyVector;
    auto lambda = DynamicSystemParser::Impl::parseExpressions("x+y", "y - 1.1*z", "z - 1", {}).operator()(emptyVector);
    EXPECT_NEAR(lambda({1, 1, 1}).x, (Model::Point{2, -0.1, 0}).x, 1e-10);
    EXPECT_NEAR(lambda({1, 1, 1}).y, (Model::Point{2, -0.1, 0}).y, 1e-10);
    EXPECT_NEAR(lambda({1, 1, 1}).z, (Model::Point{2, -0.1, 0}).z, 1e-10);
    EXPECT_NEAR(lambda({-1, 0, 2}).x, (Model::Point{-1, -2.2, 1}).x, 1e-10);
    EXPECT_NEAR(lambda({-1, 0, 2}).y, (Model::Point{-1, -2.2, 1}).y, 1e-10);
    EXPECT_NEAR(lambda({-1, 0, 2}).z, (Model::Point{-1, -2.2, 1}).z, 1e-10);
}

TEST(parser, strange_format) {
    auto func = Parser::parseExpression("2\n +\n 1\n", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 3);
    func = Parser::parseExpression("6 - 1\n", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 5);
    func = Parser::parseExpression("   1 + 2 - 6 - 1", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), -4);
    func = Parser::parseExpression("   2   *    12    ", {nullptr, nullptr, nullptr});
    EXPECT_EQ(func->calc(), 24);
}

TEST(parser, use_variables) {
    auto func = Parser::parseExpression("2 + a * bedy - c", {nullptr, nullptr, nullptr},
                                        {{"a",    0.8},
                                         {"bedy", 10.5},
                                         {"c",    -1}});
    EXPECT_NEAR(func->calc(), 11.4, 1e-10);
    func = Parser::parseExpression("(2 * abcd - 3) + (2 * abcd * abcd * (abc - 2)) / 3 - 1",
                                   {nullptr, nullptr, nullptr},
                                   {{"abcd", 11.7},
                                    {"abc",  -3.15}});
    EXPECT_NEAR(func->calc(), -450.589, 1e-10);
    func = Parser::parseExpression("3 * pi - e * 2 - (3 + e + pi) / (pi * pi - 2)", {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), 2.862, 1e-3);
}

TEST(parser, function_calls) {
    auto func = Parser::parseExpression("sin(pi) + cos(2 * tan(pi)) - 3 * sin(2) / cos(sin(3))",
                                        {nullptr, nullptr, nullptr});
    EXPECT_NEAR(func->calc(), -1.755, 1e-3);
    func = Parser::parseExpression("ln(100) * log(13) / 2 * (atan(3 + sin(hello)) - atanh(0.3)) * acos(0.5) - hello * sqrt(abs(hello))",
                                   {nullptr, nullptr, nullptr},
                                   {{"hello", -123}});
    EXPECT_NEAR(func->calc(), 1366.768, 1e-3);
    EXPECT_NEAR(Parser::parseExpression("exp(2)", {nullptr, nullptr, nullptr})->calc(),
                Parser::parseExpression("e ^ 2 - sin(pi) * cos(112.2)", {nullptr, nullptr, nullptr})->calc(), 1e-10);
}

TEST(parser, strong_tests) {
    auto func = Parser::parseExpression("sin(priv^ 2 * abs(-priv * priv * e / pre) + sin(3 * pi / "
                                        "cos(tan(log(acos(pi * (1 / pre / pre)) + e^(priv - pre + prev - 0.5)^2)"
                                        "/ 2 + priv / priv * priv * (e - pi / 2 + asinh(acosh(10) - prev * (3 / 2.5)))"
                                        "- 100 / sqrt(e^3)^3) + pi^e^e) - 10^(e + e / 2 - 3)) - 100)",
                                        {nullptr, nullptr, nullptr},
                                        {{"priv", 10},
                                         {"pre",  2},
                                         {"prev", 2.5}});
    EXPECT_NEAR(func->calc(), 0.8358, 1e-4);
    func = Parser::parseExpression("ln(sin(asin(a / 5 * 2) + asin(0.3232) * cos(abs(aaa * 20) / e)"
                                   " + e^(a * 20 - aa - aaa * 3) - e^5 * e^(aa + aa)"
                                   "/ pi^10) ^abs(aaa * 6) / ln(sin(cos(sin(cos(3) + 2) + aa * 2) - (1 / aa))"
                                   "+ ln(100) + atanh(0.3))^3 + 100)", {nullptr, nullptr, nullptr},
                                   {{"a",   0.5},
                                    {"aa",  1.5},
                                    {"aaa", -0.5}});
    EXPECT_NEAR(func->calc(), 4.6044, 1e-4);
}
