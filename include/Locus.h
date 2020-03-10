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

    void startWork();
    void endWork();

private:
    QOpenGLBuffer pointsBuffer;
    QColor color;

    static QVector3D getInterpolatedPoint(float offset, const QVector<QVector3D> &points, size_t startIndex);

    static QVector<QVector3D> interpolate(const QVector<QVector3D> &points);
};

class LocusController final {
public:
    LocusController()  = default;
    ~LocusController() = default;

    LocusController(const LocusController &)            = delete;
    LocusController(LocusController &&)                 = delete;
    LocusController &operator=(const LocusController &) = delete;
    LocusController &operator=(LocusController &&)      = delete;

    size_t size() const;

    void addLocus(Locus &&locus);
    void removeLocus(size_t index);

    void clear();

    void draw(QGLShaderProgram &shaderProgram, size_t amount);
private:
    QVector<Locus> data;
};

} //namespace Locus
