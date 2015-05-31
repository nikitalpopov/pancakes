#ifndef PANANALYTICS_H
#define PANANALYTICS_H

#include <iostream>
#include <vector>
#include <cmath>
#include "mainwindow.h"

enum e_usage
{
    BAKED,
    UNBAKED,
    NOT_USED
};

class MainWindow;
struct pancake
{
public:
    double volume;
    double x;
    double y;
    double radius;
    e_usage  usage;
    e_usage  usagePerStep;
    bool  isIdeal;

    pancake()
    {
        volume = 0;
        x      = 0;
        y      = 0;
        radius = 0;
        usage         = NOT_USED;
        usagePerStep  = NOT_USED;
        isIdeal       = false;
    }
};
class panAnalytics
{
public:
    panAnalytics(MainWindow *ui);
    ~panAnalytics();

    double residue;                    //  Остаток от totalVolume при считывании

    void   setPanRadius(double rad);
    double getPanRadius();

    void   setHeight(double hei);
    double getHeight();

    void   setTotalVolume(double tot);
    double getTotalVolume();

    void setCounter(unsigned n);

    unsigned getSize();
    unsigned getStep();

    double getCakeVolume(int n);
    double getCakeX(int n);
    double getCakeY(int n);
    double getCakeRadius(int n);

    void  setNewCakeRadius(int n, double d, double R, double square);

    e_usage  getUsage(int n);
    void     setUsage(int n, e_usage val);
    e_usage  getUsagePerStep(int n);
    void     setUsagePerStep(int n, e_usage val);
    bool     isIdeal(int n);
    void     setIdeal(int n, bool val);

    void    addCake(pancake pan);
    pancake getCake(int n);

    void   runAnalytics();
    void   checkCake(unsigned n, double currentVolume);
    double newRadius(double R, double r, double D, double square);

    void drawPan();

private:
    MainWindow *gui;

    unsigned counter;
    double   panRadius;                  //  Pan diametr
    double   height;                     //  Pancake height
    double   totalVolume;                //  Pot volume

    unsigned steps;               //  Num of steps
    unsigned baked;               //  Num of baked cakes
    unsigned unbaked;             //  Num of unbaked cakes
    unsigned ideal;               //  Num of ideal cakes
    std::vector<pancake> pancakes;
};

#endif // PANANALYTICS_H
