#include "Locus.h"
#include "Preferences.h"

namespace Locus {

Locus::Locus(QVector<QVector3D> &&points_, const QColor &color_) :
    color{color_} {

    points_ = interpolate(std::move(points_));
    pointsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    pointsBuffer.create();
    pointsBuffer.bind();
    pointsBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    pointsBuffer.allocate(points_.begin(), points_.size() * 3 * sizeof(float));
    pointsBuffer.release();
}

void Locus::startWork() {
    pointsBuffer.bind();
}

void Locus::endWork() {
    pointsBuffer.release();
}

const QColor &Locus::colorData() const {
    return color;
}

size_t Locus::size() const {
    return static_cast<size_t>(pointsBuffer.size() / 3 / sizeof(float));
}

QVector3D Locus::getInterpolatedPoint(float offset, const QVector<QVector3D> &points, size_t startIndex) {
    if (startIndex + 3 >= static_cast<size_t>(points.size())) {
        return points[startIndex];
    }
    const QVector3D &pivotPrev   = points[startIndex];
    const QVector3D &pivotFirst  = points[startIndex + 1];
    const QVector3D &pivotSecond = points[startIndex + 2];
    const QVector3D &pivotNext   = points[startIndex + 3];
    return 0.5f * ((2 * pivotFirst)
                + offset * ((-pivotPrev + pivotSecond)
                + offset * ((2 * pivotPrev - 5 * pivotFirst + 4 * pivotSecond - pivotNext)
                + offset * (-pivotPrev + 3 * pivotFirst - 3 * pivotSecond + pivotNext))));
}

QVector<QVector3D> Locus::interpolate(const QVector<QVector3D> &points) {
    if (points.size() < 2) {
        return points;
    }

    QVector<QVector3D> result;
    result.reserve(2 * points.size());

    result << points[0];
    for (size_t i = 1; i < static_cast<size_t>(points.size() - 2); i++) {
        result.push_back(points[i]);

        float distance = points[i].distanceToPoint(points[i + 1]);
        size_t cuts = distance / Preferences::DISTANCE_DELTA;
        float dt = 1.0 / (cuts + 1);
        for (size_t j = 0; j < cuts; j++) {
            result.push_back(getInterpolatedPoint(dt * (j + 1), points, i - 1));
        }
    }
    result << points[points.size() - 2] << points[points.size() - 1];

    return result;
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

void LocusController::draw(QGLShaderProgram &shaderProgram, size_t amount) {
    for (auto &locus : data) {
        locus.startWork();
        shaderProgram.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
        shaderProgram.setUniformValue(Preferences::COLOR_NAME, locus.colorData());
        glDrawArrays(GL_LINE_STRIP,
                     std::max<int>(0, static_cast<int>(std::min(locus.size(), amount)) - Preferences::AMOUNT_TAIL_POINTS),
                     std::min(Preferences::AMOUNT_TAIL_POINTS, amount));

        locus.endWork();
    }
}

} //namespace Locus
