#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // runtime error may be thrown if the connection
    // with the vme crate is not possible
    try {
        MainWindow w;
        w.show();

        return a.exec();
    }
    catch (const std::runtime_error& error) {
        qDebug() << error.what();
        return 0;
    }
}
