#pragma once

#include <QObject>
#include <QWidget>

#include "Preferences.hpp"

namespace Ui {
class WindowPreferences;
}

class WindowPreferences : public QWidget {
    Q_OBJECT
public:
    explicit WindowPreferences(QWidget *parent, Preferences::Preferences *prefs);
    ~WindowPreferences();

public slots:
    void slot_cancel_button();
    void slot_set_default_button();
    void slot_apply_button();

    void addNewColor();
    void removeLastColor();

private slots:
    void pickButtonColor();

private:
    void setCurrentStateInUI();
    void setStateFromUI();
    void addNewColorButton(QColor initColor);

    Ui::WindowPreferences *ui;

    Preferences::Preferences *prefs;
};
