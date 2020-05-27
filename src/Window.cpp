#include <QtWidgets>
#include <thread>
#include <future>
#include <QFileDialog>

#include "DynamicSystemParser/DynamicSystemParser.hpp"
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

    ui->pointsViewer->setPreferences(&prefs);

    sliderTimer = new QTimer(this);
    sliderTimer->setInterval(prefs.controller.sliderTimeInterval);
    connect(sliderTimer, SIGNAL(timeout()), this, SLOT(updateSlider()));

    for (auto &[name, system] : dynamicSystems) {
        ui->modelsComboBox->addItem(name);
    }
}

Window::DynamicSystemWrapper Window::getCustomSystem(const std::array<std::string, 3> &expressions) {
    return DynamicSystemParser::getDynamicSystem<LambdaPushBackAction>("Custom system", expressions);
}

void Window::insertConstants(const std::vector<std::pair<std::string, std::vector<long double>>> &goodParams) {
    ui->constantsComboBox->clear();
    for (auto&[name, params] : goodParams) {
        ui->constantsComboBox->addItem(name.c_str());
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
    if (ui->modelsComboBox->currentText() == "Custom system") {
        dynamicSystems.erase("Custom system");

        const std::string exprX = ui->firstExpr->text().toStdString();
        const std::string exprY = ui->secondExpr->text().toStdString();
        const std::string exprZ = ui->thirdExpr->text().toStdString();

        dynamicSystems.emplace("Custom system", getCustomSystem({exprX, exprY, exprZ}));
    }

    DynamicSystemWrapper &system = dynamicSystems.at(ui->modelsComboBox->currentText());

    ui->pointsViewer->clearAll();

    for (size_t i = 0; i < prefs.visualization.locusNumber; i++) {
        QVector<QVector3D> buffer;
        long double offset = prefs.model.startPointDelta * i;
        std::vector<long double> constants = {
                ui->firstParamValue->value(),
                ui->secondParamValue->value(),
                ui->thirdParamValue->value()};
        auto pushBackVector = DynamicSystemWrapper_n::getPushBackAndNormalizeLambda(buffer, prefs.model.divNormalization);

        system.compute(pushBackVector,
                       Model::Point{prefs.model.startPoint.x + offset,
                                    prefs.model.startPoint.y + offset,
                                    prefs.model.startPoint.z + offset},
                       prefs.model.pointsNumber,
                       prefs.model.deltaTime,
                       constants);

        ui->pointsViewer->addNewLocus(std::move(buffer));
    }

    timeValue = 0;
    ui->progressSlider->setValue(timeValue);
    sliderTimer->start();

    clearFocus();

    ui->progressSlider->setMaximum(std::min(10000, prefs.model.pointsNumber));
}

void Window::slot_model_selection(QString currentModel) {
    DynamicSystemWrapper &system = dynamicSystems.at(ui->modelsComboBox->currentText());
    insertConstants(system.getInterestingConstants());
    bool readOnly = ui->modelsComboBox->currentText() != "Custom system";
    insertExpressions(system.getFormulae(), readOnly);
}

void Window::slot_constants_selection(QString currentConstants) {
    DynamicSystemWrapper &system = dynamicSystems.at(ui->modelsComboBox->currentText());
    auto goodParams = system.getInterestingConstants();
    for (auto&[name, params] : goodParams) {
        if (name.data() == currentConstants) {
            ui->firstParamValue->setValue(params[0]);
            ui->secondParamValue->setValue(params[1]);
            ui->thirdParamValue->setValue(params[2]);
            break;
        }
    }
}

void Window::slot_time_slider(int timeValue_) {
    timeValue = timeValue_;
    ui->pointsViewer->setCurrentTime((prefs.model.pointsNumber / ui->progressSlider->maximum()) * timeValue);
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
        ui->progressSlider->setValue(timeValue += prefs.controller.deltaTimePerStep);
        ui->pointsViewer->setCurrentTime((prefs.model.pointsNumber / ui->progressSlider->maximum()) * timeValue);
    }
    ui->pointsViewer->repaint();
}

void Window::updateVideoRecordingState() {
    static bool enabled = false;

    enabled ^= true;
    if (enabled) {
        bool prevPauseState = pauseState;
        pauseState = true;

        QString videoName = QFileDialog::getSaveFileName(this, tr("Save Video"), "",
                                                        tr("Video (*.avi)"), nullptr,
                                                        QFileDialog::Option::DontUseNativeDialog);
        if (videoName.isEmpty()) {
            pauseState = prevPauseState;
            enabled = false;

            return;
        }

        if (!videoName.endsWith(".avi")) {
            videoName += ".avi";
        }
        ui->pointsViewer->startVideoRecording(videoName.toStdString().c_str());

        ui->videoRecordingButton->setText("Закончить запись");
        ui->videoRecordingProgress->setEnabled(true);

        pauseState = prevPauseState;
    } else {
        ui->videoRecordingButton->setEnabled(false);

        auto changeProgress = [&bar = ui->videoRecordingProgress](int progress) {
            bar->setValue(progress);
        };

        ui->pointsViewer->endVideoRecording(changeProgress);

        ui->videoRecordingProgress->setValue(0);
        ui->videoRecordingProgress->setEnabled(false);

        ui->videoRecordingButton->setText("Начать запись");
        ui->videoRecordingButton->setEnabled(true);
    }
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
        ui->pointsViewer->keyPressEvent(event);
    }
}

void Window::keyReleaseEvent(QKeyEvent *event) {
    ui->pointsViewer->keyReleaseEvent(event);
}
