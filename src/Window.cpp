#include <QtWidgets>
#include <thread>
#include <future>

#include "Model.hpp"
#include "DynamicSystemsDefault.hpp"
#include "AttractorsParams.h"
#include "Window.h"
#include "ui_form.h"
#include "PointsViewQGLWidget.h"
#include "Preferences.h"
#include "WindowPreferences.h"

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    Preferences::setDefaultValues();

    windowPreferences = nullptr;

    setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);

    sliderTimer = new QTimer(this);
    sliderTimer->setInterval(Preferences::SLIDER_TIMER_INTERVAL);
    connect(sliderTimer, SIGNAL(timeout()), this, SLOT(updateSlider()));

    insertConstants(AttractorsParams::goodParamsRossler);
}

void Window::insertConstants(QVector<std::pair<QString, QVector<double>>> &goodParams) {
    ui->constantsBox->clear();
    for (auto&[name, params] : goodParams) {
        ui->constantsBox->addItem(name);
    }
}

QVector3D getQPoint(const Model::Point &point) {
    return QVector3D(point.x / Preferences::DIV_NORMALIZE,
                     point.y / Preferences::DIV_NORMALIZE,
                     point.z / Preferences::DIV_NORMALIZE); //TODO: implement normalization
}

void Window::slot_restart_button() {
    if (ui->comboBox->currentText() == "Аттрактор Лоренца") {
        auto derivatives_function = Model::get_derivatives_function_lorenz(
                ui->doubleSpinBox->value(),
                ui->doubleSpinBox_2->value(),
                ui->doubleSpinBox_3->value());
        count_points(derivatives_function);
    } else if (ui->comboBox->currentText() == "Аттрактор Рёсслера") {
        auto derivatives_function = Model::get_derivatives_function_rossler(
                ui->doubleSpinBox->value(),
                ui->doubleSpinBox_2->value(),
                ui->doubleSpinBox_3->value());
        count_points(derivatives_function);
    }
}

template<typename Lambda>
void Window::count_points(Lambda derivatives_function) {
    ui->pointsViewQGLWidget->clearAll();
    for (size_t i = 0; i < Preferences::AMOUNT_LOCUS; i++) {
        QVector<QVector3D> buffer;
        auto pushBackVector = [&buffer](const Model::Point &point) {
            buffer.push_back(
                    QVector3D(point.x / Preferences::DIV_NORMALIZE,
                              point.y / Preferences::DIV_NORMALIZE,
                              point.z / Preferences::DIV_NORMALIZE)
            );
        };
        double offset = Preferences::START_POINT_DELTA * i;

        Model::generate_points(pushBackVector,
                               Model::Point{Preferences::START_POINT.x + offset,
                                            Preferences::START_POINT.y + offset,
                                            Preferences::START_POINT.z + offset},
                               Preferences::COUNT_POINTS,
                               Preferences::STEPS_PER_COUNT,
                               Preferences::TAU,
                               derivatives_function);

        ui->pointsViewQGLWidget->addNewLocus(std::move(buffer));
    }

    timeValue = 0;
    ui->horizontalSlider->setValue(timeValue);
    sliderTimer->start();

    clearFocus();
}

void Window::slot_model_selection(QString currentModel) {
    if (currentModel == "Аттрактор Рёсслера") {
        insertConstants(AttractorsParams::goodParamsRossler);
    } else if (currentModel == "Аттрактор Лоренца") {
        insertConstants(AttractorsParams::goodParamsLorenz);
    }
}

void Window::slot_constants_selection(QString currentConstants) {
    QVector<std::pair<QString, QVector<double>>> goodParams;
    if (ui->comboBox->currentText() == "Аттрактор Рёсслера") {
        goodParams = AttractorsParams::goodParamsRossler;
    } else if (ui->comboBox->currentText() == "Аттрактор Лоренца") {
        goodParams = AttractorsParams::goodParamsLorenz;
    }
    for (auto&[name, params] : goodParams) {
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
    ui->pointsViewQGLWidget->setCurrentTime((Preferences::COUNT_POINTS / ui->horizontalSlider->maximum()) * timeValue);
}

void Window::slot_pause_button() {
    pauseState ^= true;
    if (!pauseState) {
        ui->pauseButton->setText("Пауза");
    } else {
        ui->pauseButton->setText("Продолжить");
    }
}

void Window::slot_open_preferences() {
    windowPreferences = new WindowPreferences();
    windowPreferences->show();
}

void Window::updateSlider() {
    if (!pauseState && timeValue <= Preferences::COUNT_POINTS) {
        ui->horizontalSlider->setValue(timeValue += Preferences::DELTA_TIME_TIMER);
        ui->pointsViewQGLWidget->setCurrentTime((Preferences::COUNT_POINTS / ui->horizontalSlider->maximum()) * timeValue);
    }
    ui->pointsViewQGLWidget->repaint();
}

Window::~Window() {
    delete windowPreferences;
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        slot_pause_button();
    } else if (event->key() == Qt::Key_Enter) {
        slot_restart_button();
    } else {
        ui->pointsViewQGLWidget->keyPressEvent(event);
    }
}

void Window::keyReleaseEvent(QKeyEvent *event) {
    ui->pointsViewQGLWidget->keyReleaseEvent(event);
}
