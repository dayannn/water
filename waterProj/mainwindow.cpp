#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->paintWidget->setImg();
    facade = new Facade(ui->paintWidget);

    mouseLeftButtonClicked = false;

    pressed_4 = pressed_5 = pressed_6 = pressed_8 = false;

    settingsWnd = new SettingsWindow();
    settingsWnd->setFacade(facade);
    connect (ui->action, SIGNAL (triggered()), settingsWnd, SLOT (show()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete facade;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (event->type() == QEvent::MouseButtonPress)
            mouseLeftButtonClicked = true;
        else if (event->type() == QEvent::MouseButtonRelease)
            mouseLeftButtonClicked = false;
        mouseXPos = event->pos().x();
        mouseYPos = event->pos().y();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->pos().x() - mouseXPos;
    int dy = event->pos().y() - mouseYPos;

    mouseXPos = event->pos().x();
    mouseYPos = event->pos().y();

    facade->transformCamera({TRANSFORM_ROTATE_Y, double(dx)/100});
    facade->transformCamera({TRANSFORM_ROTATE_X, -double(dy)/100});
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
        facade->transformCamera({TRANSFORM_MOVE_SIDE, TRANSFORM_DELTA_MOVE});
        break;
    case Qt::Key_D:
        facade->transformCamera({TRANSFORM_MOVE_SIDE, -TRANSFORM_DELTA_MOVE});
        break;
    case Qt::Key_W:
        facade->transformCamera({TRANSFORM_MOVE_FORWARD, TRANSFORM_DELTA_MOVE});
        break;
    case Qt::Key_S:
        facade->transformCamera({TRANSFORM_MOVE_FORWARD, -TRANSFORM_DELTA_MOVE});
        break;
    case Qt::Key_Q:
        facade->transformCamera({TRANSFORM_MOVE_Y, TRANSFORM_DELTA_MOVE});
        break;
    case Qt::Key_E:
        facade->transformCamera({TRANSFORM_MOVE_Y, -TRANSFORM_DELTA_MOVE});
        break;

    case Qt::Key_Z:
        facade->transformModel({TRANSFORM_SCALE, 2*TRANSFORM_DELTA});
        break;
    case Qt::Key_X:
        facade->transformModel({TRANSFORM_SCALE, (TRANSFORM_DELTA/2)});
        break;

    case Qt::Key_8:
        pressed_8 = true;
        if (pressed_6)
            facade->moveBoat(0.5, -0.1);
        else if (pressed_4)
            facade->moveBoat(0.5, 0.1);
        else
            facade->moveBoat(0.5, 0);
        break;

    case Qt::Key_4:
        pressed_4 = true;
        if (pressed_8)
            facade->moveBoat(0.5, 0.1);
        else if (pressed_5)
            facade->moveBoat(-0.5, 0.1);
        else
            facade->moveBoat(0, 0.1);
        break;

    case Qt::Key_5:
        pressed_5 = true;
        if (pressed_6)
            facade->moveBoat(0.5, -0.1);
        else if (pressed_4)
            facade->moveBoat(0.5, 0.1);
        else
            facade->moveBoat(-0.5, 0);
        break;

    case Qt::Key_6:
        pressed_6 = true;
        if (pressed_8)
            facade->moveBoat(0.5, -0.1);
        else if (pressed_5)
            facade->moveBoat(-0.5, -0.1);
        else
            facade->moveBoat(0, -0.1);
        break;

    case Qt::Key_T:
        facade->transformCamera({TRANSFORM_ROTATE_X, TRANSFORM_DELTA});
        break;
    case Qt::Key_G:
        facade->transformCamera({TRANSFORM_ROTATE_X, -TRANSFORM_DELTA});
        break;
    case Qt::Key_Y:
        facade->transformCamera({TRANSFORM_ROTATE_Y, TRANSFORM_DELTA});
        break;
    case Qt::Key_H:
        facade->transformCamera({TRANSFORM_ROTATE_Y, -TRANSFORM_DELTA});
        break;
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_8:
        pressed_8 = false;
        break;

    case Qt::Key_4:
        pressed_4 = false;
        break;

    case Qt::Key_5:
        pressed_5 = false;
        break;

    case Qt::Key_6:
        pressed_6 = false;
        break;
    }
}

void MainWindow::on_polygonalModelRadioButton_toggled(bool checked)
{
    if (!checked)
        return;

    facade->changeModelType(MODEL_TYPE_POLYGONAL);

}

void MainWindow::on_carcasModelRadioButton_toggled(bool checked)
{
    if (!checked)
        return;

    facade->changeModelType(MODEL_TYPE_CARCASS);
}

void MainWindow::on_action_triggered()
{

}

void MainWindow::on_loadSceneAction_triggered()
{
    std::string filename = "camera.txt";
    facade->loadCameraFromFile({SOURCE_FILE, &filename});
}
