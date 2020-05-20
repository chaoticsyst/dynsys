#include <QtWidgets>
#include <thread>
#include <future>

#include "Parser.hpp"
#include "DynamicSystems/DynamicSystem.hpp"
#include "Window.hpp"
#include "ui_form.h"
#include "PointsViewQGLWidget.hpp"
#include "WindowPreferences.hpp"

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    auto dynamicSystemsVector = DynamicSystems::getDefaultSystems<LambdaPushBackAction>();
    dynamicSystemsVector.push_back(getCustomSystem({"1", "1", "1"}));
    for (auto &system : dynamicSystemsVector) {
        if (system.constantsCount() <= 3) {
            QString name = system.getAttractorName().data();
            dynamicSystems.emplace(std::move(name), std::move(system));
        }
    }

    windowPreferences = nullptr;

    setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);

    ui->pointsViewQGLWidget->setPreferences(&prefs);

    sliderTimer = new QTimer(this);
    sliderTimer->setInterval(prefs.controller.sliderTimeInterval);
    connect(sliderTimer, SIGNAL(timeout()), this, SLOT(updateSlider()));

    for (auto &[name, system] : dynamicSystems) {
        ui->comboBox->addItem(name);
    }
}

Window::DynamicSystemWrapper Window::getCustomSystem(std::array<std::string, 3> expressions) {
    auto derivativesFunction = Parser::parseExpressions(expressions[0], expressions[1], expressions[2]);
    return DynamicSystemWrapper{"Custom system", std::move(expressions), {}, {},
                                DynamicSystems::DynamicSystemInternal<LambdaPushBackAction, decltype(derivativesFunction)>{
                                        [derivativesFunction = std::move(derivativesFunction)]
                                                (std::vector<long double> &) { return derivativesFunction; }}};
}

void Window::insertConstants(const std::vector<std::pair<std::string, std::vector<long double>>> &goodParams) {
    ui->constantsBox->clear();
    for (auto&[name, params] : goodParams) {
        ui->constantsBox->addItem(name.c_str());
    }
}

void Window::insertExpressions(std::array<std::string_view, 3> array, bool readOnly) {
    ui->firstExpr->setText(array[0].data());
    ui->firstExpr->setReadOnly(readOnly);
    ui->secondExpr->setText(array[1].data());
    ui->secondExpr->setReadOnly(readOnly);
    ui->thirdExpr->setText(array[2].data());
    ui->thirdExpr->setReadOnly(readOnly);
}

void Window::slot_restart_button() {
    if (ui->comboBox->currentText() == "Custom system") {
        dynamicSystems.erase("Custom system");

        const std::string exprX = ui->firstExpr->text().toStdString();
        const std::string exprY = ui->secondExpr->text().toStdString();
        const std::string exprZ = ui->thirdExpr->text().toStdString();

        dynamicSystems.emplace("Custom system", getCustomSystem({exprX, exprY, exprZ}));
    }

    DynamicSystemWrapper &system = dynamicSystems.at(ui->comboBox->currentText());

    ui->pointsViewQGLWidget->clearAll();

    for (size_t i = 0; i < prefs.visualization.locusNumber; i++) {
        QVector<QVector3D> buffer;
        long double offset = prefs.model.startPointDelta * i;
        std::vector<long double> constants = {
                ui->doubleSpinBox->value(),
                ui->doubleSpinBox_2->value(),
                ui->doubleSpinBox_3->value()};
        auto pushBackVector = DynamicSystemWrapper_n::getPushBackAndNormalizeLambda(buffer, prefs.model.divNormalization);

        system.compute(pushBackVector,
                       Model::Point{prefs.model.startPoint.x + offset,
                                    prefs.model.startPoint.y + offset,
                                    prefs.model.startPoint.z + offset},
                       prefs.model.pointsNumber,
                       prefs.model.deltaTime,
                       constants);

        ui->pointsViewQGLWidget->addNewLocus(std::move(buffer));
    }

    timeValue = 0;
    ui->horizontalSlider->setValue(timeValue);
    sliderTimer->start();

    clearFocus();
}

void Window::slot_model_selection(QString currentModel) {
    DynamicSystemWrapper &system = dynamicSystems.at(ui->comboBox->currentText());
    insertConstants(system.getInterestingConstants());
    bool readOnly = ui->comboBox->currentText() != "Custom system";
    insertExpressions(system.getFormulae(), readOnly);
}

void Window::slot_constants_selection(QString currentConstants) {
    DynamicSystemWrapper &system = dynamicSystems.at(ui->comboBox->currentText());
    auto goodParams = system.getInterestingConstants();
    for (auto&[name, params] : goodParams) {
        if (name.data() == currentConstants) {
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
        ui->pauseButton->setText("Pause");
    } else {
        ui->pauseButton->setText("Continue");
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
