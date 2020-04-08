#include "Locus.h"
#include "Preferences.h"

namespace Locus {

Locus::Locus(QVector<QVector3D> &&points_, const QColor &color_, QGLShaderProgram *shaderProgram_) :
    color{color_}, shaderProgram{shaderProgram_} {

    points_ = interpolate(std::move(points_));
    shaderProgram->bind();
    pointsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    pointsBuffer.create();
    pointsBuffer.bind();
    pointsBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    pointsBuffer.allocate(points_.begin(), points_.size() * 3 * sizeof(float));
    pointsBuffer.release();
    shaderProgram->release();
}

void Locus::startWork(size_t pointsNumber, size_t startIndex) {
    pointsBuffer.bind();
    shaderProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
    shaderProgram->setUniformValue(Preferences::COLOR_NAME, colorData());
    shaderProgram->setUniformValue("decreasingTailMode", true);
    shaderProgram->setUniformValue("startTailSize", 1.0f);
    shaderProgram->setUniformValue("finalTailSize", 15.0f);
    shaderProgram->setUniformValue("pointsNumber", static_cast<int>(pointsNumber));
    shaderProgram->setUniformValue("startIndex", static_cast<int>(startIndex));
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

size_t Locus::initialSize() const {
    return startIndexes.size();
}

size_t Locus::getStartIndex(size_t initialIndex) const {
    initialIndex = std::min<size_t>(initialIndex, startIndexes.size() - 1);
    return startIndexes[initialIndex];
}

QVector3D Locus::getInterpolatedPoint(float offset, const QMatrix4x4 &matrix) const {
    const static QMatrix4x4 curveMatrix( 0,  2,  0,  0,
                                        -1,  0,  1,  0,
                                         2, -5,  4, -1,
                                        -1,  3, -3,  1);
    auto res = 0.5 * QVector4D(1, offset, offset * offset, offset * offset * offset) * curveMatrix * matrix;
    return {res.x(), res.y(), res.z()};
}


QVector<QVector3D> Locus::interpolate(const QVector<QVector3D> &points) {
    if (points.size() < 2) {
        return points;
    }

    QVector<QVector3D> result;
    QMatrix4x4 curMatrix;

    result << points[0];
    startIndexes << 0;
    for (size_t i = 1; i < static_cast<size_t>(points.size() - 2); i++) {
        startIndexes << result.size();

        result.push_back(points[i]);

        float distance = points[i].distanceToPoint(points[i + 1]);
        size_t cuts = distance / Preferences::DISTANCE_DELTA;
        if (cuts == 0) {
            continue;
        }
        curMatrix.setRow(0, points[i - 1]);
        curMatrix.setRow(1, points[i + 0]);
        curMatrix.setRow(2, points[i + 1]);
        curMatrix.setRow(3, points[i + 2]);
        float dt = 1.0 / (cuts + 1);
        for (size_t j = 0; j < cuts; j++) {
            result << getInterpolatedPoint(dt * (j + 1), curMatrix);
        }
    }
    startIndexes << result.size();
    result << points[points.size() - 2] << points[points.size() - 1];

    return result;
}


size_t LocusController::size() const {
    return static_cast<size_t>(data.size());
}

void LocusController::addLocus(Locus &&locus) {
    data.push_back(std::move(locus));
}

void LocusController::clear() {
    data.clear();
}

void LocusController::draw(size_t amount) {
    for (auto &locus : data) {
        locus.startWork(0, 0);
        int curAmount = std::min(locus.initialSize(), amount);
        size_t start = std::max(0, curAmount - static_cast<int>(Preferences::AMOUNT_TAIL_POINTS));
        size_t length = locus.getStartIndex(curAmount) - locus.getStartIndex(start);
        locus.startWork(length, locus.getStartIndex(start));
        glDrawArrays(GL_POINTS, locus.getStartIndex(start), length);

        locus.endWork();
    }
}

} //namespace Locus
