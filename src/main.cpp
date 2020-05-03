#include <QApplication>
#include <QGLWidget>

#include "Window.h"


#include "Parser.hpp"
//#include <QDebug>
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << Parser::parse_expression("2 + 1", {nullptr, nullptr, nullptr})->calc() << std::endl;

    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication app(argc, argv);

    Window w;

    w.resize(w.sizeHint());

    w.showMaximized();

    return app.exec();
}
