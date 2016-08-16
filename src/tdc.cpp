#include "tdc.h"
#include "ui_tdc.h"
#include "histogram.h"

tdc::tdc(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::tdc)
{
    ui->setupUi(this);


    // build the corresponding module
    module = new HVModule(handleChef_);
    updateBaseAddress(ui->spinBox_hv_rotary_switches->value());

    // create the histogram
    hTDC = new histogram(ui->qcp_tdc, "TDC Delta t", "time (ns)", "# events");
    hTDC->adjustPlot(ui->spinBox_tdc_nbins->value(), ui->doubleSpinBox_tdc_tmin->value(), ui->doubleSpinBox_tdc_tmax->value());
}

tdc::~tdc()
{
    delete ui;
}
