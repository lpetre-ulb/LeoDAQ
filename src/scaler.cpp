#include "scaler.h"
#include "ui_scaler.h"
#include <unistd.h>
#include <chrono>


scaler::scaler(QWidget *parent, int handleChef_) :
    QWidget(parent),
    ui(new Ui::scaler)
{
    ui->setupUi(this);
    makeItNice();

    // build the corresponding module
    module = new ScalerModule(handleChef_);
    updateBaseAddress(ui->spinBox_scaler_rotary_switches->value());
    ui->comboBox_scaler_unit->setCurrentIndex(3);
    setPulserMode(ui->spinBox_scaler_width->value(), ui->comboBox_scaler_unit->currentIndex());

    hScaler = new histogram(ui->qcp_scaler, "Poisson Distribution", "count", "# events");
    hScaler->adjustPlot(ui->spinBox_scaler_nbins->value(), ui->doubleSpinBox_scaler_cmin->value(), ui->doubleSpinBox_scaler_cmax->value());

    nExp = ui->spinBox_scaler_nevents->value();
}

scaler::~scaler()
{
    delete ui;
}

void scaler::updateBaseAddress(int rotSw)
{
    // setBaseAddress updates and tries to access some registers
    // to check if connection is possible
    QString moduleType = module->setBaseAddress(rotSw);
    ui->lineEdit_scaler_module_type->setText(moduleType);

    enabledScalerUi(moduleType != "UNKNOWN");
    on_pushButton_scaler_update_clicked();

}

void scaler::enabledScalerUi(bool enable)
{
    ui->widget_scaler_display->setEnabled(enable);
    ui->pushButton_scaler_poisson->setEnabled(enable);
}

void scaler::setPulserMode(unsigned char width, int unitIndex, int repeat)
{
    module->setPulserMode(width, CVTimeUnits(unitIndex), repeat);
}

void scaler::on_doubleSpinBox_scaler_cmax_valueChanged(double cMax)
{
    double cMin = ui->doubleSpinBox_scaler_cmin->value();
    if (cMax <= cMin) return;
    hScaler->adjustPlot(ui->spinBox_scaler_nbins->value(), cMin, cMax);
}

void scaler::on_doubleSpinBox_scaler_cmin_valueChanged(double cMin)
{
    double cMax = ui->doubleSpinBox_scaler_cmax->value();
    if (cMax <= cMin) return;
    hScaler->adjustPlot(ui->spinBox_scaler_nbins->value(), cMin, cMax);
}

void scaler::on_spinBox_scaler_nbins_valueChanged(int nBins)
{
    hScaler->adjustPlot(nBins, ui->doubleSpinBox_scaler_cmin->value(), ui->doubleSpinBox_scaler_cmax->value());

}

void scaler::on_pushButton_scaler_pulse_clicked()
{
    ui->pushButton_scaler_pulse->setEnabled(false);
    ui->spinBox_scaler_width->setEnabled(false);
    ui->comboBox_scaler_unit->setEnabled(false);

    double truePeriod = module->getTruePulsePeriod();
    module->startPulser();



    long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    long t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();


    while (t < now + truePeriod) {
        t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() ;
        on_pushButton_scaler_update_clicked();
        ui->progressBar_scaler_pulse->setValue((int)(100*(t-now)/(truePeriod)));
        QCoreApplication::processEvents();

    }
    ui->progressBar_scaler_pulse->setValue(100);

    ui->pushButton_scaler_pulse->setEnabled(true);
    ui->spinBox_scaler_width->setEnabled(true);
    ui->comboBox_scaler_unit->setEnabled(true);
}

void scaler::on_spinBox_scaler_width_valueChanged(int width)
{
    setPulserMode(width, CVTimeUnits(ui->comboBox_scaler_unit->currentIndex()));
}

void scaler::on_comboBox_scaler_unit_currentIndexChanged(int unitIndex)
{
    setPulserMode(ui->spinBox_scaler_width->value(), CVTimeUnits(unitIndex));
}


void scaler::on_pushButton_scaler_update_clicked()
{
    std::vector<int> values(16);
    module->readChannels(&values);
    ui->lcdNumber_scaler_ch_0->display(values[0]);
    ui->lcdNumber_scaler_ch_1->display(values[1]);
    ui->lcdNumber_scaler_ch_2->display(values[2]);
    ui->lcdNumber_scaler_ch_3->display(values[3]);
    ui->lcdNumber_scaler_ch_4->display(values[4]);
    ui->lcdNumber_scaler_ch_5->display(values[5]);
    ui->lcdNumber_scaler_ch_6->display(values[6]);
    ui->lcdNumber_scaler_ch_7->display(values[7]);
    ui->lcdNumber_scaler_ch_8->display(values[8]);
    ui->lcdNumber_scaler_ch_9->display(values[9]);
    ui->lcdNumber_scaler_ch_10->display(values[10]);
    ui->lcdNumber_scaler_ch_11->display(values[11]);
    ui->lcdNumber_scaler_ch_12->display(values[12]);
    ui->lcdNumber_scaler_ch_13->display(values[13]);
    ui->lcdNumber_scaler_ch_14->display(values[14]);
    ui->lcdNumber_scaler_ch_15->display(values[15]);
}

void scaler::makeItNice()
{
    ui->lcdNumber_scaler_ch_0->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_1->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_2->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_3->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_4->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_5->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_6->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_7->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_8->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_9->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_10->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_11->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_12->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_13->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_14->setPalette(Qt::black);
    ui->lcdNumber_scaler_ch_15->setPalette(Qt::black);

}

void scaler::on_pushButton_scaler_reset_clicked()
{

    module->resetChannels();
    on_pushButton_scaler_update_clicked();

    ui->progressBar_scaler_pulse->setValue(0);

}

void scaler::on_spinBox_scaler_rotary_switches_valueChanged(int rotSw)
{
    updateBaseAddress(rotSw);
}

void scaler::on_pushButton_scaler_poisson_clicked()
{
    ui->pushButton_scaler_poisson->setEnabled(false);
    ui->pushButton_sclaer_stop->setEnabled(true);

    double truePeriod = module->getTruePulsePeriod();

    nExp = ui->spinBox_scaler_nevents->value();
    for (int i = 0; i < nExp; i++) {
        ui->progressBar_scaler_pulse->setValue((int)(100*i*1.0/nExp));
        module->resetChannels();

        usleep(200000);
        module->startPulser();

        long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        long t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

        qDebug() << "Starting while loop ... ";
        while (t < now + truePeriod) {

            t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() ;
            QCoreApplication::processEvents();
        }

        qDebug() << "Waiting loop finished";
        uint32_t value32 = module->readChannel(0);
        qDebug() << "Finishing reading: " << value32;

        ui->lcdNumber_scaler_ch_0->display((int)value32);

        hScaler->updatePlot((int)value32);

    }

    ui->progressBar_scaler_pulse->setValue(100);
    ui->pushButton_scaler_poisson->setEnabled(true);
    ui->pushButton_sclaer_stop->setEnabled(false);


}

void scaler::on_pushButton_sclaer_stop_clicked()
{
    nExp = 0;
}

void scaler::on_pushButton_scaler_clear_clicked()
{
    hScaler->adjustPlot(ui->spinBox_scaler_nbins->value(), ui->doubleSpinBox_scaler_cmin->value(), ui->doubleSpinBox_scaler_cmax->value());
}
