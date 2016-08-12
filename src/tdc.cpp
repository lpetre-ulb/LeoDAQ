#include "tdc.h"
#include "ui_tdc.h"

tdc::tdc(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::tdc)
{
    ui->setupUi(this);
}

tdc::~tdc()
{
    delete ui;
}
