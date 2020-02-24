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
    void slot_set_constant_a(double a);
    void slot_set_constant_b(double b);
    void slot_set_constant_c(double c);

protected:
    void ketPressEvent(QKeyEvent *event);

private:

    Ui::Window *ui;
};

#endif // WINDOW_H
