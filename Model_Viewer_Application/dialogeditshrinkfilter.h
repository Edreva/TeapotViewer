#ifndef DIALOGEDITSHRINKFILTER_H
#define DIALOGEDITSHRINKFILTER_H

#include <QDialog>

namespace Ui {
class DialogEditShrinkFilter;
}

class DialogEditShrinkFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditShrinkFilter(QWidget *parent = nullptr, double startingValue = 1);
    ~DialogEditShrinkFilter();
public slots:
    void handleShrinkFactorSlider(int);
    void handleShrinkFactorSpinBox(double);
signals:
    void shrinkFactorChanged(double);

private:
    Ui::DialogEditShrinkFilter *ui;
    double shrinkFactor;
};

#endif // DIALOGEDITSHRINKFILTER_H
