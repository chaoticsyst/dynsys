#include "Preferences.h"

namespace Preferences {

void Preferences::enableArcadeMode() {
    visualization.primitive = GL_POINTS;
    visualization.arcadeMode = true;
}

void Preferences::disableArcadeMode() {
    visualization.primitive = GL_LINE_STRIP;
    visualization.arcadeMode = false;
}

} //namespace Preferences
