#pragma once

#include <vector>

#include "DynamicSystem.hpp"
#include "SystemsBase.hpp"


namespace DynamicSystems {


template<typename T>
std::vector<DynamicSystem<T>> getDefaultSystems() {
    std::vector<DynamicSystem<T>> systemsVector;
    systemsVector.push_back(getSystemLorenz<T>());
    systemsVector.push_back(getSystemRossler<T>());
    systemsVector.push_back(getSystemChua<T>());
    systemsVector.push_back(getSystemHR3<T>());
    systemsVector.push_back(getSystemAizawa<T>());
    systemsVector.push_back(getSystemChenLee<T>());
    systemsVector.push_back(getSystemAnishenkoAstakhov<T>());
    systemsVector.push_back(getSystemBouali2<T>());
    systemsVector.push_back(getSystemBurkeShaw<T>());
    systemsVector.push_back(getSystemChenCelikovsky<T>());
    systemsVector.push_back(getSystemCoullet<T>());
    systemsVector.push_back(getSystemDadras<T>());
    systemsVector.push_back(getSystemDequanLi<T>());
    systemsVector.push_back(getSystemFinance<T>());
    systemsVector.push_back(getSystemFourWing<T>());
    systemsVector.push_back(getSystemGenesioTesi<T>());
    systemsVector.push_back(getSystemHadley<T>());
    systemsVector.push_back(getSystemHalvorsen<T>());
    systemsVector.push_back(getSystemLiuChen<T>());
    systemsVector.push_back(getSystemLorenzMod1<T>());
    systemsVector.push_back(getSystemLorenzMod2<T>());
    systemsVector.push_back(getSystemLuChen<T>());
    systemsVector.push_back(getSystemNewtonLeipnik<T>());
    systemsVector.push_back(getSystemNoseHoover<T>());
    systemsVector.push_back(getSystemQiChen<T>());
    systemsVector.push_back(getSystemRayleighBenard<T>());
    systemsVector.push_back(getSystemRucklige<T>());
    systemsVector.push_back(getSystemSakaraya<T>());
    systemsVector.push_back(getSystemShimizuMorioka<T>());
    systemsVector.push_back(getSystemThomas<T>());
    systemsVector.push_back(getSystemTSUCS1<T>());
    systemsVector.push_back(getSystemTSUCS2<T>());
    systemsVector.push_back(getSystemWangSun<T>());
    systemsVector.push_back(getSystemWimolBanlue<T>());
    systemsVector.push_back(getSystemYuWang<T>());
    return systemsVector;
}


} // namespace DynamicSystem
