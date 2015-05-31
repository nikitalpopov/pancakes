#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
//  #include <qglobal.h>

#include "pananalytics.h"

class panAnalytics;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;

    unsigned getCounter();
    void drawPan();
    void drawPancake(unsigned n);

    void addStringToConsole(QString s);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

    void on_runPushButton_clicked();

private:

    QGraphicsScene *scene;

    QPen pen;
    QColor penColor;
    QBrush brush;

    unsigned counter;

    double volume;
    double x;
    double y;
    double radius;
    panAnalytics *pan;

};

#endif // MAINWINDOW_H
