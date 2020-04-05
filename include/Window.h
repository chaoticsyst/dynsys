#pragma once

#include <QWidget>
#include <QSlider>
#include <QVector>

#include "WindowPreferences.h"

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

    int timeValue = 0;

    bool pauseState = 0;

    QTimer *sliderTimer;

    WindowPreferences *windowPreferences;
    Ui::Window *ui;
};
