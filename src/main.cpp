#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QMessageBox {background-color:white}");
    a.setStyleSheet("QDialog {background-color:white}");
    // runtime error may be thrown if the connection
    // with the vme crate is not possible
    try {
        MainWindow w;
        w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
        w.show();

        return a.exec();
    }
    catch (const std::runtime_error& error) {
        qDebug() << error.what();
        return 0;
    }
}
