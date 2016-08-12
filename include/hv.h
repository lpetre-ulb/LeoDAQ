#ifndef HV_H
#define HV_H

#include <QWidget>
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
    Ui::hv *ui;
    HVModule *module;

};

#endif // HV_H
