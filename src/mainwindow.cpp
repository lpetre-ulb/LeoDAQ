#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // configure global view
    this->setWindowTitle("Muon DAQ");
    this->setCentralWidget(ui->tabWidget);
    this->setFixedSize(this->size());


    hvWidget = new hv(this, 1234);
    scalerWidget = new scaler(this, 1234);
    tdcWidget = new tdc(this, 1234);

    // add the tabs for the program
    ui->tabWidget->addTab(hvWidget, "HV");
    ui->tabWidget->addTab(scalerWidget, "Scaler");
    ui->tabWidget->addTab(tdcWidget, "TDC");

    // set the starting tab index
    ui->tabWidget->setCurrentIndex(0); // HV



}

MainWindow::~MainWindow()
{
    delete hvWidget;
    delete scalerWidget;
    delete tdcWidget;
    delete ui;
}

void MainWindow::closing()
{
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

    if (resBtn == QMessageBox::Yes) closing();
}
