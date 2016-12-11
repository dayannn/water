#ifndef COLORPICKWIDGET_H
#define COLORPICKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QColor>
#include <QColorDialog>

class colorPickWidget : public QWidget
{
    Q_OBJECT
public:
    explicit colorPickWidget(QWidget *parent = 0);
    ~colorPickWidget();
    void setColor(QColor clr);
    QColor getColor();

private:
    QColor color;
    QColorDialog *clrDlg;

signals:

public slots:
    void changeColor();
    void getColor (QColor clr);

private slots:
    void colorRequested();

signals:
    sendColor (QColor clr);

protected:
    void paintEvent (QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
};

#endif // COLORPICKWIDGET_H
