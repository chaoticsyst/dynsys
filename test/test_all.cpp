#include "gtest/gtest.h"
#include "Model.hpp"
#include "AttractorsParams.h"
#include "Parser.hpp"
#include "DynamicSystemsDefault.hpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(model, rossler_constants) {
    Model::Point start_point = {0.1, 0.1, 0.1};
    int required_count = 100'000;
    int steps_per_point = 10;
    long double tau = 0.01;

    int count = 0;
    auto counter = [&count](const Model::Point &) {
        ++count;
    };
    for (auto&[name, params] : AttractorsParams::goodParamsRossler) {
        count = 0;
        auto derivatives = Model::get_derivatives_function_rossler(params[0], params[1], params[2]);
        Model::generate_points(counter, start_point, required_count, steps_per_point, tau, derivatives);
        EXPECT_EQ(count, required_count) << "Rossler parameters \"" << name.toStdString() << "\" is not working";
    }
}


TEST(model, lorenz_constants) {
    Model::Point start_point = {0.1, 0.1, 0.1};
    int required_count = 100'000;
    int steps_per_point = 10;
    long double tau = 0.01;

    int count = 0;
    auto counter = [&count](const Model::Point &) {
        ++count;
    };
    for (auto&[name, params] : AttractorsParams::goodParamsLorenz) {
        count = 0;
        auto derivatives = Model::get_derivatives_function_lorenz(params[0], params[1], params[2]);
        Model::generate_points(counter, start_point, required_count, steps_per_point, tau, derivatives);
        EXPECT_EQ(count, required_count) << "Lorenz parameters \"" << name.toStdString() << "\" is not working";
    }
}

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
}

TEST(parser, lambda_test) {
    auto lambda = Parser::parseExpressions("x+y", "y - 1.1*z", "z - 1", {});
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
                                        {{"a", 0.8}, {"bedy", 10.5}, {"c", -1}});
    EXPECT_NEAR(func->calc(), 11.4, 1e-10);
    func = Parser::parseExpression("(2 * abcd - 3) + (2 * abcd * abcd * (abc - 2)) / 3 - 1",
                                    {nullptr, nullptr, nullptr},
                                    {{"abcd", 11.7}, {"abc", -3.15}});
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
                Parser::parseExpression("e * e - sin(pi) * cos(112.2)", {nullptr, nullptr, nullptr})->calc(), 1e-10);
}
