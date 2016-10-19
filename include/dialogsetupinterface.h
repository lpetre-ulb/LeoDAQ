#ifndef DIALOGSETUPINTERFACE_H
#define DIALOGSETUPINTERFACE_H

#include <QDialog>
#include <QVector>
#include <QString>

namespace Ui {
class DialogSetupInterface;
}

class DialogSetupInterface : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupInterface(QWidget *parent = 0);
    ~DialogSetupInterface();
    QVector<QString> getConfig();


private slots:
    void on_pushButton_done_clicked();

    void on_pushButton_cancel_clicked();

    void on_checkBox_efficiency_tab_stateChanged(int arg1);

private:
    Ui::DialogSetupInterface *ui;
    QVector<QString> config;
};

#endif // DIALOGSETUPINTERFACE_H
