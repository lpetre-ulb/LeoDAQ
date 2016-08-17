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

private:
    Ui::scaler *ui;

    //Instance of the corresponding module
    ScalerModule *module;

    histogram *hScaler;

    void updateBaseAddress(int rotSw);
    void enabledScalerUi(bool enable);


};

#endif // SCALER_H
