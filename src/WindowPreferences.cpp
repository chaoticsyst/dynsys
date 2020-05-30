#include <chrono>
#include <random>
#include <QColorDialog>

#include "WindowPreferences.hpp"
#include "ui_formPreferences.h"

WindowPreferences::WindowPreferences(QWidget *parent, Preferences::Preferences *prefs_) :
    QWidget(parent), ui(new Ui::WindowPreferences), prefs(prefs_) {

    ui->setupUi(this);
    setCurrentStateInUI();
}

QColor getColorFromQVector4D(const QVector4D &color) {
    return QColor(color.x() * 255, color.y() * 255, color.z() * 255, color.w() * 255);
}

QVector4D getQVector4DfromColor(const QColor &color) {
    return QVector4D(color.red(), color.green(), color.blue(), color.alpha()) / 255;
}

void WindowPreferences::addNewColorButton(QColor initColor) {
    constexpr static size_t MAX_COLORS_NUMBER = 7;

    if (static_cast<size_t>(ui->colorsHolderLayout->count()) >= MAX_COLORS_NUMBER) {
        return;
    }

    QPushButton *button = new QPushButton(ui->colorsHolderLayout->widget());
    button->setFlat(true);
    button->setAutoFillBackground(true);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum));
    button->setPalette(initColor);

    connect(button, SIGNAL(clicked()), this, SLOT(pickButtonColor()));

    ui->colorsHolderLayout->addWidget(button);
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

    for (const auto &color : prefs->visualization.colors) {
        addNewColorButton(getColorFromQVector4D(color));
    }

    if (prefs->visualization.arcadeMode) {
        ui->arcadeModeCheckBox->setCheckState(Qt::CheckState::Checked);
    } else {
        ui->arcadeModeCheckBox->setCheckState(Qt::CheckState::Unchecked);
    }

/* Camera settings */
    ui->videoWidthValue->setValue(prefs->video.width);
    ui->videoHeightValue->setValue(prefs->video.height);
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

    prefs->visualization.colors.clear();
    while (ui->colorsHolderLayout->count() > 1) {
        QPushButton *button = static_cast<QPushButton *>(ui->colorsHolderLayout->takeAt(1)->widget());
        prefs->visualization.colors.push_back(getQVector4DfromColor(button->palette().button().color()));
    }

    if (ui->arcadeModeCheckBox->checkState() == Qt::CheckState::Checked) {
        prefs->enableArcadeMode();
    } else {
        prefs->disableArcadeMode();
    }

/* Camera settings */
    prefs->video.width = ui->videoWidthValue->value();
    prefs->video.height = ui->videoHeightValue->value();

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

void WindowPreferences::addNewColor() {
    static std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

    int color = rnd();
    addNewColorButton(QColor::fromRgb(color > 0 ? color : -color));
}

void WindowPreferences::removeLastColor() {
    if (ui->colorsHolderLayout->count() <= 2) {
        return;
    }

    QLayoutItem *item = ui->colorsHolderLayout->takeAt(ui->colorsHolderLayout->count() - 1);
    delete item->widget();
    delete item;
}

void WindowPreferences::pickButtonColor() {
    QPushButton *button = static_cast<QPushButton *>(sender());
    QColor color = QColorDialog::getColor(Qt::white, button, "Choose a color",  QColorDialog::DontUseNativeDialog);
    if (color != QColor::Invalid) {
        button->setPalette(color);
    }
}

WindowPreferences::~WindowPreferences() {
    delete ui;
}
