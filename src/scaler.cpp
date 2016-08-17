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

}
