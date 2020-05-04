#include "gtest/gtest.h"
#include "Model.hpp"
#include "AttractorsParams.hpp"
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
