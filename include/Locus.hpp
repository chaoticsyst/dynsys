#pragma once

#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector>
#include <QColor>

#include "Preferences.hpp"
#include "ShaderController.hpp"

namespace Locus {

class Locus final {
public:
    Locus(QVector<QVector3D> &&points_);
    Locus() = default;
    ~Locus() = default;

    size_t size() const;

    void startWork();
    void endWork();

private:
    QOpenGLBuffer pointsBuffer;
    QVector<size_t> startIndexes;
};

class LocusController final {
public:
    LocusController();
    ~LocusController() = default;

    LocusController(const LocusController &)            = delete;
    LocusController(LocusController &&)                 = delete;
    LocusController &operator=(const LocusController &) = delete;
    LocusController &operator=(LocusController &&)      = delete;

    void initialize();

    size_t size() const;

    void addLocus(QVector<QVector3D> &&points_);

    void clear();

    void draw(const QMatrix4x4 &projMatrix, size_t time);

    void setPreferences(const Preferences::Preferences *prefs);

private:
    QVector<Locus> data;
    ShaderController::ShaderController shaderController;

    const Preferences::Preferences *prefs;
};

} //namespace Locus
