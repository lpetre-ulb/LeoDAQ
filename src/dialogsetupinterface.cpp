#include "dialogsetupinterface.h"
#include "ui_dialogsetupinterface.h"
#include <QDebug>
#include <QMessageBox>

DialogSetupInterface::DialogSetupInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupInterface)
{
    ui->setupUi(this);
    this->setFixedSize(609, 453);
    this->setWindowTitle("Welcome to the IIHE DAQ - version 1.0 (2017)");
    this->setStyleSheet("QDialog {background:url(:/cloudchamber.png);}");
    this->setStyleSheet("QLabel {background: transparent;}");
    ui->pushButton_done->setStyleSheet("color:#00007f");
    ui->pushButton_cancel->setStyleSheet("color:#00007f");
    ui->widget_conf->setStyleSheet("QCheckBox {border:none; font-size:12pt; color:#00007f}QLabel {font-size:12pt; color:#00007f}");

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

    if (config.isEmpty()) {
        QMessageBox::information(
            this,
            "Error",
            tr("You must select at least one module, or cancel."),
            QMessageBox::Ok,
            QMessageBox::Ok
        );
    }
    else {
        this->accept();
    }
}

QVector<QString> DialogSetupInterface::getConfig()
{
    return config;
}

void DialogSetupInterface::on_pushButton_cancel_clicked()
{
    config.clear();
    this->accept();
}

void DialogSetupInterface::on_checkBox_efficiency_tab_stateChanged()
{
    if (ui->checkBox_efficiency_tab->isChecked()) {
        if (!ui->checkBox_hv_module->isChecked() || !ui->checkBox_scaler_module->isChecked())

            ui->checkBox_hv_module->setChecked(true);
            ui->checkBox_scaler_module->setChecked(true);

    }
}

void DialogSetupInterface::on_checkBox_muon_life_clicked()
{
    if (ui->checkBox_muon_life->isChecked()) {
        ui->checkBox_cherenkov_electron->setChecked(false);
        ui->checkBox_cherenkov_muon->setChecked(false);
        ui->checkBox_cosmic_muon->setChecked(false);
        ui->checkBox_hv_module->setChecked(true);
        ui->checkBox_tdc_module->setChecked(true);
        ui->checkBox_qdc_module->setChecked(false);
        ui->checkBox_efficiency_tab->setChecked(false);
        ui->checkBox_scaler_module->setChecked(false);
    }
}

void DialogSetupInterface::on_checkBox_cosmic_muon_clicked()
{
    if (ui->checkBox_cosmic_muon->isChecked()) {
        ui->checkBox_cherenkov_electron->setChecked(false);
        ui->checkBox_cherenkov_muon->setChecked(false);
        ui->checkBox_muon_life->setChecked(false);
        ui->checkBox_hv_module->setChecked(true);
        ui->checkBox_tdc_module->setChecked(true);
        ui->checkBox_qdc_module->setChecked(false);
        ui->checkBox_efficiency_tab->setChecked(false);
        ui->checkBox_scaler_module->setChecked(false);
    }
}

void DialogSetupInterface::on_checkBox_cherenkov_muon_clicked()
{
    if (ui->checkBox_cherenkov_muon->isChecked()) {
        ui->checkBox_cherenkov_electron->setChecked(false);
        ui->checkBox_cosmic_muon->setChecked(false);
        ui->checkBox_muon_life->setChecked(false);
        ui->checkBox_hv_module->setChecked(false);
        ui->checkBox_tdc_module->setChecked(false);
        ui->checkBox_qdc_module->setChecked(true);
        ui->checkBox_efficiency_tab->setChecked(false);
        ui->checkBox_scaler_module->setChecked(false);
    }
}

void DialogSetupInterface::on_checkBox_cherenkov_electron_clicked()
{
    if (ui->checkBox_cherenkov_electron->isChecked()) {
        ui->checkBox_cherenkov_muon->setChecked(false);
        ui->checkBox_cosmic_muon->setChecked(false);
        ui->checkBox_muon_life->setChecked(false);
        ui->checkBox_hv_module->setChecked(false);
        ui->checkBox_tdc_module->setChecked(false);
        ui->checkBox_qdc_module->setChecked(true);
        ui->checkBox_efficiency_tab->setChecked(false);
        ui->checkBox_scaler_module->setChecked(false);
    }
}
