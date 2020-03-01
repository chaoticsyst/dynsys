#include <QApplication>
#include <QGLWidget>
#include <iostream>
#include <fstream>

#include "window.h"
#include "Model.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Window w;

    w.resize(w.sizeHint());

    w.show();

    return app.exec();
}
