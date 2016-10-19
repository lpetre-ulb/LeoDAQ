#include "qdc.h"
#include "ui_qdc.h"
#include <chrono>

qdc::qdc(QWidget *parent, int32_t handleChef_) :
    QWidget(parent),
    ui(new Ui::qdc)
{
    ui->setupUi(this);

    module = new QDCModule(handleChef_);
    updateBaseAddress(ui->spinBox_qdc_rotary_switches->value());



    hQDC = new histogram(ui->qdc_profile, "Integral charge profile", "charge (pC)", "nEvents");
    hQDC->adjustPlot(ui->spinBox_qdc_nbins->value(), ui->doubleSpinBox_qdc_vmin->value(), ui->doubleSpinBox_qdc_vmax->value());

    nExp = ui->spinBox_qdc_nevents->value();

    ui->pushButton_stopprofile->setEnabled(false);
    for (int i = 0; i < module->getNChannels(); i++) module->ConfigureChannel(i, false, 0);

}

qdc::~qdc()
{
    delete ui;
}

void qdc::updateBaseAddress(int rotSw)
{
    // setBaseAddress updates and tries to access some registers
    // to check if connection is possible
    QString moduleType = module->setBaseAddress(rotSw);
    ui->lineEdit_qdc_module_type->setText(moduleType);

    enabledQDCUi(moduleType != "UNKNOWN");


}

void qdc::enabledQDCUi(bool enable)
{
    ui->widget_qdc_display->setEnabled(enable);
    if (enable) ui->spinBox_qdc_channelN->setMaximum(module->getNChannels()-1);
    //ui->pushButton_qdc_poisson->setEnabled(enable);
}

void qdc::on_pushButton_clicked()
{
    int channelN = ui->spinBox_qdc_channelN->value();
    bool enable = ui->radioButton_qdc_enable->isChecked();
    int threshold = ui->spinBox_qdc_threshold->value();
    qDebug() << threshold;

    if(threshold < 0){
        QMessageBox::information(this,
                                 "Error!",
                                 "Invalid threshold!",
                                 QMessageBox::Ok);
        return;
    }

    else if(threshold < 16 ){
        QMessageBox::information(this,
                                 "Warning!",
                                 "The threshold value below 16 is treated equivalently as 0!",
                                 QMessageBox::Ok);
    }

    else if((threshold % 16) != 0){
        QMessageBox::information(this,
                                 "Warning!",
                                 "The threshold value is not the multiple of 16, and it is treated equivalently as the adjacent smaller value that is the multiple of 16!",
                                 QMessageBox::Ok);
    }

    if (channelN == -1) {
        for (int i = 0; i < ui->spinBox_qdc_channelN->maximum()+1; i++)
            module->ConfigureChannel(i, enable, threshold);
    }
    else {
        module->ConfigureChannel(channelN, enable, threshold);
    }

}

void qdc::on_spinBox_qdc_channelN_valueChanged(int channelN)
{
    if (channelN == -1) return;
    bool enable = module->ReadChannelStatus(channelN);
    int threshold = module->ReadChannelThreshold(channelN);
    qDebug() << enable;
    ui->radioButton_qdc_enable->setChecked(enable);
    ui->spinBox_qdc_threshold->setValue(threshold);
}


void qdc::on_pushButton_clearChannel_clicked()
{
    module->ClearChannels();
}

void qdc::on_spinBox_qdc_nbins_valueChanged(int nBins)
{
    hQDC->adjustPlot(nBins, ui->doubleSpinBox_qdc_vmin->value(), ui->doubleSpinBox_qdc_vmax->value());
}

void qdc::on_doubleSpinBox_qdc_vmin_valueChanged(double vMin)
{
    double vMax = ui->doubleSpinBox_qdc_vmax->value();
    if(vMax <= vMin) return;
    hQDC->adjustPlot(ui->spinBox_qdc_nbins->value(), vMin, vMax);
}

void qdc::on_doubleSpinBox_qdc_vmax_valueChanged(double vMax)
{
    double vMin = ui->doubleSpinBox_qdc_vmin->value();
    if(vMax <= vMin) return;
    hQDC->adjustPlot(ui->spinBox_qdc_nbins->value(), vMin, vMax);
}

