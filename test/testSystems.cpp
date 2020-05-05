#include "gtest/gtest.h"
#include "Model.hpp"
#include "DynamicSystems/DynamicSystem.hpp"

TEST(model, allConstantRK4_1) {
    int convergesCount = 0;

    Model::Point startPoint = {0.1, 0.1, 0.1};
    int requiredCount = 100'000;
    long double tau = 0.01;

    int count = 0;
    auto counter = [&count](const Model::Point &) {
        ++count;
    };

    auto vectorSystems = DynamicSystems::getDefaultSystems<decltype(counter)>();

    for (auto &system : vectorSystems) {
        auto constantValues = system.getInterestingConstants();
        for (auto&[name, params] : constantValues) {
            count = 0;
            system.compute(counter, startPoint, requiredCount, 1, tau, params);
            if (count == requiredCount) convergesCount++;
        }
    }
    EXPECT_GE(convergesCount, 33);
}

TEST(model, allConstantRK4_2) {
    int convergesCount = 0;

    Model::Point startPoint = {0.01, 0.01, 0.01};
    int requiredCount = 500'000;
    long double tau = 0.01;

    int count = 0;
    auto counter = [&count](const Model::Point &) {
        ++count;
    };

    auto vectorSystems = DynamicSystems::getDefaultSystems<decltype(counter)>();

    for (auto &system : vectorSystems) {
        auto constantValues = system.getInterestingConstants();
        for (auto&[name, params] : constantValues) {
            count = 0;
            system.compute(counter, startPoint, requiredCount, 1, tau, params);
            if (count == requiredCount) convergesCount++;
        }
    }
    EXPECT_GE(convergesCount, 28);
}

TEST(model, allConstantRK4_3) {
    int convergesCount = 0;

    Model::Point startPoint = {0.01, 0.01, 0.01};
    int requiredCount = 200'000;
    long double tau = 0.0001;

    int count = 0;
    auto counter = [&count](const Model::Point &) {
        ++count;
    };

    auto vectorSystems = DynamicSystems::getDefaultSystems<decltype(counter)>();

    for (auto &system : vectorSystems) {
        auto constantValues = system.getInterestingConstants();
        for (auto&[name, params] : constantValues) {
            count = 0;
            system.compute(counter, startPoint, requiredCount, 1, tau, params);
            if (count == requiredCount) convergesCount++;
        }
    }
    EXPECT_GE(convergesCount, 38);
}

TEST(model, allConstantRK4_4) {
    int convergesCount = 0;

    Model::Point startPoint = {0.01, 0.01, 0.01};
    int requiredCount = 100'000;
    long double tau = 0.1;

    int count = 0;
    auto counter = [&count](const Model::Point &) {
        ++count;
    };

    auto vectorSystems = DynamicSystems::getDefaultSystems<decltype(counter)>();

    for (auto &system : vectorSystems) {
        auto constantValues = system.getInterestingConstants();
        for (auto&[name, params] : constantValues) {
            count = 0;
            system.compute(counter, startPoint, requiredCount, 1, tau, params);
            if (count == requiredCount) convergesCount++;
        }
    }
    EXPECT_GE(convergesCount, 20);
}