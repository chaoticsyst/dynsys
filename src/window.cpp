#include <QtWidgets>

#include "Model.h"
#include "window.h"
#include "ui_form.h"

#include "visual_opengl_widget.h"

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    ui->setupUi(this);
}

void Window::slot_set_constant_a(double a) {
    set_constant_a(a);
}

void Window::slot_set_constant_b(double b) {
    set_constant_b(b);
}

void Window::slot_set_constant_c(double c) {
    set_constant_c(c);
}

Window::~Window() {
    delete ui;
}

void Window::ketPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(event);
}

