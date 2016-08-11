#include "hv.h"
#include "ui_hv.h"

hv::hv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hv)
{
    ui->setupUi(this);
}

hv::~hv()
{
    delete ui;
}
