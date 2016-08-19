#include "efficiency.h"
#include "ui_efficiency.h"
#include <unistd.h>
#include <chrono>

Efficiency::Efficiency(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::Efficiency)
{
    ui->setupUi(this);

    // change cosmetics
    makeItNice();

    hvModule = new HVModule(handleChef_);
    updateHVBaseAddress(ui->spinBox_efficiency_hv_rotary_switches->value());
    updateVMAX();
    updateIMAX();

    scalerModule = new ScalerModule(handleChef_);
    updateScalerBaseAddress(ui->spinBox_efficiency_scaler_rotary_switches->value());


    ui->comboBox_efficiency_scaler_unit->setCurrentIndex(3);
    setPulserMode(ui->spinBox_efficiency_scaler_width->value(), ui->comboBox_efficiency_scaler_unit->currentIndex(), ui->spinBox_efficiency_scaler_repeat->value());


    grEfficiency = new graph(ui->qcp_efficiency, "Efficiency vs. HV", "HV (V)", "Efficiency");
    grEfficiency->adjustPlot(ui->spinBox_efficiency_hvmin->value(), ui->spinBox_efficiency_hvmax->value());



}

Efficiency::~Efficiency()
{
    delete ui;
}


void Efficiency::makeItNice()
{
    ui->lcdNumber_efficiency_hv_vmax->setPalette(Qt::black);
    ui->lcdNumber_efficiency_hv_imax->setPalette(Qt::black);
}

void Efficiency::updateHVBaseAddress(int rotSw)
{
    // setBaseAddress updates and tries to access some registers
    // to check if connection is possible
    QString boardModel = hvModule->setBaseAddress(rotSw);
    ui->lineEdit_efficiency_board_model->setText(boardModel);

    //ui->widget_hv_configuration->setEnabled(boardModel != "UNKNOWN");
}

void Efficiency::updateScalerBaseAddress(int rotSw)
{
    // setBaseAddress updates and tries to access some registers
    // to check if connection is possible
    QString moduleType = scalerModule->setBaseAddress(rotSw);
    ui->lineEdit_efficiency_module_type->setText(moduleType);

    //enabledScalerUi(moduleType != "UNKNOWN");
    //on_pushButton_scaler_update_clicked();

}

void Efficiency::setPulserMode(unsigned char width, int unitIndex, int repeat)
{
    scalerModule->setPulserMode(width, CVTimeUnits(unitIndex), repeat);
}

void Efficiency::on_spinBox_efficiency_hv_rotary_switches_valueChanged(int rotSw)
{
    updateHVBaseAddress(rotSw);
    updateVMAX();
    updateIMAX();
}

void Efficiency::on_spinBox_efficiency_scaler_rotary_switches_valueChanged(int rotSw)
{
    updateScalerBaseAddress(rotSw);
}

void Efficiency::updateVMAX()
{
    u_int16_t value16;
    CVErrorCodes res = hvModule->getVMAX(&value16);
    if (res == cvSuccess) {
        ui->lcdNumber_efficiency_hv_vmax->display(value16);
    }
    else {
        ui->lcdNumber_efficiency_hv_vmax->display(888888);
    }
}

void Efficiency::updateIMAX()
{
    u_int16_t value16;
    CVErrorCodes res = hvModule->getIMAX(&value16);
    if (res == cvSuccess) {
        ui->lcdNumber_efficiency_hv_imax->display(value16);
    }
    else {
        ui->lcdNumber_efficiency_hv_imax->display(888888);
    }
}

void Efficiency::on_spinBox_efficiency_hvmin_valueChanged(int hvmin)
{
    int hvmax = ui->spinBox_efficiency_hvmax->value();
    if (hvmin >= hvmax)  return;
    grEfficiency->adjustPlot(hvmin, hvmax);
}

void Efficiency::on_spinBox_efficiency_hvmax_valueChanged(int hvmax)
{
    int hvmin = ui->spinBox_efficiency_hvmin->value();
    if (hvmin >= hvmax)  return;
    grEfficiency->adjustPlot(hvmin, hvmax);
}

void Efficiency::on_pushButton_efficiency_clear_clicked()
{
    grEfficiency->clear();
}

void Efficiency::on_pushButton_efficiency_start_clicked()
{
    setPulserMode(ui->spinBox_efficiency_scaler_width->value(), ui->comboBox_efficiency_scaler_unit->currentIndex(), ui->spinBox_efficiency_scaler_repeat->value());
    grEfficiency->clear();
    stopRun = false;

    ui->pushButton_efficiency_start->setEnabled(false);
    ui->pushButton_efficiency_stop->setEnabled(true);


    double truePeriod = scalerModule->getTruePulsePeriod();

    qDebug() << "N repetition= " << ui->spinBox_efficiency_scaler_repeat->value();
    truePeriod *= ui->spinBox_efficiency_scaler_repeat->value();

    int hvMax = ui->spinBox_efficiency_hvmax->value();
    int hvMin = ui->spinBox_efficiency_hvmin->value();
    int step = ui->spinBox_efficiency_step->value();
    int nPoints = 1 + (hvMax - hvMin) / step;



    int channel = ui->spinBox_efficiency_channel->value();

    for (int i = 0; i < nPoints; i++) {
        ui->progressBar_efficiency->setValue((int)(100*i*1.0/nPoints));
        scalerModule->resetChannels();

        updateHV(channel, hvMin + i * step);
        usleep(500000);
        scalerModule->startPulser();

        long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        long t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

        qDebug() << "Starting while loop ... ";
        while (t < now + truePeriod) {

            t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() ;
            QCoreApplication::processEvents();
            if (stopRun) break;
        }
        if (stopRun) break;

        qDebug() << "Waiting loop finished";
        uint32_t num = scalerModule->readChannel(0);
        uint32_t den = scalerModule->readChannel(1);
        qDebug() << "Finishing reading: " << num << "  /  " << den ;

        grEfficiency->addEfficiencyPoint(hvMin + i*step, num, den);

    }
    qDebug() << "Finishing efficiency measurements";
    stopRun = false;
    ui->progressBar_efficiency->setValue(100);

    ui->pushButton_efficiency_stop->setEnabled(false);
    ui->pushButton_efficiency_start->setEnabled(true);

}

void Efficiency::on_pushButton_efficiency_stop_clicked()
{
    stopRun = true;
}

void Efficiency::updateHV(int channel, int hv)
{
    hvModule->setVoltage(channel, hv);
    usleep(500000);
    int currentHV = hvModule->getVoltage(channel);
    while (currentHV < hv*0.99 || currentHV > hv*1.01) {
        qDebug() << "Current HV = " << currentHV << ", asked = " << hv;
        currentHV = hvModule->getVoltage(channel);
        QCoreApplication::processEvents();
        usleep(100000);
        if (stopRun) break;
    }
}
