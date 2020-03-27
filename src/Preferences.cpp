#include "Preferences.h"

// Sorry...
int          Preferences::SLIDER_TIMER_INTERVAL  = 0;
int          Preferences::DELTA_TIME_TIMER       = 0;
Model::Point Preferences::START_POINT            = (Model::Point){0, 0, 0};
int          Preferences::COUNT_POINTS           = 0;
int          Preferences::STEPS_PER_COUNT        = 0;
double       Preferences::TAU                    = 0.0;
int          Preferences::DIV_NORMALIZE          = 0;
size_t       Preferences::AMOUNT_LOCUS           = 0ul;
bool         Preferences::NEW_PREFERENCES        = false;
QSize        Preferences::MIN_WINDOW_SIZE        = QSize(0, 0);
QSize        Preferences::INIT_WINDOW_SIZE       = QSize(0, 0);
long double  Preferences::COLOR_FUNCTION_DELTA   = 0.0l;
bool         Preferences::TAILS_VIEW             = false;
long double  Preferences::COORDINATE_VALUE_LIMIT = 0.0l;
size_t       Preferences::AMOUNT_TAIL_POINTS     = 0ul;
float        Preferences::START_POINT_DELTA      = 0.0f;
float        Preferences::DISTANCE_DELTA         = 0;
float        Preferences::EPS                    = 0.0f;
float        Preferences::VERTICAL_ANGLE         = 0.0f;
float        Preferences::NEAR_PLANE             = 0.0f;
float        Preferences::FAR_PLANE              = 0.0f;
float        Preferences::SPEED_MOVE             = 0.0f;
float        Preferences::SENSITIVITY            = 0.0f;
float        Preferences::MAX_PITCH              = 0.0f;
int          Preferences::CAMERA_TIMER_DELTA     = 0;
QVector3D    Preferences::INIT_CAMERA_POSITION   = QVector3D();
QVector3D    Preferences::INIT_CAMERA_TARGET     = QVector3D();
float        Preferences::INIT_PITCH             = 0.0f;
float        Preferences::INIT_YAW               = 0.0f;
const char  *Preferences::VERTEX_NAME            = "vertex";
const char  *Preferences::COLOR_NAME             = "color";
const char  *Preferences::MATRIX_NAME            = "matrix";


void Preferences::setDefaultValues() {
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
    START_POINT_DELTA = 0.001;
    DISTANCE_DELTA = 0.2;

    EPS = 0.001;

    VERTICAL_ANGLE = 60.0;
    NEAR_PLANE     = 0.001;
    FAR_PLANE      = 1000;

    SPEED_MOVE     = 0.08;
    SENSITIVITY = 0.01;

    MAX_PITCH = M_PI / 2 - EPS;

    CAMERA_TIMER_DELTA = 1;

    INIT_CAMERA_POSITION = QVector3D(0, 0, 5);
    INIT_CAMERA_TARGET   = QVector3D(-INIT_CAMERA_POSITION);
    INIT_YAW              = -M_PI / 2;
}

void Preferences::setValuesBeautifulLorenz() {
    setDefaultValues();

    COUNT_POINTS     = 10'000;
    TAU              = 0.003;

    AMOUNT_LOCUS = 500;

    AMOUNT_TAIL_POINTS = 150;
}
