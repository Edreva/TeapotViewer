#include "dialogeditshrinkfilter.h"
#include "ui_dialogeditshrinkfilter.h"

using namespace::std;

DialogEditShrinkFilter::DialogEditShrinkFilter(QWidget *parent, double startingValue) :
    QDialog(parent),
    ui(new Ui::DialogEditShrinkFilter)
{
    ui->setupUi(this);
    //setup limits of spinboxes and sliders and their starting values
    ui->shrinkFactorSlider->setMinimum(0);
    ui->shrinkFactorSlider->setMaximum(100);
    ui->shrinkFactorSpinBox->setRange(0,1);
    ui->shrinkFactorSpinBox->setSingleStep(0.01);
    ui->shrinkFactorSlider->setValue(int(startingValue*100));
    ui->shrinkFactorSpinBox->setValue(startingValue);

    //Connect ui elements to their relevant handler functions
    connect(ui->shrinkFactorSlider, SIGNAL(valueChanged(int)),this, SLOT(handleShrinkFactorSlider(int)));
    connect(ui->shrinkFactorSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleShrinkFactorSpinBox(double)));

}

DialogEditShrinkFilter::~DialogEditShrinkFilter()
{
    delete ui;
}



//handler function for the slider
void DialogEditShrinkFilter::handleShrinkFactorSlider(int value)
{
    shrinkFactor = value / 100.0;
    ui->shrinkFactorSpinBox->setValue(shrinkFactor); //ensure that the slider and spin box are in sync
    emit shrinkFactorChanged(shrinkFactor);
}
//handler function for the spin box
void DialogEditShrinkFilter::handleShrinkFactorSpinBox(double value)
{
    shrinkFactor = value;
    ui->shrinkFactorSlider->setValue(int(shrinkFactor * 100));
    emit shrinkFactorChanged(shrinkFactor);
}
