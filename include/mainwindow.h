#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

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

    void closing();
    void closeEvent (QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    // tab widgets
    hv* hvWidget;
    scaler* scalerWidget;
    tdc* tdcWidget;
};

#endif // MAINWINDOW_H
