#pragma once

#include <functional>
#include <vector>

#include "Model/Model.hpp"


namespace DynamicSystems {


template<typename LambdaNewPointAction, typename LambdaDerivatives>
class DynamicSystemInternal final {
public:
    explicit DynamicSystemInternal(
            std::function<LambdaDerivatives(std::vector<long double> &)> derivativesFunctionGetter) :
            getDerivativesFunction{std::move(derivativesFunctionGetter)} {}

    void compute(LambdaNewPointAction newPointAction,
                 Model::Point point,
                 int pointsCount,
                 long double timeDelta,
                 std::vector<long double> &constantValues) const {
        Model::generatePoints(newPointAction, point, pointsCount, timeDelta, getDerivativesFunction(constantValues));
    }

private:
    const std::function<LambdaDerivatives(std::vector<long double> &)> getDerivativesFunction;
};


} // namespace DynamicSystem