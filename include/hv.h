#ifndef HV_H
#define HV_H

#include <QWidget>

namespace Ui {
class hv;
}

class hv : public QWidget
{
    Q_OBJECT

public:
    explicit hv(QWidget *parent = 0);
    ~hv();

private:
    Ui::hv *ui;
};

#endif // HV_H
