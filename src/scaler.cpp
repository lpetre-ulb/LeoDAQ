#include "scaler.h"
#include "ui_scaler.h"

scaler::scaler(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::scaler)
{
    ui->setupUi(this);
}

scaler::~scaler()
{
    delete ui;
}
