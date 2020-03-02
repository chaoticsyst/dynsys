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

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:

    Ui::Window *ui;
};
