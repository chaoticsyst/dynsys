#include <QtWidgets>

#include "Model.h"
#include "Window.h"
#include "ui_form.h"

#include "PointsViewQGLWidget.h"

// Timer constants
constexpr int SLIDER_TIMER_INTERVAL = 1;
constexpr int DELTA_TIME = 3;

// Model constants
constexpr Model::Point START_POINT = {1, 1, 1};
constexpr int COUNT_POINTS = 20'000;
constexpr int STEPS_PER_COUNT = 1;
constexpr double TAU = 0.02;
constexpr int DIV_NORMALIZE = 8;

// View constants
constexpr size_t AMOUNT_LOCUS = 500;

std::vector<std::pair<QString, std::vector<double>>> goodParamsRossler = {
    {"Классические значения", {0.2, 0.2, 5.7}},
    {"Устойчивый предельный цикл", {0.2, 0.2, 3}},
    {"Популярные значения", {0.1, 0.1, 14}},
    {"Расхождение аттрактора", {0.3, 0.1, 14}}
};

std::vector<std::pair<QString, std::vector<double>>> goodParamsLorenz = {
    {"Классические значения", {10, 28, 8.0 / 3.0,}},
    {"Схождение в цикл", {10, 100, 8.0 / 3.0}}
};

void Window::insertConstants(std::vector<std::pair<QString, std::vector<double>>>& goodParams) {
    ui->constantsBox->clear();
    for (auto& [name, params] : goodParams) {
        ui->constantsBox->addItem(name);
    }
}

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    sliderTimer = new QTimer(this);
    sliderTimer->setInterval(SLIDER_TIMER_INTERVAL);

    connect(sliderTimer, SIGNAL(timeout()), this, SLOT(updateSlider()));

    insertConstants(goodParamsRossler);
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
    for (size_t i = 0; i < AMOUNT_LOCUS; i++) {
        QVector<QVector3D> buffer;
        auto pushBackVector = [&buffer](const Model::Point &point) {
            buffer.push_back(
                QVector3D(point.x / DIV_NORMALIZE,
                          point.y / DIV_NORMALIZE,
                          point.z / DIV_NORMALIZE)
            );
        };
        double offset = 0.001 * i;
        Model::generate_points(pushBackVector,
                               Model::Point{START_POINT.x + offset,
                                            START_POINT.y + offset,
                                            START_POINT.z + offset},
                               COUNT_POINTS,
                               STEPS_PER_COUNT,
                               TAU,
                               modelName,
                               constants);
        ui->pointsViewQGLWidget->addNewLocus(buffer);
    }

    timeValue = 0;
    ui->horizontalSlider->setValue(timeValue);
    sliderTimer->start();
}

void Window::slot_model_selection(QString currentModel) {
    if (currentModel == "Аттрактор Рёсслера") {
        insertConstants(goodParamsRossler);
    } else if (currentModel == "Аттрактор Лоренца") {
        insertConstants(goodParamsLorenz);
    }
}

void Window::slot_constants_selection(QString currentConstants) {
    std::vector<std::pair<QString, std::vector<double>>> goodParams;
    if (ui->comboBox->currentText() == "Аттрактор Рёсслера") {
        goodParams = goodParamsRossler;
    } else if (ui->comboBox->currentText() == "Аттрактор Лоренца") {
        goodParams = goodParamsLorenz;
    }
    for (auto& [name, params] : goodParams) {
        if (name == currentConstants) {
            ui->doubleSpinBox->setValue(params[0]);
            ui->doubleSpinBox_2->setValue(params[1]);
            ui->doubleSpinBox_3->setValue(params[2]);
            break;
        }
    }
}

void Window::slot_time_slider(int timeValue_) {
    timeValue = timeValue_;
    ui->pointsViewQGLWidget->setCurrentTime((COUNT_POINTS / ui->horizontalSlider->maximum()) * timeValue);
}


void Window::updateSlider() {
    ui->horizontalSlider->setValue(timeValue += DELTA_TIME);
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

