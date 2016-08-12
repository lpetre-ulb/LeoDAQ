#ifndef SCALER_H
#define SCALER_H

#include <QWidget>

namespace Ui {
class scaler;
}

class scaler : public QWidget
{
    Q_OBJECT

public:
    explicit scaler(QWidget *parent = 0, int handleChef_ = 0);
    ~scaler();

private:
    Ui::scaler *ui;
};

#endif // SCALER_H
