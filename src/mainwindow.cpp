#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <CAENVMElib.h>
#include <dialogsetupinterface.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    hvWidget(nullptr),
    scalerWidget(nullptr),
    tdcWidget(nullptr),
    efficiencyWidget(nullptr),
    qdcWidget(nullptr)
{
    ui->setupUi(this);

    // configure global view
    this->setWindowTitle("IIHE DAQ - version 1.1 (2017)");
    this->setCentralWidget(ui->tabWidget);
    this->setFixedSize(this->size());
    //this->setStyleSheet("background-color:white;");
    ui->tabWidget->setStyleSheet("QTabWidget {background-color:white;}");
    //this->setStyleSheet("QMessageBox {background-color: white;}");

    QVector<QString> config = setUpInterface();
    if (config.isEmpty()) {
        throw std::runtime_error("Cancelation or no configuration.");
    }
    else {

        connectToVMECrate();

        if (config.indexOf("hvWidget") > -1) hvWidget = new hv(this, handleChef);
        if (config.indexOf("scalerWidget") > -1) scalerWidget = new scaler(this, handleChef);
        if (config.indexOf("tdcWidget") > -1) tdcWidget = new tdc(this, handleChef);
        if (config.indexOf("efficiencyWidget") > -1) efficiencyWidget = new Efficiency(this, handleChef);
        if (config.indexOf("qdcWidget") > -1) qdcWidget = new qdc(this, handleChef);

        // add the tabs for the program
        if (hvWidget) ui->tabWidget->addTab(hvWidget, "HV");
        if (scalerWidget) ui->tabWidget->addTab(scalerWidget, "Scaler");
        if (tdcWidget) ui->tabWidget->addTab(tdcWidget, "TDC");
        if (efficiencyWidget) ui->tabWidget->addTab(efficiencyWidget, "Efficiency");
        if (qdcWidget) ui->tabWidget->addTab(qdcWidget, "QDC");

        // set the starting tab index
        ui->tabWidget->setCurrentIndex(0); // HV
    }

}


QVector<QString> MainWindow::setUpInterface()
{

    DialogSetupInterface* setUp = new DialogSetupInterface();

    return setUp->getConfig();


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closing()
{
    qDebug() << "Still good";
    if (tdcWidget && tdcWidget->stillRunning()) tdcWidget->stopRun();
    CAENVME_End(handleChef);


    qApp->quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(
        this,
        this->windowTitle(),
        tr("Are you sure you want to exit ?\n"),
        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes
    );

    if (resBtn == QMessageBox::Yes) {
        event->accept();
        closing();
    }
    else {
        event->ignore();
    }
}

void MainWindow::connectToVMECrate()
{
    while (CAENVME_Init(cvV1718, 0, 0, &handleChef) != cvSuccess) {

        QMessageBox::StandardButton resBtn = QMessageBox::question(0,
            "Error",
            tr("Impossible to connect to VME crate!\nPlease, check that the usb cable is connected. If it is connected, check if it is seen in /dev/usb/\nTry again ?"),
            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
            QMessageBox::Yes
        );
        if (resBtn != QMessageBox::Yes) {
            throw std::runtime_error("Failed to connect to VME crate");
        }
    }
}


void MainWindow::on_tabWidget_currentChanged()
{
    if (ui->tabWidget->currentWidget()->objectName() != "hv") {
        if (hvWidget) hvWidget->pauseTimer();
    }
    else {
        if (hvWidget) hvWidget->restartTimer();
    }
    if (ui->tabWidget->currentWidget()->objectName() != "Efficiency") {
        if (efficiencyWidget) efficiencyWidget->pauseTimer();
    }
    else {
        if (efficiencyWidget) efficiencyWidget->restartTimer();
    }
}

