#ifndef DIALOGEDITCURVATUREFILTER_H
#define DIALOGEDITCURVATUREFILTER_H

#include <QDialog>

namespace Ui {
class DialogEditCurvatureFilter;
}

class DialogEditCurvatureFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditCurvatureFilter(QWidget *parent = nullptr);
    ~DialogEditCurvatureFilter();

private:
    Ui::DialogEditCurvatureFilter *ui;
};

#endif // DIALOGEDITCURVATUREFILTER_H
