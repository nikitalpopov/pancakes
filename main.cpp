//  Custom  ===========
#include "mainwindow.h"
#include "pananalytics.h"
//  Qt  ===============
#include <QApplication>
#include <QMainWindow>

using namespace std;

//  main  ==================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
