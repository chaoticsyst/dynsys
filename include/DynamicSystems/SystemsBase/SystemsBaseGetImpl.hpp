#pragma once

#include <vector>

#include "DynamicSystems/DynamicSystem.hpp"
#include "DynamicSystems/SystemsBase/SystemsBase.hpp"


namespace DynamicSystems {


template<typename LambdaNewPointAction>
std::vector<DynamicSystem<LambdaNewPointAction>> getDefaultSystems() {
    std::vector<DynamicSystem<LambdaNewPointAction>> systemsVector;
    systemsVector.push_back(AllSystems::getSystemLorenz<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemRossler<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemChua<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemHR3<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemAizawa<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemChenLee<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemAnishenkoAstakhov<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemBouali2<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemBurkeShaw<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemChenCelikovsky<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemCoullet<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemDadras<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemDequanLi<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemFinance<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemFourWing<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemGenesioTesi<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemHadley<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemHalvorsen<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemLiuChen<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemLorenzMod1<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemLorenzMod2<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemLuChen<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemNewtonLeipnik<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemNoseHoover<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemQiChen<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemRayleighBenard<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemRucklige<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemSakaraya<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemShimizuMorioka<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemThomas<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemTSUCS1<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemTSUCS2<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemWangSun<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemWimolBanlue<LambdaNewPointAction>());
    systemsVector.push_back(AllSystems::getSystemYuWang<LambdaNewPointAction>());
    return systemsVector;
}


} // namespace DynamicSystem
