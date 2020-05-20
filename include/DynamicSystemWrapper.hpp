#pragma once

#include <QVector>
#include <QVector3D>
#include <Model/Model.hpp>

namespace DynamicSystemWrapper_n {

inline auto getPushBackAndNormalizeLambda(QVector<QVector3D> &vector, float normalizeConstant) {
    return [&vector, normalizeConstant](const Model::Point &point) {
        vector.push_back(
                QVector3D(static_cast<float>(point.x) / normalizeConstant,
                          static_cast<float>(point.y) / normalizeConstant,
                          static_cast<float>(point.z) / normalizeConstant)
        );
    };
}

}