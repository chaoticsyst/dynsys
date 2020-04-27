#pragma once

#include <QSize>
#include <QVector3D>
#include <QVector4D>
#include <qgl.h>
#include <cmath>

#include "Model.hpp"

struct Preferences {
    Preferences() = delete;

    static void setDefaultValues();

    static void setValuesBeautifulLorenz();

    static void enableArcadeMode();
    static void disableArcadeMode();

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

    static bool TAILS_VIEW;


/* Locus constants */

    static size_t AMOUNT_TAIL_POINTS;
    static float  START_POINT_DELTA;
    static float  DISTANCE_DELTA;

    static bool  ARCADE_MODE_ON;
    static float START_POINT_SIZE;
    static float FINAL_POINT_SIZE;

    static GLenum PRIMITIVE;

    static bool  TAIL_COLORING_MODE;
    static QVector<QVector4D> COLORS;


/* Camera constants */

    static float EPS;

    static float VERTICAL_ANGLE;
    static float NEAR_PLANE;
    static float FAR_PLANE;

    static float SPEED_MOVE;
    static float SENSITIVITY;

    static float MAX_PITCH;

    static int   CAMERA_TIMER_DELTA;

    static QVector3D INIT_CAMERA_POSITION;
    static QVector3D INIT_CAMERA_TARGET;
    static float     INIT_PITCH;
    static float     INIT_YAW;

/* Video recording constants */
    static int VIDEO_WIDTH;
    static int VIDEO_HEIGHT;

    static int VIDEO_QUALITY;
};
