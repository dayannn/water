#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "facade.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadCameraButton_clicked();

    void on_loadModelButton_clicked();


    void on_polygonalModelRadioButton_toggled(bool checked);

    void on_carcasModelRadioButton_toggled(bool checked);

private:
    Facade *facade;
    Ui::MainWindow *ui;
    bool mouseLeftButtonClicked;

    int mouseXPos;
    int mouseYPos;

    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;

    bool pressed_8;
    bool pressed_6;
    bool pressed_5;
    bool pressed_4;
};

const double TRANSFORM_DELTA = 0.1;
const double TRANSFORM_DELTA_MOVE = 0.5;

#endif // MAINWINDOW_H
