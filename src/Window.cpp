#include <QtWidgets>

#include "Model.h"
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

Model::ModelName Window::getModelName() const {
    Model::ModelName modelName = Model::ModelName::ROSSLER;
    if (ui->comboBox->currentText() == "Аттрактор Лоренца") {
        modelName = Model::ModelName::LORENZ;
    } else if (ui->comboBox->currentText() == "Аттрактор Рёсслера") {
        modelName = Model::ModelName::ROSSLER;
    } else if (ui->comboBox->currentText() == "Аттрактор Chen-Lee") {
        modelName = Model::ModelName::CHEN_LEE;
    } else if (ui->comboBox->currentText() == "Аттрактор Aizawa") {
        modelName = Model::ModelName::AIZAWA;
    } else if (ui->comboBox->currentText() == "Аттрактор Henon") {
        modelName = Model::ModelName::HENON;
    } else if (ui->comboBox->currentText() == "Аттрактор HR3") {
        modelName = Model::ModelName::HR3;
    } else if (ui->comboBox->currentText() == "Аттрактор Chua") {
        modelName = Model::ModelName::CHUA;
    } else if (ui->comboBox->currentText() == "Аттрактор Gin") {
        modelName = Model::ModelName::GIN;
    } else if (ui->comboBox->currentText() == "Аттрактор Anishchenko-Astakhov") {
        modelName = Model::ModelName::ANISHCHENKO_ASTAKHOV;
    } else if (ui->comboBox->currentText() == "Аттрактор Arnedo") {
        modelName = Model::ModelName::ARNEODO;
    } else if (ui->comboBox->currentText() == "Аттрактор Bouali") {
        modelName = Model::ModelName::BOUALI_2;
    } else if (ui->comboBox->currentText() == "Аттрактор Burke-Shaw") {
        modelName = Model::ModelName::BURKE_SHAW;
    } else if (ui->comboBox->currentText() == "Аттрактор Chen-Celikovsky") {
        modelName = Model::ModelName::CHEN_CELIKOVSKY;
    } else if (ui->comboBox->currentText() == "Аттрактор Coullet") {
        modelName = Model::ModelName::COULLET;
    } else if (ui->comboBox->currentText() == "Аттрактор Dadras") {
        modelName = Model::ModelName::DADRAS;
    } else if (ui->comboBox->currentText() == "Аттрактор Dequan-Li") {
        modelName = Model::ModelName::DEQUAN_LI;
    } else if (ui->comboBox->currentText() == "Аттрактор Finance") {
        modelName = Model::ModelName::FINANCE;
    } else if (ui->comboBox->currentText() == "Аттрактор Four-wing") {
        modelName = Model::ModelName::FOUR_WING;
    } else if (ui->comboBox->currentText() == "Аттрактор Genesio-Tesi") {
        modelName = Model::ModelName::GENESIO_TESI;
    } else if (ui->comboBox->currentText() == "Аттрактор Hadley") {
        modelName = Model::ModelName::HADLEY;
    } else if (ui->comboBox->currentText() == "Аттрактор Halvorsen") {
        modelName = Model::ModelName::HALVORSEN;
    } else if (ui->comboBox->currentText() == "Аттрактор Liu-Chen") {
        modelName = Model::ModelName::LIU_CHEN;
    } else if (ui->comboBox->currentText() == "Аттрактор Lorenz mod 1") {
        modelName = Model::ModelName::LORENZ_MOD_1;
    } else if (ui->comboBox->currentText() == "Аттрактор Lorenz mod 2") {
        modelName = Model::ModelName::LORENZ_MOD_2;
    } else if (ui->comboBox->currentText() == "Аттрактор Lu-Chen") {
        modelName = Model::ModelName::LU_CHEN;
    } else if (ui->comboBox->currentText() == "Аттрактор Newton-Leipnik") {
        modelName = Model::ModelName::NEWTON_LEIPNIK;
    } else if (ui->comboBox->currentText() == "Аттрактор Nose-Hoover") {
        modelName = Model::ModelName::NOSE_HOOVER;
    } else if (ui->comboBox->currentText() == "Аттрактор Qi-Chen") {
        modelName = Model::ModelName::QI_CHEN;
    } else if (ui->comboBox->currentText() == "Аттрактор Rayleigh-Benard") {
        modelName = Model::ModelName::RAYLEIGH_BENARD;
    } else if (ui->comboBox->currentText() == "Аттрактор Rucklidge") {
        modelName = Model::ModelName::RUCKLIDGE;
    } else if (ui->comboBox->currentText() == "Аттрактор Sakaraya") {
        modelName = Model::ModelName::SAKARAYA;
    } else if (ui->comboBox->currentText() == "Аттрактор Shimizu-Morioka") {
        modelName = Model::ModelName::SHIMIZU_MORIOKA;
    } else if (ui->comboBox->currentText() == "Аттрактор Thomas") {
        modelName = Model::ModelName::THOMAS;
    } else if (ui->comboBox->currentText() == "Аттрактор TSUCS1") {
        modelName = Model::ModelName::TSUCS1;
    } else if (ui->comboBox->currentText() == "Аттрактор TSUCS2") {
        modelName = Model::ModelName::TSUCS2;
    } else if (ui->comboBox->currentText() == "Аттрактор Wang-Sun") {
        modelName = Model::ModelName::WANG_SUN;
    } else if (ui->comboBox->currentText() == "Аттрактор Wimol-Banlue") {
        modelName = Model::ModelName::WIMOL_BANLUE;
    } else if (ui->comboBox->currentText() == "Аттрактор Yu-Wang") {
        modelName = Model::ModelName::YU_WANG;
    }
    return modelName;
}

void Window::slot_restart_button() {
    Model::ModelName modelName = getModelName();

    std::vector<long double> constants = {
            ui->doubleSpinBox->value(),
            ui->doubleSpinBox_2->value(),
            ui->doubleSpinBox_3->value()
    };
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
                               modelName,
                               constants);
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
        ui->pointsViewQGLWidget->setCurrentTime(
                (Preferences::COUNT_POINTS / ui->horizontalSlider->maximum()) * timeValue);
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

