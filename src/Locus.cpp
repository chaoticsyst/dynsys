#include "Locus.h"
#include "Preferences.h"

namespace Locus {

Locus::Locus(QVector<QVector3D> &&points) {
    pointsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    pointsBuffer.create();
    pointsBuffer.bind();
    pointsBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    pointsBuffer.allocate(points.begin(), points.size() * 3 * sizeof(float));
    pointsBuffer.release();
}

void Locus::startWork() {
    pointsBuffer.bind();
}

void Locus::endWork() {
    pointsBuffer.release();
}

size_t Locus::size() const {
    return static_cast<size_t>(pointsBuffer.size() / 3 / sizeof(float));
}

void LocusController::initialize() {
    shaderController.initialize();
}

size_t LocusController::size() const {
    return static_cast<size_t>(data.size());
}

void LocusController::addLocus(QVector<QVector3D> &&points_) {
    shaderController.startWork();
    data.push_back(Locus(std::move(points_)));
    shaderController.endWork();
}

void LocusController::clear() {
    data.clear();
}

void LocusController::draw(const QMatrix4x4 &projMatrix, size_t time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderController.startWork();
    shaderController.setMatrix(projMatrix);

    shaderController.setArcadeMode(Preferences::ARCADE_MODE_ON);

    shaderController.setTailColoringMode(Preferences::TAIL_COLORING_MODE);
    shaderController.setTrajectoriesNumber(static_cast<size_t>(data.size()));
    shaderController.setColors(Preferences::COLORS);

    shaderController.setInterpolationDistance(Preferences::DISTANCE_DELTA);

    for (size_t i = 0; i < static_cast<size_t>(data.size()); i++) {
        auto &locus = data[i];

        locus.startWork();
        if (locus.size() == 0) {
            continue;
        }

        int curAmount = std::min(locus.size(), time);
        size_t start = std::max(0, curAmount - static_cast<int>(Preferences::AMOUNT_TAIL_POINTS));
        size_t actualLength = curAmount - start;

        shaderController.setVertex();

        shaderController.setStartTailSize(Preferences::START_POINT_SIZE);
        shaderController.setFinalTailSize(Preferences::FINAL_POINT_SIZE);
        shaderController.setTailLength(actualLength);
        shaderController.setStartVertexIndex(start);
        shaderController.setTrajectoryIndex(i);

        glDrawArrays(GL_LINE_STRIP_ADJACENCY, start, actualLength);

        locus.endWork();
    }
    shaderController.endWork();
}

} //namespace Locus
