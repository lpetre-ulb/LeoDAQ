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

    void on_spinBox_tdc_nbins_valueChanged(int nBins);

    void on_doubleSpinBox_tdc_tmin_valueChanged(double tMin);

    void on_doubleSpinBox_tdc_tmax_valueChanged(double tMax);

    void on_pushButton_tdc_log_clicked();

    void on_pushButton_tdc_clear_clicked();

    void on_spinBox_tdc_start_valueChanged(int startChannel);

    void on_spinBox_tdc_stop_valueChanged(int stopChannel);


public:
    bool stillRunning();
    void stopRun();

private:
    Ui::tdc *ui;

    // instance of corresponding module
    TDCModule *module;

    histogram *hTDC;

    QTime time, prevTime;
    int counter;
    double mean_arrival;
    double mean_arrival_square;
    double mean_lifetime;
    double mean_lifetime_square;


    bool isRunning;
    bool isEnabledTDCUi;

    void makeItNice();

    void updateBaseAddress(int rotSw);
    void enabledTDCUi(bool enable);
    void resetCounters();
    void updateStatUiAndPlot(double difference);
    void clearStatUi();



};

#endif // TDC_H
