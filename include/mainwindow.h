#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

#include "hv.h"
#include "scaler.h"
#include "tdc.h"
#include "efficiency.h"

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


private slots:
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    // handle to VME Crate
    int32_t handleChef;

    // tab widgets
    hv* hvWidget;
    scaler* scalerWidget;
    tdc* tdcWidget;
    Efficiency* efficiencyWidget;
};

#endif // MAINWINDOW_H
