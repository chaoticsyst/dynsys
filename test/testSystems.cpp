#include "gtest/gtest.h"
#include "Model/Model.hpp"
#include "DynamicSystems/DynamicSystem.hpp"

auto getCounter(int &count) {
    return [&count](const Model::Point &) {
        ++count;
    };
}

int countConvergesSystems(const Model::Point &startPoint, int requiredCount, long double tau) {
    int convergesCount = 0;

    auto vectorSystems = DynamicSystems::getDefaultSystems<decltype(getCounter(std::declval<int &>()))>();

    for (auto &system : vectorSystems) {
        auto constantValues = system.getInterestingConstants();
        for (auto&[name, params] : constantValues) {
            int count = 0;
            system.compute(getCounter(count), startPoint, requiredCount, tau, params);
            if (count == requiredCount) convergesCount++;
        }
    }
    return convergesCount;
}

TEST(model, allConstantRK4_1) {
    Model::Point startPoint = {0.2, 0.3, 0.1};
    int requiredCount = 100'000;
    long double tau = 0.01;

    EXPECT_GE(countConvergesSystems(startPoint, requiredCount, tau), 34);
}


TEST(model, allConstantRK4_2) {
    Model::Point startPoint = {0.02, 0.03, 0.01};
    int requiredCount = 500'000;
    long double tau = 0.01;

    EXPECT_GE(countConvergesSystems(startPoint, requiredCount, tau), 33);
}

TEST(model, allConstantRK4_3) {
    Model::Point startPoint = {0.02, 0.03, 0.01};
    int requiredCount = 200'000;
    long double tau = 0.0001;

    EXPECT_GE(countConvergesSystems(startPoint, requiredCount, tau), 37);
}

TEST(model, allConstantRK4_4) {
    Model::Point startPoint = {0.02, 0.03, 0.01};
    int requiredCount = 100'000;
    long double tau = 0.1;

    EXPECT_GE(countConvergesSystems(startPoint, requiredCount, tau), 21);
}