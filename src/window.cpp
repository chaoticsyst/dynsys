#include <QtWidgets>

#include "Model.h"
#include "window.h"
#include "ui_form.h"

#include "visual_opengl_widget.h"

// Model constants
const Model::Point START_POINT = {1, 1, 1};
const int COUNT_POINTS = 200'000;
const int STEPS_PER_COUNT = 10;
const double TAU = 0.001;

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    ui->setupUi(this);
}

void Window::slot_restart_button() {
    std::vector<long double> constants;
    constants.push_back(ui->doubleSpinBox->value());
    constants.push_back(ui->doubleSpinBox_2->value());
    constants.push_back(ui->doubleSpinBox_3->value());
    ui->visualOpenGLWidget->clearPoints();
    ui->visualOpenGLWidget->setPoints(Model::generate_points(START_POINT,
                                                             COUNT_POINTS,
                                                             STEPS_PER_COUNT,
                                                             TAU,
                                                             constants));
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

