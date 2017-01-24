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

    void on_checkBox_efficiency_tab_stateChanged();

    void on_checkBox_muon_life_clicked();

    void on_checkBox_cosmic_muon_clicked();

    void on_checkBox_cherenkov_muon_clicked();

    void on_checkBox_cherenkov_electron_clicked();

private:
    Ui::DialogSetupInterface *ui;
    QVector<QString> config;
};

#endif // DIALOGSETUPINTERFACE_H
