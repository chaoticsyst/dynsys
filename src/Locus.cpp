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


void LocusController::initialize() {
    shaderController.initialize();
}

size_t LocusController::size() const {
    return static_cast<size_t>(data.size());
}

void LocusController::addLocus(QVector<QVector3D> &&points_, const QColor &color_) {
    shaderController.startWork();
    data.push_back(Locus(std::move(points_), color_));
    shaderController.endWork();
}

void LocusController::clear() {
    data.clear();
}

void LocusController::draw(const QMatrix4x4 &projMatrix, size_t length) {
    shaderController.startWork();
    shaderController.setMatrix(projMatrix);
    for (auto &locus : data) {
        locus.startWork();
        if (locus.initialSize() == 0) {
            continue;
        }

        int curAmount = std::min(locus.initialSize(), length);
        size_t start = std::max(0, curAmount - static_cast<int>(Preferences::AMOUNT_TAIL_POINTS));
        size_t actualLength = locus.getStartIndex(curAmount) - locus.getStartIndex(start);

        shaderController.setVertex();
        shaderController.setColor(locus.colorData());

        shaderController.setArcadeMode(Preferences::ARCADE_MODE_ON);
        shaderController.setStartTailSize(Preferences::START_POINT_SIZE);
        shaderController.setFinalTailSize(Preferences::FINAL_POINT_SIZE);
        shaderController.setTailLength(actualLength);
        shaderController.setStartVertexIndex(locus.getStartIndex(start));

        glDrawArrays(Preferences::PRIMITIVE, locus.getStartIndex(start), actualLength);

        locus.endWork();
    }
    shaderController.endWork();
}

} //namespace Locus
