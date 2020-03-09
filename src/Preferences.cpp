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
QSize        Preferences::INIT_WINDOW_SIZE       = QSize(0,0);
long double  Preferences::COLOR_FUNCTION_DELTA   = 0.0l;
bool         Preferences::TAILS_VIEW             = false;
long double  Preferences::COORDINATE_VALUE_LIMIT = 0.0l;
size_t       Preferences::AMOUNT_TAIL_POINTS     = 0ul;
float        Preferences::DISTANCE_DELTA         = 0;
float        Preferences::eps                    = 0.0f;
float        Preferences::VERTICAL_ANGLE         = 0.0f;
float        Preferences::NEAR_PLANE             = 0.0f;
float        Preferences::FAR_PLANE              = 0.0f;
float        Preferences::speedMove              = 0.0f;
float        Preferences::speedRotation          = 0.0f;
float        Preferences::MAX_PITCH              = 0.0f;
int          Preferences::DELTA_TIME             = 0;
