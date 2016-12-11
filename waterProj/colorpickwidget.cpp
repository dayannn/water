#include "colorpickwidget.h"
#include <QPainter>

colorPickWidget::colorPickWidget(QWidget *parent) : QWidget(parent)
{
    color = QColor (0, 0, 0);
    clrDlg = new QColorDialog(color);
    connect (clrDlg, SIGNAL(accepted()), this, SLOT (changeColor()));
}

colorPickWidget::~colorPickWidget()
{
    delete clrDlg;
}

void colorPickWidget::setColor(QColor clr)
{
    color = clr;
    emit sendColor(color);
}

QColor colorPickWidget::getColor()
{
    return color;
}

void colorPickWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width() - 1, height() - 1, color);
    p.drawRect(0, 0, width() - 1, height() - 1);
    QPen pen;
    pen.setColor(QColor(60, 60, 60));
    p.setPen(pen);
    p.drawRect(1, 1, width() - 3, height() - 3);
}

void colorPickWidget::mousePressEvent(QMouseEvent  *)
{
    clrDlg->show();
}

void colorPickWidget::changeColor()
{
    color = clrDlg->currentColor();
    this->update();
    emit sendColor(color);
}

void colorPickWidget::getColor(QColor clr)
{
    color = clr;
    this->update();
}

void colorPickWidget::colorRequested()
{
    emit sendColor(color);
}
