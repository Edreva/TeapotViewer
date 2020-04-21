#include "dialogeditcurvaturefilter.h"
#include "ui_dialogeditcurvaturefilter.h"

DialogEditCurvatureFilter::DialogEditCurvatureFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditCurvatureFilter)
{
    ui->setupUi(this);
}

DialogEditCurvatureFilter::~DialogEditCurvatureFilter()
{
    delete ui;
}
