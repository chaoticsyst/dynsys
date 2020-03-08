#pragma once

#include <QSize>
#include <cmath>

#include "Model.h"

namespace Preferences {
/* Controller constants */

    // Timer constants
    static int SLIDER_TIMER_INTERVAL = 1;
    static int DELTA_TIME_TIMER = 3;

    // Model constants
    static Model::Point START_POINT = {1, 1, 1};
    static int COUNT_POINTS = 20'000;
    static int STEPS_PER_COUNT = 1;
    static double TAU = 0.02;
    static int DIV_NORMALIZE = 8;

    // View constants
    static size_t AMOUNT_LOCUS = 500;


/* PointsViewQGLWidget constants */

    // Window size constants
    static QSize MIN_WINDOW_SIZE = QSize(640, 480);
    static QSize INIT_WINDOW_SIZE = QSize(1080, 720);

    static long double COLOR_FUNCTION_DELTA = 0.01;


/* Model constants */

    static long double COORDINATE_VALUE_LIMIT = 1000; //generation stops if the coordinate value becomes greater


/* Locus constants */

    static size_t AMOUNT_TAIL_POINTS = 100;


/* Camera constants */

    static float eps = 0.001;

    static float VERTICAL_ANGLE = 60.0;
    static float NEAR_PLANE = 0.001;
    static float FAR_PLANE = 1000;

    static float speedMove = 0.08;
    static float speedRotation = 0.01;

    static float MAX_PITCH = M_PI / 2 - eps;

    static int DELTA_TIME = 1;

}
