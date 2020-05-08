#pragma once

#include <QWidget>
#include <QSlider>
#include <QVector>

#include "DynamicSystems/DynamicSystem.hpp"
#include "Preferences.hpp"
#include "WindowPreferences.hpp"

namespace Ui::Utils {
/// Lambda-getter for typedef (you can find another way to define lambda type)
auto getPushBackAndNormalizeLambda(QVector<QVector3D> &vector, float normalizeConstant) {
    return [&vector, normalizeConstant](const Model::Point &point) {
        vector.push_back(
                QVector3D(static_cast<float>(point.x) / normalizeConstant,
                          static_cast<float>(point.y) / normalizeConstant,
                          static_cast<float>(point.z) / normalizeConstant)
        );
    };
}

}

namespace Ui {
class Window;
}

class Window : public QWidget {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

public slots:
    void slot_restart_button();
    void slot_time_slider(int);
    void slot_model_selection(QString);
    void slot_constants_selection(QString);
    void slot_pause_button();
    void slot_open_preferences();
    void updateSlider();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    template<typename Lambda>
    void count_points(Lambda derivatives_function);

    void insertConstants(QVector<std::pair<QString, QVector<double>>>&);

    /// Define type of LambdaNewPointAction
    typedef decltype(Ui::Utils::getPushBackAndNormalizeLambda(std::declval<QVector<QVector3D> &>(), std::declval<float>())) LambdaNewPointAction;

    std::vector<DynamicSystems::DynamicSystem<LambdaNewPointAction>> dynamicSystems;

    int timeValue = 0;

    bool pauseState = 0;

    Preferences::Preferences prefs;

    QTimer *sliderTimer;

    WindowPreferences *windowPreferences;
    Ui::Window *ui;
};
