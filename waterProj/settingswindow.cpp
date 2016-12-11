#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    ui->amb_light_pick_widget->setColor(QColor(68, 69, 50));
    ui->diff_light_pick_widget->setColor(QColor(248, 238, 228));
    ui->spec_light_pick_widget->setColor(QColor(255, 238, 175));
    connect(ui->amb_light_pick_widget, SIGNAL(sendColor(QColor)), this, SLOT(amb_Light_Changed(QColor)));
    connect(ui->diff_light_pick_widget, SIGNAL(sendColor(QColor)), this, SLOT(diff_Light_Changed(QColor)));
    connect(ui->spec_light_pick_widget, SIGNAL(sendColor(QColor)), this, SLOT(spec_Light_Changed(QColor)));
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::setFacade(Facade *facade)
{
    _facade = facade;
}

void SettingsWindow::on_light_x_slider_valueChanged(int value)
{
    _facade->setLightPoint(Vec3d(value, ui->light_y_slider->value(), ui->light_z_slider->value()));
}

void SettingsWindow::on_light_y_slider_valueChanged(int value)
{
     _facade->setLightPoint(Vec3d(ui->light_x_slider->value(), value, ui->light_z_slider->value()));
}

void SettingsWindow::on_light_z_slider_valueChanged(int value)
{
     _facade->setLightPoint(Vec3d(ui->light_x_slider->value(), ui->light_y_slider->value(), value));
}

void SettingsWindow::amb_Light_Changed(QColor clr)
{
    _facade->setAmbLight(&clr);
}

void SettingsWindow::diff_Light_Changed(QColor clr)
{
    _facade->setDiffLight(&clr);
}

void SettingsWindow::spec_Light_Changed(QColor clr)
{
    _facade->setSpecLight(&clr);
}
