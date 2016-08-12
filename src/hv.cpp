#include "hv.h"
#include "ui_hv.h"
#include "qdebug.h"

hv::hv(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::hv)
{
    ui->setupUi(this);

    // change cosmetics
    makeItNice();

    // build the corresponding module
    module = new HVModule(handleChef_);
    updateBaseAddress(ui->spinBox_hv_rotary_switches->value());
    updateVMAX();
    updateIMAX();

    launchTimer(1000);

}

void hv::launchTimer(int interval)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateUI()));
    updateUI();
    timer->start(interval); //time specified in ms
}

void hv::updateUI() {
    updateVMAX();
    updateIMAX();
    //updateHV();
}

void hv::makeItNice()
{
    ui->lcdNumber_hv_vmax->setPalette(Qt::black);
    ui->lcdNumber_hv_imax->setPalette(Qt::black);
}

hv::~hv()
{
    //delete module;
    delete ui;
}

void hv::on_spinBox_hv_rotary_switches_valueChanged(int rotSw)
{
    updateBaseAddress(rotSw);
    updateVMAX();
    updateIMAX();
}

void hv::updateBaseAddress(int rotSw)
{
    // setBaseAddress updates and tries to access some registers
    // to check if connection is possible
    QString boardModel = module->setBaseAddress(rotSw);
    ui->lineEdit_hv_board_model->setText(boardModel);
    if (boardModel != "UNKNOWN") {
        ui->widget_hv_configuration->setEnabled(true);
    }
    else {
        ui->widget_hv_configuration->setEnabled(false);
    }
}

void hv::updateVMAX() {
    u_int16_t value16;
    CVErrorCodes res = module->getVMAX(&value16);
    if (res == cvSuccess) {
        ui->lcdNumber_hv_vmax->display(value16);
    }
    else {
        ui->lcdNumber_hv_vmax->display(888888);
    }
}

void hv::updateIMAX() {
    u_int16_t value16;
    CVErrorCodes res = module->getIMAX(&value16);
    if (res == cvSuccess) {
        ui->lcdNumber_hv_imax->display(value16);
    }
    else {
        ui->lcdNumber_hv_imax->display(888888);
    }
}
