#include "Locus.hpp"

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


LocusController::LocusController() :
    prefs{&Preferences::defaultPreferences} {}

void LocusController::setPreferences(const Preferences::Preferences *prefs_) {
    prefs = prefs_;
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

    shaderController.setArcadeMode(prefs->visualization.arcadeMode);

    shaderController.setTailColoringMode(prefs->visualization.tailColoringMode);
    shaderController.setTrajectoriesNumber(static_cast<size_t>(data.size()));
    shaderController.setColors(prefs->visualization.colors);

    shaderController.setInterpolationDistance(prefs->visualization.interpolationDistance);

    shaderController.setPrimitive(prefs->visualization.primitive);

    for (size_t i = 0; i < static_cast<size_t>(data.size()); i++) {
        auto &locus = data[i];

        locus.startWork();

        if (locus.size() == 0) {
            continue;
        }

        size_t start = std::max(0, static_cast<int>(time) - static_cast<int>(prefs->visualization.tailPointsNumber));
        if (start >= locus.size()) {
            locus.endWork();

            continue;
        }
        size_t actualLength = std::min(time, locus.size()) - start;

        shaderController.setVertex();

        shaderController.setStartTailSize(prefs->visualization.startPointSize);
        shaderController.setFinalTailSize(prefs->visualization.finalPointSize);
        shaderController.setTailLength(actualLength);
        shaderController.setStartVertexIndex(start);
        shaderController.setTrajectoryIndex(i);

        glDrawArrays(GL_LINE_STRIP_ADJACENCY, start, actualLength);

        locus.endWork();
    }
    shaderController.endWork();
}

} //namespace Locus
