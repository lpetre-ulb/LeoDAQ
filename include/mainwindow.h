#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QVector>
#include <QString>

#include "hv.h"
#include "scaler.h"
#include "tdc.h"
#include "efficiency.h"
#include "qdc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closing();
    void closeEvent (QCloseEvent *event);
    void connectToVMECrate();
    QVector<QString> setUpInterface();



private slots:
    void on_tabWidget_currentChanged();

private:
    Ui::MainWindow *ui;

    // handle to VME Crate
    int32_t handleChef;

    // tab widgets
    hv* hvWidget;
    scaler* scalerWidget;
    tdc* tdcWidget;
    Efficiency* efficiencyWidget;
    qdc* qdcWidget;
};

#endif // MAINWINDOW_H
