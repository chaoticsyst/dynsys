#include <QtWidgets>
#include <thread>
#include <future>

#include "Model.hpp"
#include "Parser.hpp"
#include "DynamicSystemsDefault.hpp"
#include "AttractorsParams.h"
#include "Window.h"
#include "ui_form.h"
#include "PointsViewQGLWidget.h"
#include "WindowPreferences.h"

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    windowPreferences = nullptr;

    setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);

    ui->pointsViewQGLWidget->setPreferences(&prefs);

    sliderTimer = new QTimer(this);
    sliderTimer->setInterval(prefs.controller.sliderTimeInterval);
    connect(sliderTimer, SIGNAL(timeout()), this, SLOT(updateSlider()));

    ui->comboBox->addItem("Свои уравнения");
    ui->comboBox->addItem("Аттрактор Рёсслера");
    ui->comboBox->addItem("Аттрактор Лоренца");
}

void Window::insertConstants(QVector<std::pair<QString, QVector<double>>> &goodParams) {
    ui->constantsBox->clear();
    for (auto&[name, params] : goodParams) {
        ui->constantsBox->addItem(name);
    }
}

void Window::slot_restart_button() {
    if (ui->comboBox->currentText() == "Свои уравнения") {
        const std::string exprX = ui->firstExpr->text().toStdString();
        const std::string exprY = ui->secondExpr->text().toStdString();
        const std::string exprZ = ui->thirdExpr->text().toStdString();
        auto derivatives_function = Parser::parseExpressions(exprX, exprY, exprZ);
        count_points(derivatives_function);
    } else if (ui->comboBox->currentText() == "Аттрактор Лоренца") {
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
    for (size_t i = 0; i < prefs.visualization.locusNumber; i++) {
        QVector<QVector3D> buffer;
        auto pushBackVector = [&buffer, &divNorm = prefs.model.divNormalization](const Model::Point &point) {
            buffer.push_back(
                    QVector3D(point.x / divNorm,
                              point.y / divNorm,
                              point.z / divNorm)
            );
        };
        double offset = prefs.model.startPointDelta * i;

        Model::generate_points(pushBackVector,
                               Model::Point{prefs.model.startPoint.x + offset,
                                            prefs.model.startPoint.y + offset,
                                            prefs.model.startPoint.z + offset},
                               prefs.model.pointsNumber,
                               1,
                               prefs.model.deltaTime,
                               derivatives_function);

        ui->pointsViewQGLWidget->addNewLocus(std::move(buffer));
    }

    timeValue = 0;
    ui->horizontalSlider->setValue(timeValue);
    sliderTimer->start();

    clearFocus();
}

void Window::slot_model_selection(QString currentModel) {
    if (currentModel == "Свои уравнения") {
        insertConstants(AttractorsParams::customParams);
    } else if (currentModel == "Аттрактор Рёсслера") {
        insertConstants(AttractorsParams::goodParamsRossler);
    } else if (currentModel == "Аттрактор Лоренца") {
        insertConstants(AttractorsParams::goodParamsLorenz);
    }
}

void Window::slot_constants_selection(QString currentConstants) {
    QVector<std::pair<QString, QVector<double>>> goodParams;
    if (ui->comboBox->currentText() == "Свои уравнения") {
        goodParams = AttractorsParams::customParams;
    } else if (ui->comboBox->currentText() == "Аттрактор Рёсслера") {
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
    ui->pointsViewQGLWidget->setCurrentTime((prefs.model.pointsNumber / ui->horizontalSlider->maximum()) * timeValue);
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
    windowPreferences = new WindowPreferences(this, &prefs);
    windowPreferences->show();
}

void Window::updateSlider() {
    if (!pauseState && timeValue <= prefs.model.pointsNumber) {
        ui->horizontalSlider->setValue(timeValue += prefs.controller.deltaTimePerStep);
        ui->pointsViewQGLWidget->setCurrentTime((prefs.model.pointsNumber / ui->horizontalSlider->maximum()) * timeValue);
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
