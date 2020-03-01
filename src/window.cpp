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
    setFocusPolicy(Qt::StrongFocus);
}

void Window::slot_restart_button() {
    Model::ModelName modelName = Model::ModelName::Rossler;
    std::vector<long double> constants;
    constants.push_back(ui->doubleSpinBox->value());
    constants.push_back(ui->doubleSpinBox_2->value());
    constants.push_back(ui->doubleSpinBox_3->value());
    std::vector<Model::Point> points;
    points.reserve(COUNT_POINTS);
    auto pushBackVector = [&points](const Model::Point &point) { points.push_back(point); };
    Model::generate_points(pushBackVector,
                           START_POINT,
                           COUNT_POINTS,
                           STEPS_PER_COUNT,
                           TAU,
                           modelName,
                           constants);
    ui->visualOpenGLWidget->clearPoints();
    ui->visualOpenGLWidget->setPoints(points);
}

Window::~Window() {
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        close();
    else {
        ui->visualOpenGLWidget->keyPressEvent(event);
    }
}

void Window::keyReleaseEvent(QKeyEvent *event) {
    ui->visualOpenGLWidget->keyReleaseEvent(event);
}

