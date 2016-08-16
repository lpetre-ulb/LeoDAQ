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

private slots:
    void on_spinBox_tdc_rotary_switches_valueChanged(int rotSw);

    void on_pushButton_tdc_clear_tdcs_clicked();

    void on_pushButton_tdc_start_run_clicked();

    void on_pushButton_tdc_stop_run_clicked();

private:
    Ui::tdc *ui;

    // instance of corresponding module
    TDCModule *module;

    histogram *hTDC;

    bool isRunning;
    bool isEnabledTDCUi;
    void updateBaseAddress(int rotSw);
    void enabledTDCUi(bool enable);


};

#endif // TDC_H
