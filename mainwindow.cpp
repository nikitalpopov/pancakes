//  Custom  ==============
#include "mainwindow.h"
#include "ui_mainwindow.h"
//  C++  =================
#include <cmath>
//  Qt  ==================
#include <QGraphicsScene>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pan = new panAnalytics(this);

    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    penColor = QColor();

    counter = 0;
    ui->lineEdit->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    pan->setPanRadius(ui->lineEdit->text().toDouble());
    pan->setPanRadius(pan->getPanRadius() / 2);

    drawPan();
    ui->graphicsView->scale(ui->graphicsView->width()/(pan->getPanRadius() * 100), ui->graphicsView->height()/(pan->getPanRadius() * 100));

    QString s;
    s.setNum(pan->getPanRadius());
    s = "Pan radius: " + s + "\n";
    ui->textBrowser_2->insertPlainText(s);

    ui->pushButton_2->setEnabled(true);
    ui->lineEdit_2->setEnabled(true);
    ui->lineEdit_2->setFocus();
}

void MainWindow::on_pushButton_2_clicked()
{
    pan->setHeight(ui->lineEdit_2->text().toDouble());

    QString s;
    s.setNum(pan->getHeight());
    s = "Pancakes height: " + s + "\n";
    ui->textBrowser_2->insertPlainText(s);

    ui->pushButton_3->setEnabled(true);
    ui->lineEdit_3->setEnabled(true);
    ui->lineEdit_3->setFocus();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton_4->setEnabled(true);
    ui->lineEdit_4->setEnabled(true);
    ui->lineEdit_4->setFocus();

    pan->setTotalVolume(ui->lineEdit_3->text().toDouble());
    pan->residue = pan->getTotalVolume();

    QString s;
    s.setNum(pan->getTotalVolume());
    s = "Pot volume: " + s + "\n\n";
    ui->textBrowser_2->insertPlainText(s);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->pushButton_5->setEnabled(true);
    ui->lineEdit_5->setEnabled(true);
    ui->lineEdit_5->setFocus();

    QString s;
    s = ui->lineEdit_4->text();
    if (s.isEmpty())
    {
        s = "0";
    }
    volume = s.toDouble();
    pan->residue = pan->residue - volume;

    if (pan->residue < 0)
    {
        ui->textBrowser_2->insertPlainText("All volume used\n\n");
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_6->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
    }
    else
    {
        radius = sqrt(volume / (pan->getHeight() * 3.1415));
        counter++;
        s = "Single Volume: " + s + "\n";
        ui->textBrowser_2->insertPlainText(s);
        //====================
        s.setNum(radius);
        s = "Cake radius: " + s + "\n";
        ui->textBrowser_2->insertPlainText(s);
        //====================
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->pushButton_6->setEnabled(true);
    ui->lineEdit_6->setEnabled(true);
    ui->lineEdit_6->setFocus();

    QString s;
    s = ui->lineEdit_5->text();
    if (s.isEmpty())
    {
        s = "0";
    }
    x = s.toDouble();
    s = "x: " + s + "\n";
    ui->textBrowser_2->insertPlainText(s);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->runPushButton->setEnabled(true);

    QString s;
    s = ui->lineEdit_6->text();
    if (s.isEmpty())
    {
        s = "0";
    }
    y = s.toDouble();
    s = "y: " + s + "\n\n";
    ui->textBrowser_2->insertPlainText(s);

    pancake current;
    current.volume = volume;
    current.x      = x;
    current.y      = y;
    current.radius = radius;
    pan->addCake(current);

    if (pan->residue == 0)
    {
        ui->textBrowser_2->insertPlainText("All volume used\n\n");
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_6->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
    }

    if (ui->lineEdit_4->isEnabled())
    {
        ui->lineEdit_4->setFocus();
    }
    else
    {
        pan->setCounter(counter);
        ui->runPushButton->setFocus();
    }
}

void MainWindow::on_runPushButton_clicked()
{
    pan->runAnalytics();
}

void MainWindow::addStringToConsole(QString s)
{
    ui->textBrowser_2->insertPlainText(s);
}


unsigned MainWindow::getCounter()
{
    return counter;
}

void MainWindow::drawPan()
{
    scene->clear();
    brush.setStyle(Qt::NoBrush);
    brush.setColor(QColor(Qt::transparent));
    scene->addEllipse((pan->getPanRadius()) * (-25), (pan->getPanRadius()) * (-25), (pan->getPanRadius()) * 50, (pan->getPanRadius()) * 50, pen, brush);
    scene->addEllipse((pan->getPanRadius()) * (-25) - 1, (pan->getPanRadius()) * (-25) - 1, (pan->getPanRadius()) * 50 + 2, (pan->getPanRadius()) * 50 + 2, pen, brush);
    scene->addEllipse((pan->getPanRadius()) * (-25) - 2, (pan->getPanRadius()) * (-25) - 2, (pan->getPanRadius()) * 50 + 4, (pan->getPanRadius()) * 50 + 4, pen, brush);
    scene->addEllipse(0, 0, 1, 1, pen, brush);
}

void MainWindow::drawPancake(unsigned n)
{
    pen.setColor(QColor("black"));
    brush.setColor(QColor("orange"));
    brush.setStyle(Qt::SolidPattern);
    scene->addEllipse(25 * (pan->getCakeX(n) - pan->getCakeRadius(n)), (-25) * (pan->getCakeY(n) + pan->getCakeRadius(n)), pan->getCakeRadius(n) * 50, pan->getCakeRadius(n) * 50, pen, brush);
}
