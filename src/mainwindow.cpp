#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <CAENVMElib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // configure global view
    this->setWindowTitle("Muon DAQ");
    this->setCentralWidget(ui->tabWidget);
    this->setFixedSize(this->size());


    connectToVMECrate();

    hvWidget = new hv(this, handleChef);
    scalerWidget = new scaler(this, handleChef);
    tdcWidget = new tdc(this, handleChef);

    // add the tabs for the program
    ui->tabWidget->addTab(hvWidget, "HV");
    ui->tabWidget->addTab(scalerWidget, "Scaler");
    ui->tabWidget->addTab(tdcWidget, "TDC");

    // set the starting tab index
    ui->tabWidget->setCurrentIndex(0); // HV


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closing()
{
    qDebug() << "Still good";
    if (tdcWidget->stillRunning()) tdcWidget->stopRun();
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
        QMessageBox::StandardButton resBtn = QMessageBox::question(
            this,
            "Error",
            tr("Impossible to connect to VME crate!\nPlease, check that the usb cable is connected. If it is connected, check if it is seen in /etc/usb/\nTry again ?"),
            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
            QMessageBox::Yes
        );

        if (resBtn != QMessageBox::Yes) {
            throw std::runtime_error("Failed to connect to VME crate");
        }
    }
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (ui->tabWidget->currentWidget()->objectName() != "hv") {
        hvWidget->pauseTimer();
    }
    else {
        hvWidget->restartTimer();
    }
}
