#include "Locus.h"
#include "Preferences.h"

namespace Locus {

const char *VERTEX_NAME = "vertex";
const char *COLOR_NAME = "color";

Locus::Locus(const QVector<QVector3D> &points_, const QVector<QVector3D> &colors_) :
    points{points_}, colors{colors_} {}

const QVector3D *Locus::pointsData() const {
    return points.constData();
}

const QVector3D *Locus::colorsData() const {
    return colors.constData();
}

size_t Locus::size() const {
    return static_cast<size_t>(points.size());
}


size_t LocusController::size() const {
    return static_cast<size_t>(data.size());
}

void LocusController::addLocus(const Locus &locus) {
    data.push_back(locus);
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
        glDrawArrays(GL_POINTS,
                     std::max<int>(0, (int)std::min(locus.size(), amount) - Preferences::AMOUNT_TAIL_POINTS),
                     std::min(Preferences::AMOUNT_TAIL_POINTS, amount));
    }
    shaderProgram.disableAttributeArray(COLOR_NAME);
    shaderProgram.disableAttributeArray(VERTEX_NAME);
}

} //namespace Locus