void qdc::on_pushButton_qdc_startprofile_clicked()
{
    nExp = ui->spinBox_qdc_nevents->value();
    ui->spinBox_qdc_nevents->setEnabled(false);
    ui->pushButton_clearChannel->setEnabled(false);
    ui->pushButton_clearProfile->setEnabled(false);
    ui->pushButton_qdc_startprofile->setEnabled(false);
    ui->pushButton_stopprofile->setEnabled(true);

    ui->spinBox_qdc_nbins->setEnabled(false);
    ui->doubleSpinBox_qdc_vmin->setEnabled(false);
    ui->doubleSpinBox_qdc_vmax->setEnabled(false);


    QString fileName = ui->lineEdit_qdc_file_name->text();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
        qDebug() << "Problem opening or creating the file:";
        qDebug() << fileName;
    }
    else {
          QTextStream stream(&file);
          for(int i=0; i<nExp; i++){ // the stop will be implemented in the next function
              bool validData = false;
              uint32_t value32 = 0;

              do {
                  module->ReadChannelData(0, &value32, &validData);
                  QCoreApplication::processEvents();
              } while(!validData);

              hQDC->updatePlot(value32/10.0);
              stream << QString::number(value32/10.0) << "\n";
          }
    }
    file.close();

    ui->spinBox_qdc_nevents->setEnabled(true);
    ui->pushButton_clearChannel->setEnabled(true);
    ui->pushButton_clearProfile->setEnabled(true);
    ui->pushButton_qdc_startprofile->setEnabled(true);
    ui->pushButton_stopprofile->setEnabled(false);

    ui->spinBox_qdc_nbins->setEnabled(true);
    ui->doubleSpinBox_qdc_vmin->setEnabled(true);
    ui->doubleSpinBox_qdc_vmax->setEnabled(true);

}

void qdc::on_pushButton_stopprofile_clicked()
{
    nExp = 0;
    ui->spinBox_qdc_nevents->setEnabled(true);
    ui->pushButton_clearChannel->setEnabled(true);
    ui->pushButton_clearProfile->setEnabled(true);
    ui->pushButton_qdc_startprofile->setEnabled(true);
    ui->pushButton_stopprofile->setEnabled(false);

    ui->spinBox_qdc_nbins->setEnabled(true);
    ui->doubleSpinBox_qdc_vmin->setEnabled(true);
    ui->doubleSpinBox_qdc_vmax->setEnabled(true);
}

void qdc::on_pushButton_clearProfile_clicked()
{
    QMessageBox::StandardButton clrbtn = QMessageBox::question(
                this,
                "Clear Graph",
                tr("Warning: Are you sure that you would like to clear the graph? \n Otherwise you will lose the recorded profile!"),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes
    );

    if(clrbtn == QMessageBox::Yes){
        hQDC->adjustPlot(ui->spinBox_qdc_nbins->value(), ui->doubleSpinBox_qdc_vmin->value(), ui->doubleSpinBox_qdc_vmax->value());
    }
}

void qdc::on_spinBox_qdc_rotary_switches_valueChanged(int rotSw)
{
    updateBaseAddress(rotSw);
}

void qdc::setFileName()
{
     //get current date
     QDate date = QDate::currentDate();
     QString dateString = date.toString();
     qDebug() << dateString.replace(" ", "_");

     //get current time
     QTime time = QTime::currentTime();
     QString timeString = time.toString();
     qDebug() << timeString.replace(":", "_");

     QString fileNameTmp = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
     fileNameTmp += "Data_";
     fileNameTmp += dateString;
     fileNameTmp += "_at_";
     fileNameTmp += timeString;
     fileNameTmp += ".txt";
     qDebug() << "This is the location: ";
     qDebug() << fileNameTmp;
     ui->lineEdit_qdc_file_name->setText(fileNameTmp);

}

void qdc::on_pushButton_qdc_file_name_clicked()
{
    QString fileName = ui->lineEdit_qdc_file_name->text();
    QString newFileName = QFileDialog::getSaveFileName(this, tr("Save File"), QStandardPaths::locate(QStandardPaths::DocumentsLocation,
                                       QString(), QStandardPaths::LocateDirectory));

    ui->lineEdit_qdc_file_name->setText(newFileName == "" ? fileName : newFileName);
}
