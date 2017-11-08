#include "tdc.h"
#include "ui_tdc.h"

tdc::tdc(QWidget *parent, int handleChef_) :
    QWidget(parent), ui(new Ui::tdc), isRunning(false)
{
    ui->setupUi(this);


    // change cosmetics
    makeItNice();
    ui->pushButton_read_config->setVisible(false);
    ui->pushButton_set_offset->setVisible(false);
    ui->pushButton_set_width->setVisible(false);
    ui->pushButton_set_trigger->setVisible(false);
    ui->pushButton_set_cont->setVisible(false);
    ui->pushButton_tdc_file_name->setVisible(false);
    ui->lineEdit_tdc_file_name->setEnabled(false);


    // build the corresponding module
    module = new TDCModule(handleChef_);
    updateBaseAddress(ui->spinBox_tdc_rotary_switches->value());

    setFileName();

    // create the histogram
    hTDC = new histogram(ui->qcp_tdc, "TDC Delta t", "time (ns)", "# events");
    hTDC->adjustPlot(ui->spinBox_tdc_nbins->value(), ui->doubleSpinBox_tdc_tmin->value(), ui->doubleSpinBox_tdc_tmax->value());
}

tdc::~tdc()
{
    delete module;
    delete hTDC;
    delete ui;
}

void tdc::updateBaseAddress(int rotSw)
{
    // setBaseAddress updates and tries to access some registers
    // to check if connection is possible
    QString fwRev = module->setBaseAddress(rotSw);
    ui->lineEdit_tdc_firwmare_revision->setText(fwRev);

    enabledTDCUi(fwRev != "UNKNOWN");
}

void tdc::enabledTDCUi(bool enable)
{
    isEnabledTDCUi = enable;
    ui->pushButton_tdc_clear->setEnabled(enable);
    ui->pushButton_tdc_clear_tdcs->setEnabled(enable);
    ui->pushButton_tdc_file_name->setEnabled(enable);
    ui->pushButton_tdc_start_run->setEnabled(enable && !isRunning);
    ui->pushButton_tdc_stop_run->setEnabled(enable && isRunning);
    ui->widget_tdc_channel->setEnabled(enable);
    ui->widget_tdc_trigger->setEnabled(enable);
    ui->widget_tdc_histogram_config->setEnabled(enable);
    ui->widget_tdc_lifetime_statistics->setEnabled(enable);
    ui->widget_tdc_event_statistics->setEnabled(enable);
}

void tdc::on_spinBox_tdc_rotary_switches_valueChanged(int rotSw)
{
    updateBaseAddress(rotSw);
}

void tdc::on_pushButton_tdc_clear_tdcs_clicked()
{
    // software clear
    module->clear();
}

void tdc::on_pushButton_tdc_start_run_clicked()
{
    uint16_t tdcWindowWidth = ui->spinBox_tdc_window_width->value();
    int32_t tdcWindowOffset = ui->spinBox_tdc_window_offset->value();

    // We try to set window parameters
    TDCModule::Errors ret = module->setTriggerMode(tdcWindowWidth, tdcWindowOffset);

    if (ret == TDCModule::WRONG_CONDITION_ON_DELAYED_TRIGGER) {
        QMessageBox::critical(
            this,
            "Bad TDC Trigger Config",
            "When the whole matching window straddling the trigger or delayed with respect to the trigger, the following condition must be fulfilled:\nMatch window width + window offset ≤ 40 clock cycles = 1000 ns",
            QMessageBox::Ok
        );
        return;
    }
    else if (ret != TDCModule::NO_ERROR) {
        return;
    }


    isRunning = true;
    ui->pushButton_tdc_start_run->setEnabled(!isRunning);
    ui->pushButton_tdc_stop_run->setEnabled(isRunning);
    ui->widget_tdc_trigger->setEnabled(!isRunning);
    ui->widget_tdc_histogram_config->setEnabled(!isRunning);

    setFileName();

    QString fileName = ui->lineEdit_tdc_file_name->text();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
        qDebug() << "Problem opening or creating the file:";
        qDebug() << fileName;
    }
    else {
        QTextStream stream(&file);

        // set channel
        module->setStartStopChannels(ui->spinBox_tdc_start->value(), ui->spinBox_tdc_stop->value());

        // reading data
        resetCounters();

        while (isRunning) {
            QCoreApplication::processEvents();

            std::vector<long> values;
            module->readEvents(&values);
            int nEvents = values.size();
            //if (nEvents > 0) qDebug() << "Number of events: " << nEvents;
            for (int i = 0; i < nEvents; ++i) {
                updateStatUiAndPlot(values[i]/1000.0);
                stream << QString::number(values[i]/1000.0) << "\n";
            }

        }
    }
    file.close();
    qDebug() << "Finish run";
    isRunning = false;

    ui->pushButton_tdc_start_run->setEnabled(!isRunning);
    ui->pushButton_tdc_stop_run->setEnabled(isRunning);
    ui->widget_tdc_trigger->setEnabled(!isRunning);
    ui->widget_tdc_histogram_config->setEnabled(!isRunning);



}


