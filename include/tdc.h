#ifndef TDC_H
#define TDC_H

#include <QWidget>

namespace Ui {
class tdc;
}

class tdc : public QWidget
{
    Q_OBJECT

public:
    explicit tdc(QWidget *parent = 0);
    ~tdc();

private:
    Ui::tdc *ui;
};

#endif // TDC_H
