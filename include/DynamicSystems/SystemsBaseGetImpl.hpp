#pragma once

#include <vector>

#include "DynamicSystem.hpp"
#include "SystemsBase.hpp"


namespace DynamicSystems {


template<typename LambdaNewPointAction>
std::vector<DynamicSystem<LambdaNewPointAction>> getDefaultSystems() {
    std::vector<DynamicSystem<LambdaNewPointAction>> systemsVector;
    systemsVector.push_back(getSystemLorenz<LambdaNewPointAction>());
    systemsVector.push_back(getSystemRossler<LambdaNewPointAction>());
    systemsVector.push_back(getSystemChua<LambdaNewPointAction>());
    systemsVector.push_back(getSystemHR3<LambdaNewPointAction>());
    systemsVector.push_back(getSystemAizawa<LambdaNewPointAction>());
    systemsVector.push_back(getSystemChenLee<LambdaNewPointAction>());
    systemsVector.push_back(getSystemAnishenkoAstakhov<LambdaNewPointAction>());
    systemsVector.push_back(getSystemBouali2<LambdaNewPointAction>());
    systemsVector.push_back(getSystemBurkeShaw<LambdaNewPointAction>());
    systemsVector.push_back(getSystemChenCelikovsky<LambdaNewPointAction>());
    systemsVector.push_back(getSystemCoullet<LambdaNewPointAction>());
    systemsVector.push_back(getSystemDadras<LambdaNewPointAction>());
    systemsVector.push_back(getSystemDequanLi<LambdaNewPointAction>());
    systemsVector.push_back(getSystemFinance<LambdaNewPointAction>());
    systemsVector.push_back(getSystemFourWing<LambdaNewPointAction>());
    systemsVector.push_back(getSystemGenesioTesi<LambdaNewPointAction>());
    systemsVector.push_back(getSystemHadley<LambdaNewPointAction>());
    systemsVector.push_back(getSystemHalvorsen<LambdaNewPointAction>());
    systemsVector.push_back(getSystemLiuChen<LambdaNewPointAction>());
    systemsVector.push_back(getSystemLorenzMod1<LambdaNewPointAction>());
    systemsVector.push_back(getSystemLorenzMod2<LambdaNewPointAction>());
    systemsVector.push_back(getSystemLuChen<LambdaNewPointAction>());
    systemsVector.push_back(getSystemNewtonLeipnik<LambdaNewPointAction>());
    systemsVector.push_back(getSystemNoseHoover<LambdaNewPointAction>());
    systemsVector.push_back(getSystemQiChen<LambdaNewPointAction>());
    systemsVector.push_back(getSystemRayleighBenard<LambdaNewPointAction>());
    systemsVector.push_back(getSystemRucklige<LambdaNewPointAction>());
    systemsVector.push_back(getSystemSakaraya<LambdaNewPointAction>());
    systemsVector.push_back(getSystemShimizuMorioka<LambdaNewPointAction>());
    systemsVector.push_back(getSystemThomas<LambdaNewPointAction>());
    systemsVector.push_back(getSystemTSUCS1<LambdaNewPointAction>());
    systemsVector.push_back(getSystemTSUCS2<LambdaNewPointAction>());
    systemsVector.push_back(getSystemWangSun<LambdaNewPointAction>());
    systemsVector.push_back(getSystemWimolBanlue<LambdaNewPointAction>());
    systemsVector.push_back(getSystemYuWang<LambdaNewPointAction>());
    return systemsVector;
}


} // namespace DynamicSystem
