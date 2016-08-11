#include "tdc.h"
#include "ui_tdc.h"

tdc::tdc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tdc)
{
    ui->setupUi(this);
}

tdc::~tdc()
{
    delete ui;
}
