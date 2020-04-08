#pragma once

#include <QGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector>
#include <QColor>

namespace Locus {

class Locus final {
public:
    Locus(QVector<QVector3D> &&points_, const QColor &color_);
    Locus() = default;
    ~Locus() = default;

    const QColor &colorData() const;

    size_t size() const;
    size_t initialSize() const;
    size_t getStartIndex(size_t initialIndex) const;

    void startWork();
    void endWork();
private:
    QOpenGLBuffer pointsBuffer;
    QColor color;
    QVector<size_t> startIndexes;

    QVector3D getInterpolatedPoint(float offset, const QMatrix4x4 &matrix) const;

    //it has to be called in constructor
    QVector<QVector3D> interpolate(const QVector<QVector3D> &points);
};

class LocusController final {
public:
    LocusController(QGLShaderProgram &shaderProgram_);
    ~LocusController() = default;

    LocusController(const LocusController &)            = delete;
    LocusController(LocusController &&)                 = delete;
    LocusController &operator=(const LocusController &) = delete;
    LocusController &operator=(LocusController &&)      = delete;

    size_t size() const;

    void addLocus(QVector<QVector3D> &&points_, const QColor &color_);

    void clear();

    void draw(size_t length);
private:
    QVector<Locus> data;
    QGLShaderProgram &shaderProgram;
};

} //namespace Locus
