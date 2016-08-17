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

private:
    Ui::scaler *ui;

    //Instance of the corresponding module
    ScalerModule *module;

    histogram *hScaler;

    void updateBaseAddress(int rotSw);
    void enabledScalerUi(bool enable);
    void setPulserMode(unsigned char width, int unitIndex);



};

#endif // SCALER_H
