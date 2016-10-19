#include "dialogsetupinterface.h"
#include "ui_dialogsetupinterface.h"
#include <QDebug>

DialogSetupInterface::DialogSetupInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupInterface)
{
    ui->setupUi(this);
    if (this->exec() == QDialog::Rejected) {
        qDebug() << "Not configured";
    }
}

DialogSetupInterface::~DialogSetupInterface()
{
    delete ui;
}

void DialogSetupInterface::on_pushButton_done_clicked()
{
    if (ui->checkBox_hv_module->isChecked()) config.append("hvWidget");
    if (ui->checkBox_scaler_module->isChecked()) config.append("scalerWidget");
    if (ui->checkBox_tdc_module->isChecked()) config.append("tdcWidget");
    if (ui->checkBox_qdc_module->isChecked()) config.append("qdcWidget");
    if (ui->checkBox_efficiency_tab->isChecked()) config.append("efficiencyWidget");



    this->accept();
}

QVector<QString> DialogSetupInterface::getConfig()
{
    return config;
}

void DialogSetupInterface::on_pushButton_cancel_clicked()
{
    this->reject();
}

void DialogSetupInterface::on_checkBox_efficiency_tab_stateChanged(int arg1)
{
    if (ui->checkBox_efficiency_tab->isChecked()) {
        if (!ui->checkBox_hv_module->isChecked() || !ui->checkBox_scaler_module->isChecked())

            ui->checkBox_hv_module->setChecked(true);
            ui->checkBox_scaler_module->setChecked(true);

    }
}
