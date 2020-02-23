#include <QApplication>
#include <QGLWidget>
#include <iostream>
#include <fstream>

#include "mainwindow.h"
#include "Model.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //set_constants(0.2, 5.7, 0.3); //Не работает, а должно!

    MainWindow w;
    w.currentPoints = generate_points({1, 1, 1}, 100000, 5, 0.45); //It's random values for testing
    w.show();

    return a.exec();
}
