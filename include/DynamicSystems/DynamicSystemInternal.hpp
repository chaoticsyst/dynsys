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
            getDerivativesFunction{derivativesFunctionGetter} {}

    void compute(LambdaNewPointAction newPointAction,
                 Model::Point point,
                 int pointsCount,
                 int stepsPerPoint,
                 long double timeDelta,
                 std::vector<long double> &constantValues) const {
        LambdaDerivatives derivativesFunction = getDerivativesFunction(constantValues);
        Model::generatePoints(newPointAction, point, pointsCount, stepsPerPoint, timeDelta, derivativesFunction);
    }

private:
    const std::function<LambdaDerivatives(std::vector<long double> &)> getDerivativesFunction;
};


} // namespace DynamicSystem