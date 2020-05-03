//dialogeditshrinkfilter.h

#ifndef DIALOGEDITCURVATUREFILTER_H
#define DIALOGEDITCURVATUREFILTER_H

/// @file mainwindow.h This file contains the declarations for the DialogEditCurvatureFilterfilter class

#include <QDialog>

namespace Ui {
class DialogEditCurvatureFilter;
}

/**
 * @brief Class for creating the dialog window to edit the curvature filter
 * @author Ewan Drever-Smith
 * @version 1.0
 * @date 02/05/2020
 */
class DialogEditCurvatureFilter : public QDialog
{
    Q_OBJECT

public:
    ///@brief Constructor which takes as potential arguments the parent widget and the starting values of the curvature filter properties
    explicit DialogEditCurvatureFilter(QWidget *parent = nullptr,
                                       int initMinVal = 10, int initMaxVal = 20, int initType = 3, int initScheme = 16);
    ///@brief Default Deconstructor
    ~DialogEditCurvatureFilter();
public slots:
    ///@brief Function that updates spinbox values and slider positions when the lower value spin box is changed
    void handleMinSpinBox(int);
    ///@brief Function that updates spinbox values and slider positions when the upper value spin box is changed
    void handleMaxSpinBox(int);
    ///@brief Function that updates spinbox values and slider positions when the lower value slider is changed
    void handleMinSlider(int);
    ///@brief Function that updates spinbox values and slider positions when the upper value slider is changed
    void handleMaxSlider(int);
    ///@brief Function that updates the stored value for scheme based on the scheme spin box interaction
    void handleSchemeSpinBox(int);
    ///@brief Function that updates the stored value for type based on the currently selected option in the combo box
    void handleTypeComboBox(int);

signals:
    ///@brief Signal that carries the values for the curvature filter properties
    void valuesChanged(int min, int max, int type, int scheme);

private:
    Ui::DialogEditCurvatureFilter *ui;
    int minVal;
    int maxVal;
    int type;
    int scheme;
};

#endif // DIALOGEDITCURVATUREFILTER_H
