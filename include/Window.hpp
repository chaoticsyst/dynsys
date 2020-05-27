#pragma once

#include <QWidget>
#include <QSlider>
#include <QVector>
#include <QVector3D>

#include "DynamicSystems/DynamicSystem.hpp"
#include "Preferences.hpp"
#include "WindowPreferences.hpp"
#include "DynamicSystemWrapper.hpp"
#include "StoppableTask.hpp"

namespace Ui {
class Window;
}

class CountPointsTask;

class Window : public QWidget {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

public slots:
    void updateOpenGLWidget(QVector<QVector3D>);

    void updateVideoRecordingState();
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
    friend class CountPointsTask;
    void afterCountPointsUIUpdate();

    void insertConstants(const std::vector<std::pair<std::string, std::vector<long double>>> &);

    void insertExpressions(std::array<std::string_view, 3> array, bool);

    using LambdaPushBackAction = decltype(DynamicSystemWrapper_n::getPushBackAndNormalizeLambda(std::declval<QVector<QVector3D> &>(),
                                                                                                std::declval<float>()));
    using DynamicSystemWrapper = DynamicSystems::DynamicSystem<LambdaPushBackAction>;

    static DynamicSystemWrapper getCustomSystem(const std::array<std::string, 3> &);

    std::map<QString, DynamicSystemWrapper> dynamicSystems;

    int timeValue = 0;

    bool pauseState = false;

    Preferences::Preferences prefs;

    QTimer *sliderTimer;

    WindowPreferences *windowPreferences;
    Ui::Window *ui;
    CountPointsTask *task;
};


class CountPointsTask : public StoppableTask {
    Q_OBJECT

signals:
    void updater(QVector<QVector3D>);
public:
    CountPointsTask(Window& wind_);

    void run() override;

public slots:
    void afterRun() override {
        wind.afterCountPointsUIUpdate();
    }
private:
    Window& wind;
};
