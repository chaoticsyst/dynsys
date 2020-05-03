#include "WindowPreferences.h"
#include "ui_formPreferences.h"

WindowPreferences::WindowPreferences(QWidget *parent, Preferences::Preferences *prefs_) :
    QWidget(parent), ui(new Ui::WindowPreferences), prefs(prefs_) {

    ui->setupUi(this);
    setCurrentStateInUI();
}

void WindowPreferences::setCurrentStateInUI() {
/* Model settings */
    ui->countPoints->setValue(prefs->visualization.locusNumber);
    ui->pointsOneStart->setValue(prefs->model.pointsNumber);
    ui->deltaStart->setValue(prefs->model.startPointDelta);
    ui->tauBox->setValue(prefs->model.deltaTime);
    ui->xCoord->setValue(prefs->model.startPoint.x);
    ui->yCoord->setValue(prefs->model.startPoint.y);
    ui->zCoord->setValue(prefs->model.startPoint.z);

/* Camera settings */
    ui->sensSlider->setValue((prefs->camera.sensitivity - 0.0005) / (0.03 - 0.0005) * 100);
    ui->speedSlider->setValue((prefs->camera.speed - 0.05) / (0.3 - 0.05) * 100);

/* View settings */
    ui->tailSize->setValue(prefs->visualization.tailPointsNumber);
    ui->timerSpeed->setValue(prefs->controller.deltaTimePerStep);
    ui->interDegree->setValue((0.21 - prefs->visualization.interpolationDistance) / 0.002);

}

void WindowPreferences::setStateFromUI() {
/* Model settings */
    prefs->visualization.locusNumber = ui->countPoints->value();
    prefs->model.pointsNumber        = ui->pointsOneStart->value();
    prefs->model.startPointDelta     = ui->deltaStart->value();
    prefs->model.deltaTime           = ui->tauBox->value();
    prefs->model.startPoint.x        = ui->xCoord->value();
    prefs->model.startPoint.y        = ui->yCoord->value();
    prefs->model.startPoint.z        = ui->zCoord->value();

/* Camera settings */
    prefs->camera.speed       = 0.05 + ui->speedSlider->value() / 100.0 * (0.3 - 0.05);
    prefs->camera.sensitivity = 0.0005 + ui->sensSlider->value() / 100.0 * (0.03 - 0.0005);

/* View settings */
    prefs->visualization.tailPointsNumber      = ui->tailSize->value();
    prefs->controller.deltaTimePerStep         = ui->timerSpeed->value();
    prefs->visualization.interpolationDistance = 0.21 - ui->interDegree->value() * 0.002;

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
