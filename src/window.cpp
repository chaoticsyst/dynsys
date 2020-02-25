#include <QtWidgets>

#include "Model.h"
#include "window.h"
#include "ui_form.h"

#include "visual_opengl_widget.h"

// Model constants
const Point START_POINT = {1, 1, 1};
const int COUNT_POINTS = 200'000;
const int STEPS_PER_COUNT = 10;
const double TAU = 0.001;

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    ui->setupUi(this);
}

void Window::slot_set_constant_a(double a) {
    set_constant_a(a);
}

void Window::slot_set_constant_b(double b) {
    set_constant_b(b);
}

void Window::slot_set_constant_c(double c) {
    set_constant_c(c);
}

void Window::slot_restart_button() {
    ui->visualOpenGLWidget->clearPoints();
    ui->visualOpenGLWidget->setPoints(generate_points(START_POINT, COUNT_POINTS, STEPS_PER_COUNT, TAU));
}

Window::~Window() {
    delete ui;
}

void Window::ketPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(event);
}

