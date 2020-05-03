//dialogeditshrinkfilter.h

#ifndef DIALOGEDITSHRINKFILTER_H
#define DIALOGEDITSHRINKFILTER_H

/// @file mainwindow.h This file contains the declarations for the dialogeditshrinkfilter class

#include <QDialog>

namespace Ui {
class DialogEditShrinkFilter;
}

/**
 * @brief Class for creating the dialog window to edit the shrink filter
 * @author Ewan Drever-Smith
 * @version 1.0
 * @date 02/05/2020
 */
class DialogEditShrinkFilter : public QDialog
{
    Q_OBJECT

public:
    ///@brief Constructor which takes as potential arguments the parent widget and the starting value of shrinkFactor
    explicit DialogEditShrinkFilter(QWidget *parent = nullptr, double startingValue = 1);
    ///@brief Default Deconstructor
    ~DialogEditShrinkFilter();
public slots:
    ///@brief handler function which takes the value from the slider and updates class values
    ///@note Updates the value of spin box to match the slider value
    void handleShrinkFactorSlider(int);
    ///@brief handler function which takes the value from the slider and updates class values
    ///@note Updates the value of spin box to match the slider value
    void handleShrinkFactorSpinBox(double);
signals:
    ///@brief Signal that carries the shrinkFactor value.
    void shrinkFactorChanged(double);

private:
    Ui::DialogEditShrinkFilter *ui;
    double shrinkFactor;
};

#endif // DIALOGEDITSHRINKFILTER_H
