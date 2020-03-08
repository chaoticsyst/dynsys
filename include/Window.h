#pragma once

#include <QWidget>
#include <QSlider>

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
    void updateSlider();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void insertConstants(std::vector<std::pair<QString, std::vector<double>>>&);

    int timeValue = 0;

    bool pauseState = 0;

    QTimer *sliderTimer;

    Ui::Window *ui;
};
