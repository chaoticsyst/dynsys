#pragma once

#include <QVector4D>
#include <qgl.h>

#include "Model/Model.hpp"

namespace Preferences {

class Preferences final {
private:
    struct CameraPreferences final {
        float speed = 0.08;
        float sensitivity = 0.01;
    };

    struct VisualizationPreferences final {
        size_t tailPointsNumber = 100;
        size_t locusNumber = 200;

        float interpolationDistance = 0.15;
        float startPointSize = 0;
        float finalPointSize = 10;

        GLenum primitive = GL_LINE_STRIP;

        bool arcadeMode = false;
        bool tailColoringMode = true;

        QVector<QVector4D> colors = { { 0, 1, 1, 1 }, { 0, 0, 1, 1 }, { 1, 0, 0, 1 } };
    };

    struct VideoPreferences final {
        size_t width = 1920;
        size_t height = 1080;
    };

    struct ControllerPreferences final {
        int sliderTimeInterval = 1;
        int deltaTimePerStep = 1;

        bool preferencesChanged = false;
    };

    struct ModelPreferences final {
        Model::Point startPoint = { 1, 1, 1 };
        int pointsNumber = 20000;
        double deltaTime = 0.01;
        float divNormalization = 8;
        float startPointDelta = 0.05;
    };

public:
    VideoPreferences video;
    ModelPreferences model;
    CameraPreferences camera;
    ControllerPreferences controller;
    VisualizationPreferences visualization;

    void enableArcadeMode();
    void disableArcadeMode();
};

inline const Preferences defaultPreferences{};

} //namespace Preferences
