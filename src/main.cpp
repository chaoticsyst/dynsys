#include <QApplication>
#include <QGLWidget>

#include "Window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Window w;

    w.resize(w.sizeHint());

    w.showMaximized();

    return app.exec();
}
