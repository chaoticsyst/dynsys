#pragma once

#include <QObject>
#include <QWidget>

namespace Ui {
class WindowPreferences;
}

class WindowPreferences : public QWidget {
    Q_OBJECT
public:
    explicit WindowPreferences(QWidget *parent = nullptr);
    ~WindowPreferences();

public slots:
    void slot_cancel_button();
    void slot_set_default_button();
    void slot_apply_button();

private:
    void setCurrentStateInUI();
    void setStateFromUI();

    Ui::WindowPreferences *ui;
};
