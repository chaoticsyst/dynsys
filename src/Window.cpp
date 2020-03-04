#include <QtWidgets>

#include "Model.h"
#include "Window.h"
#include "ui_form.h"

#include "PointsViewQGLWidget.h"

// Timer constants
constexpr int SLIDER_TIMER_INTERVAL = 10;

// Model constants
constexpr Model::Point START_POINT = {1, 1, 1};
constexpr int COUNT_POINTS = 200'000;
constexpr int STEPS_PER_COUNT = 10;
constexpr double TAU = 0.001;
constexpr int DIV_NORMALIZE = 8;

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    sliderTimer = new QTimer(this);
    sliderTimer->setInterval(SLIDER_TIMER_INTERVAL);

    connect(sliderTimer, SIGNAL(timeout()), this, SLOT(updateSlider()));

}

QVector3D getQPoint(const Model::Point &point) {
    return QVector3D(point.x / DIV_NORMALIZE, point.y / DIV_NORMALIZE, point.z / DIV_NORMALIZE); //TODO: implement normalization
}

void Window::slot_restart_button() {
    Model::ModelName modelName = Model::ModelName::ROSSLER;
    if (ui->comboBox->currentText() == "Аттрактор Лоренца") {
        modelName = Model::ModelName::LORENZ;
    } else if (ui->comboBox->currentText() == "Аттрактор Рёсслера") {
        modelName = Model::ModelName::ROSSLER;
    }

    std::vector<long double> constants = {
        ui->doubleSpinBox->value(),
        ui->doubleSpinBox_2->value(),
        ui->doubleSpinBox_3->value()
    };

    ui->pointsViewQGLWidget->clearAll();
    QVector<QVector3D> buffer;
    auto pushBackVector = [&buffer](const Model::Point &point) {
        buffer.push_back(
            QVector3D(point.x / DIV_NORMALIZE,
                      point.y / DIV_NORMALIZE,
                      point.z / DIV_NORMALIZE)
        );
    };
    Model::generate_points(pushBackVector,
                           START_POINT,
                           COUNT_POINTS,
                           STEPS_PER_COUNT,
                           TAU,
                           modelName,
                           constants);
    ui->pointsViewQGLWidget->addNewLocus(buffer);

    timeValue = 0;
    ui->horizontalSlider->setValue(timeValue);
    sliderTimer->start();
}

void Window::slot_time_slider(int timeValue_) {
    timeValue = timeValue_;
    ui->pointsViewQGLWidget->setCurrentTime((COUNT_POINTS / ui->horizontalSlider->maximum()) * timeValue);
}

void Window::updateSlider() {
    ui->horizontalSlider->setValue(++timeValue);
    ui->pointsViewQGLWidget->setCurrentTime((COUNT_POINTS / ui->horizontalSlider->maximum()) * timeValue);
    ui->pointsViewQGLWidget->repaint();
}

Window::~Window() {
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        close();
    else {
        ui->pointsViewQGLWidget->keyPressEvent(event);
    }
}

void Window::keyReleaseEvent(QKeyEvent *event) {
    ui->pointsViewQGLWidget->keyReleaseEvent(event);
}

