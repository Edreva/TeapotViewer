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
    explicit DialogEditCurvatureFilter(QWidget *parent = nullptr,
                                       int initMinVal = 10, int initMaxVal = 20, int initType = 3, int initScheme = 16);
    ~DialogEditCurvatureFilter();
public slots:
    //Function that updates spinbox values and slider positions when the lower value spin box is changed
    void handleMinSpinBox(int);
    //Function that updates spinbox values and slider positions when the upper value spin box is changed
    void handleMaxSpinBox(int);
    //Function that updates spinbox values and slider positions when the lower value slider is changed
    void handleMinSlider(int);
    //Function that updates spinbox values and slider positions when the upper value slider is changed
    void handleMaxSlider(int);
    //Function that updates the stored value for scheme based on the scheme spin box interaction
    void handleSchemeSpinBox(int);
    //Function that updates the stored value for type based on the currently selected option in the combo box
    void handleTypeComboBox(int);

signals:
    void valuesChanged(int min, int max, int type, int scheme);

private:
    Ui::DialogEditCurvatureFilter *ui;
    int minVal;
    int maxVal;
    int type;
    int scheme;
};

#endif // DIALOGEDITCURVATUREFILTER_H
