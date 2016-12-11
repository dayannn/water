#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include "facade.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();
    void setFacade(Facade* facade);

private slots:
    void on_light_x_slider_valueChanged(int value);

    void on_light_y_slider_valueChanged(int value);

    void on_light_z_slider_valueChanged(int value);

    void amb_Light_Changed(QColor clr);

    void diff_Light_Changed(QColor clr);

    void spec_Light_Changed(QColor clr);
private:
    Ui::SettingsWindow *ui;
    Facade* _facade;
};

#endif // SETTINGSWINDOW_H
