#ifndef QDC_H
#define QDC_H

#include <QWidget>
#include "qdcmodule.h"
#include "qdebug.h"
#include "histogram.h"

namespace Ui {
class qdc;
}

class qdc : public QWidget
{
    Q_OBJECT

public:
    explicit qdc(QWidget *parent = 0, int32_t handleChef_ = 0);
    ~qdc();

private slots:
    void on_pushButton_clicked();

    void on_spinBox_qdc_channelN_valueChanged(int channelN);

    void on_pushButton_clearChannel_clicked();

    void on_spinBox_qdc_nbins_valueChanged(int nBins);

    void on_doubleSpinBox_qdc_vmin_valueChanged(double vMin);

    void on_doubleSpinBox_qdc_vmax_valueChanged(double vMax);

    void on_pushButton_qdc_startprofile_clicked();

    void on_pushButton_stopprofile_clicked();

    void on_pushButton_clearProfile_clicked();

    void on_spinBox_qdc_rotary_switches_valueChanged(int rotSw);



    void on_pushButton_qdc_file_name_clicked();

private:
    Ui::qdc *ui;
    QDCModule *module;
    histogram *hQDC;
    int nExp;

    void updateBaseAddress(int rotSw);
    void enabledQDCUi(bool enable);
    void setFileName();




};

#endif // QDC_H
