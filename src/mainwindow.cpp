#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(&hvWidget, "HV");
    ui->tabWidget->addTab(&scalerWidget, "Scaler");
    ui->tabWidget->addTab(&tdcWidget, "TDC");
}

MainWindow::~MainWindow()
{
    delete ui;
}
