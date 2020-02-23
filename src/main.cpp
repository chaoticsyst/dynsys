#include <QApplication>
#include <QGLWidget>
#include <iostream>
#include <fstream>

#include "mainwindow.h"
#include "Model.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    set_constants(0.1, 0.1, 14);

    MainWindow w;
    w.currentPoints = generate_points({1, 1, 1}, 200000, 10, 0.001); //It's random values for testing
    w.show();

    return a.exec();
}
