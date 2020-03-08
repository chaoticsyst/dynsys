#pragma once

#include <QSize>
#include <cmath>

#include "Model.h"

struct Preferences {
    Preferences() = delete;

    static void setDefaultValues() {
        SLIDER_TIMER_INTERVAL = 1;
        DELTA_TIME_TIMER = 3;
        START_POINT      = {1, 1, 1};
        COUNT_POINTS     = 20'000;
        STEPS_PER_COUNT  = 1;
        TAU              = 0.01;
        DIV_NORMALIZE    = 8;

        AMOUNT_LOCUS = 500;

        NEW_PREFERENCES = false;

        MIN_WINDOW_SIZE  = QSize(640, 480);
        INIT_WINDOW_SIZE = QSize(1080, 720);

        COLOR_FUNCTION_DELTA = 0.01;

        TAILS_VIEW = true;

        COORDINATE_VALUE_LIMIT = 1000;

        AMOUNT_TAIL_POINTS = 100;

        eps = 0.001;

        VERTICAL_ANGLE = 60.0;
        NEAR_PLANE     = 0.001;
        FAR_PLANE      = 1000;

        speedMove     = 0.08;
        speedRotation = 0.01;

        MAX_PITCH = M_PI / 2 - eps;

        DELTA_TIME = 1;
    }


    static void setValuesBeautifulLorenz() {
        setDefaultValues();
        COUNT_POINTS     = 10'000;
        TAU              = 0.003;

        AMOUNT_LOCUS = 500;

        AMOUNT_TAIL_POINTS = 150;
    }

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


/* Model constants */

    static long double COORDINATE_VALUE_LIMIT; //generation stops if the coordinate value becomes greater


/* Locus constants */

    static size_t AMOUNT_TAIL_POINTS;


/* Camera constants */

    static float eps;

    static float VERTICAL_ANGLE;
    static float NEAR_PLANE;
    static float FAR_PLANE;

    static float speedMove;
    static float speedRotation;

    static float MAX_PITCH;

    static int DELTA_TIME;
};
