#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include "qcustomplot.h"


class graph
{
public:
    graph(QCustomPlot* qcp_, const QString &title_ = "", const QString& xLabel_ = "", const QString &yLabel_ = "");

private:
    QCustomPlot* qcp;

    QVector<double> x, y, ey, eylow, eyhigh;
    int nPoints;
    double xMin, xMax, yMin, yMax, max;
    bool xLog, yLog;
    QString xLabel, yLabel, title;

    void init(const QString &title_, const QString& xLabel_, const QString &yLabel_);
    void clearXY();

public:
    void clear();
    void addPoint(double x_, double y_, double ey_ = 0.0);
    void addPoint(double x_, double y_, double eylow_, double eyhigh_);
    void addEfficiencyPoint(double x_, uint num_, uint den_);
    void adjustPlot(double xMin_, double xMax_);
    double getXMin();
    double getXMax();
    bool setLogX();
    bool setLogY();
};

inline double graph::getXMin() {
    return xMin;
}

inline double graph::getXMax() {
    return xMax;
}

#endif // GRAPH_H