void tdc::updateStatUiAndPlot(double difference)
{
    if (difference <= hTDC->getXMax() && difference >= hTDC->getXMin()) {
        // total number of events
        counter++;
        ui->lcdNumber_tdc_total->display(counter);

        // rate estimate
        QDateTime currentTime = QDateTime::currentDateTime();
        long long timeFromBeginingOfRun = time.msecsTo(currentTime);

        double rate = counter * 1000.0 / timeFromBeginingOfRun;

        //ui->lcdNumber_tdc_rate->display((long)((counter*1000.0/(timeFromBeginingOfRun))*1000)/1000.0);
        ui->lcdNumber_tdc_rate->display(rate);

        // mean arrival estimate
        long long arrivalTime = prevTime.msecsTo(currentTime);
        mean_arrival *= (counter-1);
        mean_arrival_square *= (counter-1);
        mean_arrival += arrivalTime;
        mean_arrival_square += arrivalTime*arrivalTime;
        mean_arrival /= counter;
        mean_arrival_square /= counter;

        double mean_arrival_double = mean_arrival / 1000.0;
        ui->lcdNumber_tdc_mean_arrival->display(mean_arrival_double);
        double rms_arrival_double = sqrt(mean_arrival_square - mean_arrival*mean_arrival) / 1000.0;
        ui->lcdNumber_tdc_rms_arrival->display(rms_arrival_double);

        // lifetime
        mean_lifetime *= (counter - 1);
        mean_lifetime_square *= (counter - 1);
        mean_lifetime += (difference / 1000.0);
        mean_lifetime_square += (difference / 1000.0) * (difference / 1000.0);
        mean_lifetime /= counter;
        mean_lifetime_square /= counter;
        ui->lcdNumber_tdc_mean_lifetime->display((long)(mean_lifetime*1000)/1000.0);
        ui->lcdNumber_tdc_rms_lifetime->display((long)(sqrt(mean_lifetime_square - mean_lifetime*mean_lifetime)*1000)/1000.0);
        ui->lcdNumber_tdc_last_lifetime->display((long)(difference)/1000.0);

        // plot
        hTDC->updatePlot(difference);
        prevTime = currentTime;
    }

}

void tdc::resetCounters()
{
    time = QDateTime::currentDateTime();
    counter = 0;
    prevTime = time;
    mean_arrival = 0;
    mean_arrival_square = 0;
    mean_lifetime = 0;
    mean_lifetime_square = 0;

}

void tdc::on_pushButton_tdc_stop_run_clicked()
{
    if (isRunning) isRunning = false;
}

void tdc::makeItNice()
{
    ui->lcdNumber_tdc_last_lifetime->setPalette(Qt::black);
    ui->lcdNumber_tdc_mean_arrival->setPalette(Qt::black);
    ui->lcdNumber_tdc_mean_lifetime->setPalette(Qt::black);
    ui->lcdNumber_tdc_rate->setPalette(Qt::black);;
    ui->lcdNumber_tdc_rms_arrival->setPalette(Qt::black);
    ui->lcdNumber_tdc_rms_lifetime->setPalette(Qt::black);
    ui->lcdNumber_tdc_total->setPalette(Qt::black);
}

void tdc::on_spinBox_tdc_nbins_valueChanged(int nBins)
{
    hTDC->adjustPlot(nBins, ui->doubleSpinBox_tdc_tmin->value(), ui->doubleSpinBox_tdc_tmax->value());
}

