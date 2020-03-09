#include "Locus.h"
#include "Preferences.h"

namespace Locus {

const char *VERTEX_NAME = "vertex";
const char *COLOR_NAME = "color";

Locus::Locus(QVector<QVector3D> &&points_, QVector<QVector3D> &&colors_) :
    points{std::move(points_)}, colors{std::move(colors_)} {

    interpolate();
}

const QVector3D *Locus::pointsData() const {
    return points.constData();
}

const QVector3D *Locus::colorsData() const {
    return colors.constData();
}

size_t Locus::size() const {
    return static_cast<size_t>(points.size());
}

QVector3D Locus::getInterpolatedPoint(float offset, size_t startIndex) {
    QVector3D &pivotPrev   = points[startIndex];
    QVector3D &pivotFirst  = points[startIndex + 1];
    QVector3D &pivotSecond = points[startIndex + 2];
    QVector3D &pivotNext   = points[startIndex + 3];
    return 0.5f * ((2 * pivotFirst)
                + offset * ((-pivotPrev + pivotSecond)
                + offset * ((2 * pivotPrev - 5 * pivotFirst + 4 * pivotSecond - pivotNext)
                + offset * (-pivotPrev + 3 * pivotFirst - 3 * pivotSecond + pivotNext))));
}

void Locus::interpolate() {
    QVector<QVector3D> result;
    result.reserve(2 * points.size());

    result << points[0];
    for (size_t i = 0; i < static_cast<size_t>(points.size() - 2); i++) {
        result.push_back(points[i]);

        float distance = points[i].distanceToPoint(points[i + 1]);
        size_t cuts = distance / Preferences::DISTANCE_DELTA;
        float dt = 1.0 / (cuts + 1);
        for (size_t j = 0; j < cuts; j++) {
            result.push_back(getInterpolatedPoint(dt * (j + 1), i - 1));
        }
    }
    result << points[points.size() - 2] << points[points.size() - 1];

    points = std::move(result);
}


size_t LocusController::size() const {
    return static_cast<size_t>(data.size());
}

void LocusController::addLocus(Locus &&locus) {
    data.push_back(std::move(locus));
}

void LocusController::removeLocus(size_t index) {
    data.removeAt(index);
}

void LocusController::clear() {
    data.clear();
}

void LocusController::draw(QGLShaderProgram &shaderProgram, size_t amount) const {
    for (const auto &locus : data) {
        shaderProgram.setAttributeArray(VERTEX_NAME, locus.pointsData());
        shaderProgram.setAttributeArray(COLOR_NAME, locus.colorsData());
        shaderProgram.enableAttributeArray(VERTEX_NAME);
        shaderProgram.enableAttributeArray(COLOR_NAME);
        glDrawArrays(GL_LINE_STRIP,
                     std::max<int>(0, (int)std::min(locus.size(), amount) - Preferences::AMOUNT_TAIL_POINTS),
                     std::min(Preferences::AMOUNT_TAIL_POINTS, amount));
    }
    shaderProgram.disableAttributeArray(COLOR_NAME);
    shaderProgram.disableAttributeArray(VERTEX_NAME);
}

} //namespace Locus
