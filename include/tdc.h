#ifndef TDC_H
#define TDC_H

#include <QWidget>
#include "tdcmodule.h"
#include "histogram.h"


namespace Ui {
class tdc;
}

class tdc : public QWidget
{
    Q_OBJECT

public:
    explicit tdc(QWidget *parent = 0, int handleChef_ = 0);
    ~tdc();

private:
    Ui::tdc *ui;

    // instance of corresponding module
    TDCModule *module;

    histogram *hTDC;
};

#endif // TDC_H
