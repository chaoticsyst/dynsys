#include "WindowPreferences.hpp"
#include "ui_formPreferences.h"

WindowPreferences::WindowPreferences(QWidget *parent, Preferences::Preferences *prefs_) :
    QWidget(parent), ui(new Ui::WindowPreferences), prefs(prefs_) {

    ui->setupUi(this);
    setCurrentStateInUI();
}

void WindowPreferences::setCurrentStateInUI() {
/* Model settings */
    ui->trajNumberValue->setValue(prefs->visualization.locusNumber);
    ui->pointsNumberValue->setValue(prefs->model.pointsNumber);
    ui->startPointsDeltaValue->setValue(prefs->model.startPointDelta);
    ui->deltaTimeValue->setValue(prefs->model.deltaTime);
    ui->xCoordValue->setValue(prefs->model.startPoint.x);
    ui->yCoordValue->setValue(prefs->model.startPoint.y);
    ui->zCoordValue->setValue(prefs->model.startPoint.z);

/* Camera settings */
    ui->sensitivitySlider->setValue((prefs->camera.sensitivity - 0.0005) / (0.03 - 0.0005) * 100);
    ui->speedMoveSlider->setValue((prefs->camera.speed - 0.05) / (0.3 - 0.05) * 100);

/* View settings */
    ui->tailPointsNumberValue->setValue(prefs->visualization.tailPointsNumber);
    ui->timerSpeedValue->setValue(prefs->controller.deltaTimePerStep);
    ui->interpolationDegree->setValue((0.21 - prefs->visualization.interpolationDistance) / 0.002);

}

void WindowPreferences::setStateFromUI() {
/* Model settings */
    prefs->visualization.locusNumber = ui->trajNumberValue->value();
    prefs->model.pointsNumber        = ui->pointsNumberValue->value();
    prefs->model.startPointDelta     = ui->startPointsDeltaValue->value();
    prefs->model.deltaTime           = ui->deltaTimeValue->value();
    prefs->model.startPoint.x        = ui->xCoordValue->value();
    prefs->model.startPoint.y        = ui->yCoordValue->value();
    prefs->model.startPoint.z        = ui->zCoordValue->value();

/* Camera settings */
    prefs->camera.speed       = 0.05 + ui->speedMoveSlider->value() / 100.0 * (0.3 - 0.05);
    prefs->camera.sensitivity = 0.0005 + ui->sensitivitySlider->value() / 100.0 * (0.03 - 0.0005);

/* View settings */
    prefs->visualization.tailPointsNumber      = ui->tailPointsNumberValue->value();
    prefs->controller.deltaTimePerStep         = ui->timerSpeedValue->value();
    prefs->visualization.interpolationDistance = 0.21 - ui->interpolationDegree->value() * 0.002;

    prefs->controller.preferencesChanged = true;
}

void WindowPreferences::slot_cancel_button() {
    setCurrentStateInUI();
    this->hide();
}

void WindowPreferences::slot_apply_button() {
    setStateFromUI();
    this->hide();
}

void WindowPreferences::slot_set_default_button() {
    *prefs = Preferences::defaultPreferences;
    setCurrentStateInUI();
    this->hide();
}

WindowPreferences::~WindowPreferences() {
    delete ui;
}
