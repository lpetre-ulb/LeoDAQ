#include "tdc.h"
#include "ui_tdc.h"

tdc::tdc(QWidget *parent, int handleChef_) :
    QWidget(parent), ui(new Ui::tdc), isRunning(false)
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
    isEnabledTDCUi = enable;
    ui->pushButton_tdc_clear->setEnabled(enable);
    ui->pushButton_tdc_clear_tdcs->setEnabled(enable);
    ui->pushButton_tdc_file_name->setEnabled(enable);
    ui->pushButton_tdc_start_run->setEnabled(enable && !isRunning);
    ui->pushButton_tdc_stop_run->setEnabled(enable && isRunning);
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

void tdc::on_pushButton_tdc_clear_tdcs_clicked()
{
    // software clear
    module->clear();
}

void tdc::on_pushButton_tdc_start_run_clicked()
{
    isRunning = true;
    ui->pushButton_tdc_start_run->setEnabled(!isRunning);
    ui->pushButton_tdc_stop_run->setEnabled(isRunning);

    u_int16_t tdcWindowWidth = ui->spinBox_tdc_window_width->value();
    int16_t tdcWindowOffset = ui->spinBox_tdc_window_offset->value();

    QString fileName = ui->lineEdit_tdc_file_name->text();


    // check config of trigger window and set it
    if (tdcWindowOffset + tdcWindowWidth < 0) { //1st case: the trigger matching window precedes the trigger arrival
        if (-tdcWindowOffset > 102375 || tdcWindowOffset == 0) {
            qDebug() << "false";
            return;
        }
    }
    else if (tdcWindowOffset + tdcWindowWidth > 1000) { // 2nd case: the trigger matching is straddling the trigger or delayed with respect to the trigger
        qDebug() << "false";
        return;
    }

    bool goodConfig = module->setTriggerMode(tdcWindowWidth, tdcWindowOffset);
    if (!goodConfig) {
        qDebug() << "false";
        return;
    }

    // set channel
    module->setStartStopChannels(ui->spinBox_tdc_start->value(), ui->spinBox_tdc_stop->value());

    // reading data
    while (isRunning) {
        QCoreApplication::processEvents();

        std::vector<int> values;
        module->readEvents(&values);
        int nEvents = values.size();
        for (int i = 0; i < nEvents; ++i) {
            hTDC->updatePlot(values[i]);
        }

    }




}

void tdc::on_pushButton_tdc_stop_run_clicked()
{
    if (isRunning) isRunning = false;
}
