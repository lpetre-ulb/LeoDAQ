#include "hv.h"
#include "ui_hv.h"
#include "qdebug.h"
#include <QMessageBox>

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

void hv::launchTimer(int interval_)
{
    interval = interval_;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateUI()));
    updateUI();
    timer->start(interval); //time specified in ms
}

void hv::pauseTimer()
{
    timer->stop();
    qDebug() << "Timer is paused";
}

void hv::restartTimer()
{
    timer->start(interval);
    qDebug() << "Timer is restarted";
}

void hv::updateUI()
{
    updateVMAX();
    updateIMAX();
    updateHV();
}

void hv::updateHV()
{

    ui->lcdNumber_hv_voltage_ch_0->display(module->getVoltage(0));
    ui->lcdNumber_hv_current_ch_0->display(module->getCurrent(0));

    ui->lcdNumber_hv_voltage_ch_1->display(module->getVoltage(1));
    ui->lcdNumber_hv_current_ch_1->display(module->getCurrent(1));

    ui->lcdNumber_hv_voltage_ch_2->display(module->getVoltage(2));
    ui->lcdNumber_hv_current_ch_2->display(module->getCurrent(2));

    ui->lcdNumber_hv_voltage_ch_3->display(module->getVoltage(3));
    ui->lcdNumber_hv_current_ch_3->display(module->getCurrent(3));

    ui->lcdNumber_hv_voltage_ch_4->display(module->getVoltage(4));
    ui->lcdNumber_hv_current_ch_4->display(module->getCurrent(4));

    ui->lcdNumber_hv_voltage_ch_5->display(module->getVoltage(5));
    ui->lcdNumber_hv_current_ch_5->display(module->getCurrent(5));

    bool hvOn = false;

    u_int16_t chStatus;
    chStatus = module->getChStatus(0);
    hvOn = hvOn || (chStatus & (0x1 << 0));
    ui->label_hv_state_led_ch_0->setPixmap(chStatus & (0x1 << 0) ? QPixmap(":/green.png") : QPixmap(":/lightgreen.png"));
    ui->label_hv_ramp_up_led_ch_0->setPixmap(chStatus & (0x1 << 1) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_ramp_down_led_ch_0->setPixmap(chStatus & (0x1 << 2) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_current_led_ch_0->setPixmap(chStatus & (0x1 << 3) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_over_voltage_led_ch_0->setPixmap(chStatus & (0x1 << 4) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_under_voltage_led_ch_0->setPixmap(chStatus & (0x1 << 5) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_maxv_led_ch_0->setPixmap(chStatus & (0x1 << 6) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_maxi_led_ch_0->setPixmap(chStatus & (0x1 << 7) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_trip_led_ch_0->setPixmap(chStatus & (0x1 << 8) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_power_led_ch_0->setPixmap(chStatus & (0x1 << 9) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_disabled_led_ch_0->setPixmap(chStatus & (0x1 << 11) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_interlock_led_ch_0->setPixmap(chStatus & (0x1 << 12) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));

    chStatus = module->getChStatus(1);
    hvOn = hvOn || (chStatus & (0x1 << 0));
    ui->label_hv_state_led_ch_1->setPixmap(chStatus & (0x1 << 0) ? QPixmap(":/green.png") : QPixmap(":/lightgreen.png"));
    ui->label_hv_ramp_up_led_ch_1->setPixmap(chStatus & (0x1 << 1) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_ramp_down_led_ch_1->setPixmap(chStatus & (0x1 << 2) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_current_led_ch_1->setPixmap(chStatus & (0x1 << 3) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_over_voltage_led_ch_1->setPixmap(chStatus & (0x1 << 4) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_under_voltage_led_ch_1->setPixmap(chStatus & (0x1 << 5) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_maxv_led_ch_1->setPixmap(chStatus & (0x1 << 6) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_maxi_led_ch_1->setPixmap(chStatus & (0x1 << 7) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_trip_led_ch_1->setPixmap(chStatus & (0x1 << 8) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_power_led_ch_1->setPixmap(chStatus & (0x1 << 9) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_disabled_led_ch_1->setPixmap(chStatus & (0x1 << 11) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_interlock_led_ch_1->setPixmap(chStatus & (0x1 << 12) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));

    chStatus = module->getChStatus(2);
    hvOn = hvOn || (chStatus & (0x1 << 0));
    ui->label_hv_state_led_ch_2->setPixmap(chStatus & (0x1 << 0) ? QPixmap(":/green.png") : QPixmap(":/lightgreen.png"));
    ui->label_hv_ramp_up_led_ch_2->setPixmap(chStatus & (0x1 << 1) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_ramp_down_led_ch_2->setPixmap(chStatus & (0x1 << 2) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_current_led_ch_2->setPixmap(chStatus & (0x1 << 3) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_over_voltage_led_ch_2->setPixmap(chStatus & (0x1 << 4) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_under_voltage_led_ch_2->setPixmap(chStatus & (0x1 << 5) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_maxv_led_ch_2->setPixmap(chStatus & (0x1 << 6) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_maxi_led_ch_2->setPixmap(chStatus & (0x1 << 7) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_trip_led_ch_2->setPixmap(chStatus & (0x1 << 8) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_power_led_ch_2->setPixmap(chStatus & (0x1 << 9) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_disabled_led_ch_2->setPixmap(chStatus & (0x1 << 11) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_interlock_led_ch_2->setPixmap(chStatus & (0x1 << 12) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));

    chStatus = module->getChStatus(3);
    hvOn = hvOn || (chStatus & (0x1 << 0));
    ui->label_hv_state_led_ch_3->setPixmap(chStatus & (0x1 << 0) ? QPixmap(":/green.png") : QPixmap(":/lightgreen.png"));
    ui->label_hv_ramp_up_led_ch_3->setPixmap(chStatus & (0x1 << 1) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_ramp_down_led_ch_3->setPixmap(chStatus & (0x1 << 2) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_current_led_ch_3->setPixmap(chStatus & (0x1 << 3) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_over_voltage_led_ch_3->setPixmap(chStatus & (0x1 << 4) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_under_voltage_led_ch_3->setPixmap(chStatus & (0x1 << 5) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_maxv_led_ch_3->setPixmap(chStatus & (0x1 << 6) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_maxi_led_ch_3->setPixmap(chStatus & (0x1 << 7) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_trip_led_ch_3->setPixmap(chStatus & (0x1 << 8) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_power_led_ch_3->setPixmap(chStatus & (0x1 << 9) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_disabled_led_ch_3->setPixmap(chStatus & (0x1 << 11) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_interlock_led_ch_3->setPixmap(chStatus & (0x1 << 12) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));

    chStatus = module->getChStatus(4);
    hvOn = hvOn || (chStatus & (0x1 << 0));
    ui->label_hv_state_led_ch_4->setPixmap(chStatus & (0x1 << 0) ? QPixmap(":/green.png") : QPixmap(":/lightgreen.png"));
    ui->label_hv_ramp_up_led_ch_4->setPixmap(chStatus & (0x1 << 1) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_ramp_down_led_ch_4->setPixmap(chStatus & (0x1 << 2) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_current_led_ch_4->setPixmap(chStatus & (0x1 << 3) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_over_voltage_led_ch_4->setPixmap(chStatus & (0x1 << 4) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_under_voltage_led_ch_4->setPixmap(chStatus & (0x1 << 5) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_maxv_led_ch_4->setPixmap(chStatus & (0x1 << 6) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_maxi_led_ch_4->setPixmap(chStatus & (0x1 << 7) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_trip_led_ch_4->setPixmap(chStatus & (0x1 << 8) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_power_led_ch_4->setPixmap(chStatus & (0x1 << 9) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_disabled_led_ch_4->setPixmap(chStatus & (0x1 << 11) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_interlock_led_ch_4->setPixmap(chStatus & (0x1 << 12) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));

    chStatus = module->getChStatus(5);
    hvOn = hvOn || (chStatus & (0x1 << 0));
    ui->label_hv_state_led_ch_5->setPixmap(chStatus & (0x1 << 0) ? QPixmap(":/green.png") : QPixmap(":/lightgreen.png"));
    ui->label_hv_ramp_up_led_ch_5->setPixmap(chStatus & (0x1 << 1) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_ramp_down_led_ch_5->setPixmap(chStatus & (0x1 << 2) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_current_led_ch_5->setPixmap(chStatus & (0x1 << 3) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_over_voltage_led_ch_5->setPixmap(chStatus & (0x1 << 4) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_under_voltage_led_ch_5->setPixmap(chStatus & (0x1 << 5) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_maxv_led_ch_5->setPixmap(chStatus & (0x1 << 6) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_maxi_led_ch_5->setPixmap(chStatus & (0x1 << 7) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_trip_led_ch_5->setPixmap(chStatus & (0x1 << 8) ? QPixmap(":/orange.png") : QPixmap(":/lightorange.png"));
    ui->label_hv_over_power_led_ch_5->setPixmap(chStatus & (0x1 << 9) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_disabled_led_ch_5->setPixmap(chStatus & (0x1 << 11) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));
    ui->label_hv_interlock_led_ch_5->setPixmap(chStatus & (0x1 << 12) ? QPixmap(":/red.png") : QPixmap(":/lightred.png"));

    ui->label_hv_danger->setEnabled(hvOn);

}

void hv::makeItNice()
{
    ui->lcdNumber_hv_vmax->setPalette(Qt::black);
    ui->lcdNumber_hv_imax->setPalette(Qt::black);
}

hv::~hv()
{
    pauseTimer();
    delete timer;
    delete module;
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

void hv::on_pushButton_hv_set_clicked()
{
    int channel, state, voltage, current, tripTime, vMax, rampDown, rampUp;
    channel = ui->spinBox_hv_channel->value();
    state = ui->radioButton_hv_on_off->isChecked();
    voltage = ui->spinBox_hv_voltage->value();
    current = ui->spinBox_hv_current->value();
    tripTime = ui->spinBox_hv_trip_time->value();
    vMax = ui->spinBox_hv_vmax->value();
    rampDown = ui->spinBox_hv_ramp_down->value();
    rampUp = ui->spinBox_hv_ramp_up->value();

    if (channel == -1) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "Confirmation",
            "Are you sure you want to set all the channels ?",
            QMessageBox::Yes|QMessageBox::No
        );

        if (reply == QMessageBox::No) return;

    }

    if (channel < -1 || channel > 5) {
         qDebug() << "Impossible channel";
    }
    else {
        int start = channel;
        int end = channel+1;
        if (channel == -1) {
            start = 0;
            end = 6;
        }
        for (int i = start; i < end; ++i) {
            module->setChannel(i, state, voltage, current, tripTime, vMax, rampDown, rampUp);
        }
    }

}

void hv::on_radioButton_hv_on_off_clicked()
{
    if (ui->radioButton_hv_on_off->isChecked()) {
        ui->radioButton_hv_on_off->setText("ON");
        ui->label_state_led->setPixmap(QPixmap(":/green.png"));
    }
    else {
        ui->radioButton_hv_on_off->setText("OFF");
        ui->label_state_led->setPixmap(QPixmap(":/grey.png"));
    }
}
