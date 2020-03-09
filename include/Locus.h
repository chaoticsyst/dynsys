#pragma once

#include <QGLShaderProgram>
#include <QVector3D>
#include <QVector>
#include <QColor>

namespace Locus {

class Locus final {
public:
    Locus(QVector<QVector3D> &&points_, const QColor &color_);
    Locus() = default;
    ~Locus() = default;

    const QVector3D *pointsData() const;
    const QColor &colorData() const;

    size_t size() const;

private:
    QVector<QVector3D> points;
    QColor color;

    QVector3D getInterpolatedPoint(float offset, size_t startIndex);

    void interpolate();
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

    void draw(QGLShaderProgram &shaderProgram, size_t amount) const;
private:
    QVector<Locus> data;
};

} //namespace Locus
