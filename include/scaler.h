#ifndef SCALER_H
#define SCALER_H

#include <QWidget>
#include "scalermodule.h"
#include "histogram.h"

namespace Ui {
class scaler;
}

class scaler : public QWidget
{
    Q_OBJECT

public:
    explicit scaler(QWidget *parent = 0, int handleChef_ = 0);
    ~scaler();

private slots:
    void on_doubleSpinBox_scaler_cmax_valueChanged(double cMax);

    void on_doubleSpinBox_scaler_cmin_valueChanged(double cMin);

    void on_spinBox_scaler_nbins_valueChanged(int nBins);

    void on_pushButton_scaler_pulse_clicked();

    void on_spinBox_scaler_width_valueChanged(int width);

    void on_comboBox_scaler_unit_currentIndexChanged(int unitIndex);

    void on_pushButton_scaler_update_clicked();

    void on_pushButton_scaler_reset_clicked();

    void on_spinBox_scaler_rotary_switches_valueChanged(int rotSw);

    void on_pushButton_scaler_poisson_clicked();

    void on_pushButton_sclaer_stop_clicked();

    void on_pushButton_scaler_clear_clicked();

private:
    Ui::scaler *ui;

    //Instance of the corresponding module
    ScalerModule *module;

    histogram *hScaler;
    int nExp;

    void makeItNice();

    void updateBaseAddress(int rotSw);
    void enabledScalerUi(bool enable);
    void setPulserMode(unsigned char width, int unitIndex);


};

#endif // SCALER_H
