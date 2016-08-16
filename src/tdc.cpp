#include "tdc.h"
#include "ui_tdc.h"

tdc::tdc(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::tdc)
{
    ui->setupUi(this);


    // build the corresponding module
    module = new TDCModule(handleChef_);
    updateBaseAddress(ui->spinBox_tdc_rotary_switches->value());

    // create the histogram
    hTDC = new histogram(ui->qcp_tdc, "TDC Delta t", "time (ns)", "# events");
    hTDC->adjustPlot(ui->spinBox_tdc_nbins->value(), ui->doubleSpinBox_tdc_tmin->value(), ui->doubleSpinBox_tdc_tmax->value());
}

tdc::~tdc()
{
    delete module;
    delete hTDC;
    delete ui;
}

void tdc::updateBaseAddress(int rotSw)
{
    // setBaseAddress updates and tries to access some registers
    // to check if connection is possible
    QString fwRev = module->setBaseAddress(rotSw);
    ui->lineEdit_tdc_firwmare_revision->setText(fwRev);
    if (fwRev != "UNKNOWN") {
        enabledTDCUi(true);
    }
    else {
        enabledTDCUi(false);
    }
}

void tdc::enabledTDCUi(bool enable)
{
    ui->pushButton_tdc_clear->setEnabled(enable);
    ui->pushButton_tdc_clear_tdcs->setEnabled(enable);
    ui->pushButton_tdc_file_name->setEnabled(enable);
    ui->pushButton_tdc_start_run->setEnabled(enable);
    ui->pushButton_tdc_stop_run->setEnabled(enable);
    ui->widget_tdc_channel->setEnabled(enable);
    ui->widget_tdc_trigger->setEnabled(enable);
    ui->widget_tdc_histogram_config->setEnabled(enable);
    ui->widget_tdc_lifetime_statistics->setEnabled(enable);
    ui->widget_tdc_event_statistics->setEnabled(enable);
}

void tdc::on_spinBox_tdc_rotary_switches_valueChanged(int rotSw)
{
    updateBaseAddress(rotSw);
}
