#include "hv.h"
#include "ui_hv.h"

hv::hv(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::hv)
{
    ui->setupUi(this);

    // build the corresponding module
    module = new HVModule(handleChef_, ui->spinBox_hv_rotary_switches->value());
}

hv::~hv()
{
    delete module;
    delete ui;
}

void hv::on_spinBox_hv_rotary_switches_valueChanged(int rotSw)
{

}
