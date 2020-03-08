#pragma once

#include <QVector>

namespace AttractorsParams {

QVector<std::pair<QString, QVector<double>>> goodParamsRossler = {
    {"Классические значения", {0.2, 0.2, 5.7}},
    {"Устойчивый предельный цикл", {0.2, 0.2, 3}},
    {"Популярные значения", {0.1, 0.1, 14}},
    {"Расхождение аттрактора", {0.3, 0.1, 14}}
};

QVector<std::pair<QString, QVector<double>>> goodParamsLorenz = {
    {"Классические значения", {10, 28, 8.0 / 3.0,}},
    {"Схождение в цикл", {10, 100, 8.0 / 3.0}}
};

}
