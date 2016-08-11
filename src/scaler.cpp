#include "scaler.h"
#include "ui_scaler.h"

scaler::scaler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scaler)
{
    ui->setupUi(this);
}

scaler::~scaler()
{
    delete ui;
}
