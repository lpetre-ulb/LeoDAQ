#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hv.h"
#include "scaler.h"
#include "tdc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // tab widgets
    hv hvWidget;
    scaler scalerWidget;
    tdc tdcWidget;
};

#endif // MAINWINDOW_H
