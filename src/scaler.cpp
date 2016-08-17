#include "scaler.h"
#include "ui_scaler.h"

scaler::scaler(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::scaler)
{
    ui->setupUi(this);

    // build the corresponding module
    module = new ScalerModule(handleChef_);
    updateBaseAddress(ui->spinBox_scaler_rotary_switches->value());
    ui->comboBox_scaler_unit->setCurrentIndex(3);
    setPulserMode(ui->spinBox_scaler_width->value(), ui->comboBox_scaler_unit->currentIndex());

    hScaler = new histogram(ui->qcp_scaler, "Poisson Distribution", "count", "# events");
    hScaler->adjustPlot(ui->spinBox_scaler_nbins->value(), ui->doubleSpinBox_scaler_cmin->value(), ui->doubleSpinBox_scaler_cmax->value());
}

scaler::~scaler()
{
    delete ui;
}

void scaler::updateBaseAddress(int rotSw)
{
    // setBaseAddress updates and tries to access some registers
    // to check if connection is possible
    QString moduleType = module->setBaseAddress(rotSw);
    ui->lineEdit_scaler_module_type->setText(moduleType);

    enabledScalerUi(moduleType != "UNKNOWN");
}

void scaler::enabledScalerUi(bool enable)
{
    ui->widget_scaler_display->setEnabled(enable);
    ui->pushButton_scaler_poisson->setEnabled(enable);
}

void scaler::setPulserMode(unsigned char width, int unitIndex)
{
    module->setPulserMode(width, CVTimeUnits(unitIndex));
}

void scaler::on_doubleSpinBox_scaler_cmax_valueChanged(double cMax)
{
    double cMin = ui->doubleSpinBox_scaler_cmin->value();
    if (cMax <= cMin) return;
    hScaler->adjustPlot(ui->spinBox_scaler_nbins->value(), cMin, cMax);
}

void scaler::on_doubleSpinBox_scaler_cmin_valueChanged(double cMin)
{
    double cMax = ui->doubleSpinBox_scaler_cmax->value();
    if (cMax <= cMin) return;
    hScaler->adjustPlot(ui->spinBox_scaler_nbins->value(), cMin, cMax);
}

void scaler::on_spinBox_scaler_nbins_valueChanged(int nBins)
{
    hScaler->adjustPlot(nBins, ui->doubleSpinBox_scaler_cmin->value(), ui->doubleSpinBox_scaler_cmax->value());

}
