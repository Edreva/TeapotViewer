#include "dialogeditcurvaturefilter.h"
#include "ui_dialogeditcurvaturefilter.h"

DialogEditCurvatureFilter::DialogEditCurvatureFilter(QWidget *parent,
                                                     int initMinVal, int initMaxVal, int initType, int initScheme) :
    QDialog(parent), ui(new Ui::DialogEditCurvatureFilter),
    minVal(initMinVal), maxVal(initMaxVal), type(initType), scheme(initScheme)
{
    ui->setupUi(this);
    //Configure spinboxes and sliders
    ui->minSpinBox->setRange(-30,30);
    ui->minSpinBox->setSingleStep(1);
    ui->maxSpinBox->setRange(-30,30);
    ui->maxSpinBox->setSingleStep(1);
    ui->minSlider->setMinimum(-30);
    ui->minSlider->setMaximum(30);
    ui->maxSlider->setMinimum(-30);
    ui->maxSlider->setMaximum(30);
    ui->minSpinBox->setValue(minVal);
    ui->maxSpinBox->setValue(maxVal);
    ui->minSlider->setValue(minVal);
    ui->maxSlider->setValue(maxVal);
    ui->schemeSpinBox->setRange(1,60);
    ui->schemeSpinBox->setValue(type);
    ui->typeComboBox->addItem("Minimum");
    ui->typeComboBox->addItem("Maximum");
    ui->typeComboBox->addItem("Gaussian");
    ui->typeComboBox->addItem("Mean");

    ui->maxSpinBox->setValue(maxVal);
    ui->maxSlider->setValue(maxVal);
    ui->minSpinBox->setValue(minVal);
    ui->minSlider->setValue(minVal);
    ui->typeComboBox->setCurrentIndex(type);
    ui->schemeSpinBox->setValue(scheme);

    //Connect ui elements to their relevant handler functions
    connect(ui->minSpinBox, SIGNAL(valueChanged(int)), this, SLOT(handleMinSpinBox(int)));
    connect(ui->maxSpinBox, SIGNAL(valueChanged(int)), this, SLOT(handleMaxSpinBox(int)));
    connect(ui->minSlider, SIGNAL(valueChanged(int)), this, SLOT(handleMinSlider(int)));
    connect(ui->maxSlider, SIGNAL(valueChanged(int)), this, SLOT(handleMaxSlider(int)));
    connect(ui->schemeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(handleSchemeSpinBox(int)));
    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handleTypeComboBox(int)));
}

DialogEditCurvatureFilter::~DialogEditCurvatureFilter()
{
    delete ui;
}

//Function that updates spinbox values and slider positions when the upper value slider is changed
void DialogEditCurvatureFilter::handleMaxSlider(int value)
{
    maxVal = value;
    //Ensure that maxVal cannot be made less than minVal
    if(maxVal < minVal)
    {
        minVal = maxVal;
        ui->minSpinBox->setValue(minVal);
        ui->minSlider->setValue(minVal);
    }
    //Update ui elements
    ui->maxSpinBox->setValue(maxVal);

    //emit signal containing updated values
    emit valuesChanged(minVal, maxVal, type, scheme);
}

//Function that updates spinbox values and slider positions when the upper value spin box is changed
void DialogEditCurvatureFilter::handleMaxSpinBox(int value)
{
    maxVal = value;
    //Ensure that maxVal cannot be made less than minVal
    if(maxVal < minVal)
    {
        minVal = maxVal;
        ui->minSpinBox->setValue(minVal);
        ui->minSlider->setValue(minVal);
    }
    //Update ui elements
    ui->maxSlider->setValue(maxVal);

    //emit signal containing updated values
    emit valuesChanged(minVal, maxVal, type, scheme);
}

//Function that updates spinbox values and slider positions when the lower value slider is changed
void DialogEditCurvatureFilter::handleMinSlider(int value)
{
    minVal = value;
    //Ensure that maxVal cannot be made less than minVal
    if(minVal > maxVal)
    {
        maxVal = minVal;
        ui->maxSpinBox->setValue(maxVal);
        ui->maxSlider->setValue(maxVal);
    }
    //Update ui elements
    ui->minSpinBox->setValue(minVal);

    //emit signal containing updated values
    emit valuesChanged(minVal, maxVal, type, scheme);
}

//Function that updates spinbox values and slider positions when the lower value spin box is changed
void DialogEditCurvatureFilter::handleMinSpinBox(int value)
{
    minVal = value;
    //Ensure that maxVal cannot be made less than minVal
    if(minVal > maxVal)
    {
        maxVal = minVal;
        ui->maxSpinBox->setValue(maxVal);
        ui->maxSlider->setValue(maxVal);
    }
    //Update ui elements
    ui->minSlider->setValue(minVal);

    //emit signal containing updated values
    emit valuesChanged(minVal, maxVal, type, scheme);
}

//Function that updates the stored value for scheme based on the scheme spin box interaction
void DialogEditCurvatureFilter::handleSchemeSpinBox(int value)
{
    scheme = value;
    emit valuesChanged(minVal, maxVal, type, scheme);
}
//Function that updates the stored value for type based on the currently selected option in the combo box
void DialogEditCurvatureFilter::handleTypeComboBox(int value)
{
    type = value;
    emit valuesChanged(minVal, maxVal, type, scheme);
}
