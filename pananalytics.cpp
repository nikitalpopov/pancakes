//  Custom  =============
#include "pananalytics.h"
//  C++  ================
#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

double F(double R, double r, double d)
{
    return 2.0 * acos((d * d + R * R - r * r) / (2.0 * R * d));
}


panAnalytics::panAnalytics(MainWindow *ui)
{
    gui         = ui;

    panRadius   = 0;
    height      = 0;
    totalVolume = 0;
    residue     = 0;

    counter     = 0;
    steps       = 0;
    baked       = 0;
    unbaked     = 0;
    ideal       = 0;
}

panAnalytics::~panAnalytics()
{

}


void     panAnalytics::setPanRadius(double rad)
{
    panRadius = rad;
}

double   panAnalytics::getPanRadius()
{
    return panRadius;
}


void     panAnalytics::setHeight(double hei)
{
    height = hei;
}

double   panAnalytics::getHeight()
{
    return height;
}


void     panAnalytics::setTotalVolume(double tot)
{
    totalVolume = tot;
}

double   panAnalytics::getTotalVolume()
{
    return totalVolume;
}

void     panAnalytics::setCounter(unsigned n)
{
    counter = n;
}


unsigned panAnalytics::getSize()
{
    return pancakes.size();
}

unsigned panAnalytics::getStep()
{
    return steps;
}


void     panAnalytics::addCake(pancake pan)
{
    pancakes.push_back(pan);
}

pancake  panAnalytics::getCake(int n)
{
    return pancakes[n];
}


double   panAnalytics::getCakeVolume(int n)
{
    return pancakes[n].volume;
}

double   panAnalytics::getCakeX(int n)
{
    return pancakes[n].x;
}

double   panAnalytics::getCakeY(int n)
{
    return pancakes[n].y;
}

double   panAnalytics::getCakeRadius(int n)
{
    return pancakes[n].radius;
}


void     panAnalytics::setNewCakeRadius(int n, double D, double R, double square)
{
    pancakes[n].radius = newRadius(R, pancakes[n].radius, D, square);
}

double   panAnalytics::newRadius(double R, double r, double D, double square)
{
    D = sqrt(D);
    while (true)
    {
        double sqr = (R * R * (F(R, r, D) - sin(F(R, r, D))) + r * r * (F(r, R, D) - sin(F(r, R, D)))) / 2.0;
        if (sqr >= square)
        {
            break;
        }
        r += 0.0001;
    }
    return r;
}


e_usage  panAnalytics::getUsage(int n)
{
    return pancakes[n].usage;
}

void     panAnalytics::setUsage(int n, e_usage val)
{
    pancakes[n].usage = val;
}


e_usage  panAnalytics::getUsagePerStep(int n)
{
    return pancakes[n].usagePerStep;
}

void     panAnalytics::setUsagePerStep(int n, e_usage val)
{
    pancakes[n].usagePerStep = val;
}


bool     panAnalytics::isIdeal(int n)
{
    return pancakes[n].isIdeal;
}

void     panAnalytics::setIdeal(int n, bool val)
{
    pancakes[n].isIdeal = val;
}


void     panAnalytics::runAnalytics()
{
    counter = gui->getCounter();

    while ((totalVolume > 0) && (baked + unbaked < counter))
    {
        double stepVolume = totalVolume;
        steps++;
        //-------------------------
        QString s;
        s.setNum(steps);
        s = "Step: " + s + "\n";
        gui->addStringToConsole(s);
        //-------------------------

        for (unsigned i = 0; i < counter; i++)
        {
            if (getUsage(i) == NOT_USED)
            {
                checkCake(i, stepVolume);

                if (getUsagePerStep(i) == BAKED)
                {
                    setUsage(i, BAKED);
                    baked++;
                    gui->drawPancake(i);
                    //------------------------
                    s.setNum(i);
                    s = "Pancake[" + s + "] is baking\n";
                    gui->addStringToConsole(s);
                    //------------------------

                    stepVolume = stepVolume - getCakeVolume(i);
                }
                else if ((getUsage(i) == UNBAKED) && (getUsagePerStep(i) == UNBAKED))
                {
                    s.setNum(i);
                    s = "Pancake[" + s + "] can't be baked\n";
                    unbaked++;
                    totalVolume = totalVolume - getCakeVolume(i);
                    stepVolume = stepVolume - getCakeVolume(i);
                }
                else if ((getUsage(i) == UNBAKED) && (getUsagePerStep(i) == NOT_USED))
                {
                    stepVolume = stepVolume - getCakeVolume(i);
                }
            }
        }

        totalVolume = stepVolume;

        for (unsigned i = 0; i < counter; i++)
        {
            setUsagePerStep(i, NOT_USED);
        }

        //------------------------
        s = "--------------------\n\n";
        gui->addStringToConsole(s);
        //------------------------
    }

    QString s, c;
    s.setNum(baked);
    c.setNum(steps);
    s = "Pancakes baked: " + s + " in " + c + " steps\n";
    gui->addStringToConsole(s);

    s.setNum(ideal);
    s = "Ideal: " + s + "\n";
    gui->addStringToConsole(s);

    if (baked == 0)
    {
        s = "0";
    }
    else
    {
        double res = (double)ideal / baked;
        s.setNum(((res) * 100));
    }
    s = "Percentage: " + s + " %\n";
    gui->addStringToConsole(s);
}

void     panAnalytics::checkCake(unsigned n, double currentVolume)
{
    setUsagePerStep(n, BAKED);

    if (currentVolume >= getCakeVolume(n))
    {
        double d = (getCakeX(n) * getCakeX(n) + getCakeY(n) * getCakeY(n));
        if (((d) < (getPanRadius() * getPanRadius())) && (getCakeVolume(n) <= 3.1415 * pow(getPanRadius(), 2)))  //  Checking if (x, y) in the pan
        {
            //  Checking w/ other cakes
            for (unsigned i = 0; i < counter; i++)
            {
                if (i != n)
                {
                    if (getUsagePerStep(i) == BAKED)
                    {
                        d = pow((getCakeX(n) - getCakeX(i)), 2) + pow((getCakeY(n) - getCakeY(i)), 2);
                        if (d > pow(getCakeRadius(n) + getCakeRadius(i), 2))
                        {
                            setIdeal(n, true);
                            setUsagePerStep(n, BAKED);
                        }
                        else
                        {
                            setUsagePerStep(n, UNBAKED);
                            setUsage(n, NOT_USED);
                        }
                    }
                }
            }
            //  ==========================

            //  Checking if ideal or not
            if (getUsagePerStep(n) == BAKED)
            {
                d = (getCakeX(n) * getCakeX(n) + getCakeY(n) * getCakeY(n));
                if (d <= pow((getPanRadius() - getCakeRadius(n)), 2))
                {
                    ideal++;
                    setUsage(n, BAKED);
                    setIdeal(n, true);
                }
                else
                {
                    setNewCakeRadius(n, d, getPanRadius(), getCakeVolume(n));
                    setUsage(n, BAKED);
                    setIdeal(n, false);
                }
            }
        }
        else
        {
            setUsage(n, UNBAKED);
            setUsagePerStep(n, UNBAKED);
        }
    }
    else
    {
        setUsagePerStep(n, UNBAKED);
    }
}
