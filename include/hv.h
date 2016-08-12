#ifndef HV_H
#define HV_H

#include <QWidget>
#include <QTimer>
#include "hvmodule.h"

namespace Ui {
class hv;
}

class hv : public QWidget
{
    Q_OBJECT

public:
    explicit hv(QWidget *parent = 0, int handleChef_ = 0);
    ~hv();

private slots:
    void on_spinBox_hv_rotary_switches_valueChanged(int rotSw);

private:
    // user interface
    Ui::hv *ui;

    // instance of corresponding module
    HVModule *module;

    // utils
    void makeItNice();
    void launchTimer(int interval);


    // memthods
    void updateBaseAddress(int rotSw);
    void updateVMAX();
    void updateIMAX();
    void updateUI();

    // members
    QTimer* timer;



};

#endif // HV_H
