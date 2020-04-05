#pragma once

#include <QSize>
#include <cmath>

#include "Model.hpp"

struct Preferences {
    Preferences() = delete;

    static void setDefaultValues();

    static void setValuesBeautifulLorenz();

/* Controller constants */

    // Timer constants
    static int SLIDER_TIMER_INTERVAL;
    static int DELTA_TIME_TIMER;

    // Model constants
    static Model::Point START_POINT;
    static int COUNT_POINTS;
    static int STEPS_PER_COUNT;
    static double TAU;
    static int DIV_NORMALIZE;

    // View constants
    static size_t AMOUNT_LOCUS;

    static bool NEW_PREFERENCES;

/* PointsViewQGLWidget constants */

    // Window size constants
    static QSize MIN_WINDOW_SIZE;
    static QSize INIT_WINDOW_SIZE;

    static long double COLOR_FUNCTION_DELTA;

    static bool TAILS_VIEW;


/* Locus constants */

    static size_t AMOUNT_TAIL_POINTS;
    static float  START_POINT_DELTA;
    static float  DISTANCE_DELTA;


/* Camera constants */

    static float EPS;

    static float VERTICAL_ANGLE;
    static float NEAR_PLANE;
    static float FAR_PLANE;

    static float SPEED_MOVE;
    static float SENSITIVITY;

    static float MAX_PITCH;

    static int   CAMERA_TIMER_DELTA;

/* Shaders */

    static const char *VERTEX_SHADER;
    static const char *FRAGMENT_SHADER;
    static const char *VERTEX_NAME;
    static const char *COLOR_NAME;
    static const char *MATRIX_NAME;
};