void tdc::on_doubleSpinBox_tdc_tmin_valueChanged(double tMin)
{
    double tMax = ui->doubleSpinBox_tdc_tmax->value();
    if (tMin >= tMax) return;
    hTDC->adjustPlot(ui->spinBox_tdc_nbins->value(), tMin, tMax);
}

void tdc::on_doubleSpinBox_tdc_tmax_valueChanged(double tMax)
{
    double tMin = ui->doubleSpinBox_tdc_tmin->value();
    if (tMax <= tMin) return;
    hTDC->adjustPlot(ui->spinBox_tdc_nbins->value(), tMin, tMax);
}

void tdc::on_pushButton_tdc_log_clicked()
{
    bool isLogY = hTDC->setLogY();

    if (isLogY) {
        ui->pushButton_tdc_log->setText("Linear Scale");
    }
    else {
        ui->pushButton_tdc_log->setText("Log Scale");
    }
}

void tdc::on_pushButton_tdc_clear_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Clear data",
        "Are you sure you want to clear the data ?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";

        clearStatUi();
        resetCounters();

        hTDC->adjustPlot(ui->spinBox_tdc_nbins->value(), ui->doubleSpinBox_tdc_tmin->value(), ui->doubleSpinBox_tdc_tmax->value());
    }
    else {
        qDebug() << "Yes was *not* clicked";
        return;
    }

}

bool tdc::stillRunning()
{
    return isRunning;
}

void tdc::stopRun()
{
    on_pushButton_tdc_stop_run_clicked();
}

void tdc::clearStatUi()
{
    ui->lcdNumber_tdc_total->display(0);
    ui->lcdNumber_tdc_rate->display(0);
    ui->lcdNumber_tdc_rms_arrival->display(0);
    ui->lcdNumber_tdc_mean_arrival->display(0);
    ui->lcdNumber_tdc_last_lifetime->display(0);
    ui->lcdNumber_tdc_rms_lifetime->display(0);
    ui->lcdNumber_tdc_mean_lifetime->display(0);
}

void tdc::on_spinBox_tdc_start_valueChanged(int startChannel)
{
    module->setStartStopChannels(startChannel, ui->spinBox_tdc_stop->value());
}

void tdc::on_spinBox_tdc_stop_valueChanged(int stopChannel)
{
    module->setStartStopChannels(ui->spinBox_tdc_start->value(), stopChannel);
}

void tdc::setFileName()
{

     //get current time
     QDateTime time = QDateTime::currentDateTime();
     QString timeString = time.toString();
     qDebug() << timeString.replace(":", "_");

     QString fileNameTmp = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
     fileNameTmp += "Data_";
     fileNameTmp += timeString;
     fileNameTmp += ".txt";
     qDebug() << "This is the location: ";
     qDebug() << fileNameTmp;
     ui->lineEdit_tdc_file_name->setText(fileNameTmp);

}

void tdc::on_pushButton_tdc_file_name_clicked()
{
    QString fileName = ui->lineEdit_tdc_file_name->text();
    QString newFileName = QFileDialog::getSaveFileName(this, tr("Save File"), QStandardPaths::locate(QStandardPaths::DocumentsLocation,
                                       QString(), QStandardPaths::LocateDirectory));

    ui->lineEdit_tdc_file_name->setText(newFileName == "" ? fileName : newFileName);

}

void tdc::on_pushButton_set_width_clicked()
{
    module->setWindowWidth(ui->spinBox_tdc_window_width->value());
}


void tdc::on_pushButton_read_config_clicked()
{
    module->readConfig();
    module->readAcqMode();
}

void tdc::on_pushButton_set_trigger_clicked()
{
    module->setTriggerAcquisitionMode();
}

void tdc::on_pushButton_set_cont_clicked()
{
    module->setContinuousAcquisitionMode();
}

void tdc::on_pushButton_set_offset_clicked()
{
    module->setWindowOffset(ui->spinBox_tdc_window_offset->value());
}

void tdc::on_pushButton_save_plot_clicked()
{
    QString plotFileName = ui->lineEdit_tdc_file_name->text();
    int dotPosition = plotFileName.lastIndexOf(".");
    if (dotPosition < 0) {
        qDebug() << "No extension in fileName";
    }
    else {

        plotFileName = plotFileName.left(dotPosition) + ".pdf";
        qDebug() << "Plot file name: " << plotFileName;
    }
    ui->qcp_tdc->savePdf(plotFileName);
}
