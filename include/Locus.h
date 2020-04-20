#pragma once

#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector>
#include <QColor>
#include "ShaderController.h"

namespace Locus {

class Locus final {
public:
    Locus(QVector<QVector3D> &&points_);
    Locus() = default;
    ~Locus() = default;

    size_t size() const;
    size_t initialSize() const;
    size_t getStartIndex(size_t initialIndex) const;

    void startWork();
    void endWork();
private:
    QOpenGLBuffer pointsBuffer;
    QVector<size_t> startIndexes;

    QVector3D getInterpolatedPoint(float offset, const QMatrix4x4 &matrix) const;

    //it has to be called in constructor
    QVector<QVector3D> interpolate(const QVector<QVector3D> &points);
};

class LocusController final {
public:
    LocusController()  = default;
    ~LocusController() = default;

    LocusController(const LocusController &)            = delete;
    LocusController(LocusController &&)                 = delete;
    LocusController &operator=(const LocusController &) = delete;
    LocusController &operator=(LocusController &&)      = delete;

    void initialize();

    size_t size() const;

    void addLocus(QVector<QVector3D> &&points_);

    void clear();

    void draw(const QMatrix4x4 &projMatrix, size_t length);
private:
    QVector<Locus> data;
    ShaderController::ShaderController shaderController;
};

} //namespace Locus
