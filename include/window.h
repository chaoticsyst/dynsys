#ifndef WINDOW_H
#define WINDOW_H

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
    void slot_set_constant_a(double){};//TODO: safe delete
    void slot_set_constant_b(double){};//TODO: safe delete
    void slot_set_constant_c(double){};//TODO: safe delete

    void slot_restart_button();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:

    Ui::Window *ui;
};

#endif // WINDOW_H
