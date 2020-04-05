// this file contains the defination of class MainWindow

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Qt libraries
#include <QButtonGroup>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QVariant>

//VTK libaries
#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkAxesActor.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkClipDataSet.h>
#include <vtkCommand.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkHexahedron.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkLight.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPlane.h>
#include <vtkPlaneWidget.h>
#include <vtkPNGWriter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPyramid.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkShrinkFilter.h>
#include <vtkSmartPointer.h>
#include <vtkTetra.h>
#include <vtkTransform.h>
#include <vtkTriangle.h>
#include <vtkTriangleFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkWindowToImageFilter.h>

//C++ libaries
#include <map>
#include <vector>

//Model Loading Library
#include "vector.h"
#include "model.h"
#include "material.h"
#include "cell.h"
#include "pyramid.h"
#include "hexahedron.h"
#include "tetrahedron.h"

//Dialog Boxes
#include "dialogeditshrinkfilter.h"

using std::vector;
using std::unique_ptr;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


//Callback functions

class vtkBoxWidgetCallback : public vtkCommand
{
public:
  static vtkBoxWidgetCallback *New();
  void SetActor( vtkSmartPointer<vtkActor> actor );
  virtual void Execute( vtkObject *caller, unsigned long, void* );
  vtkSmartPointer<vtkActor> m_actor;
};

class vtkPlaneWidgetCallback : public vtkCommand
{
public:
    static vtkPlaneWidgetCallback *New();
    virtual void Execute( vtkObject *caller, unsigned long, void* );
    vtkPlaneWidgetCallback():Plane(0),Actor(0){}
    vtkPlane *Plane;
    vtkActor *Actor;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // open the STL file
    void open();
    // set the color of the light
    void setLightColor();
    // set the intensity of the light
    void setLightIntensitySpinBox();
    void setLightIntensitySlider();
    // reset the light
    void resetLight();
    // chang color of the object to a user selected color
    void selectedObjectColor();
    // set the edge visibility
    void visableEdge(int);
    // set background color
    void setBackgroundColor();
    // apply filter setting
    void applyFilter(int);
    // add primitive shape
    void primitiveShape(int);
    // reset camera
    void resetCamera();
    //adds orientation widget to the bottom left of the screen
    void displayOrientationWidget(bool);
    //adds widget that allows planes to be edited
    void displayPlaneWidget(bool);
    //adds box widget that allows model to be edited
    void displayBoxWidget(bool);
    //This function will allow the shrink filter's properties to be changed
    void editShrinkFilter(double);
    //Functions for opening different file types
    void openMOD(QString);
    void openSTL(QString);
    //function to open the filter editor dialog box
    void loadShrinkFilterDialog();
    // convert MOD to STL
    void conversion(Model*);
    //function to select which filter editor dialog to open
    void loadFilterEditor();
    //function to allow the user to save a screenshot
    void handleScreenshot();

private:
    Ui::MainWindow *ui;

    vtkSmartPointer<vtkSTLReader> STLReader;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkActor> actor;
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkLight> light;
    vtkSmartPointer<vtkNamedColors> color;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkActor> shapeActor;
    vtkSmartPointer<vtkCamera> camera;
    vtkSmartPointer<vtkAxesActor> axes;
    vtkSmartPointer<vtkOrientationMarkerWidget> orientationMarker;
    vtkSmartPointer<vtkPlane> plane;
    vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidget;
    vtkSmartPointer<vtkBoxWidget2> boxWidget;
    vtkSmartPointer<vtkBoxWidgetCallback> boxWidgetCallback;
    vtkSmartPointer<vtkPlaneWidgetCallback> planeWidgetCallback;
    vtkSmartPointer<vtkImplicitPlaneRepresentation> rep;
    vtkSmartPointer<vtkShrinkFilter> shrinkFilter;
  	vtkSmartPointer<vtkCellArray> cell;
	  vtkSmartPointer<vtkPoints> pointData;
    
    dialogEditShrinkFilter *shrinkFilterDialog;

    vector<double> value; // store the RGB value of light
    double intensity; // store the intensity of light
    vector<vtkSmartPointer<vtkActor>> primitiveShapeActor; // store all the actor for primitive shape
    vector<vtkSmartPointer<vtkActor>>::const_iterator shapeItor; // iterator for primitive shape

    QButtonGroup* filterButton; //button group to link radio buttons for the filter
};
#endif // MAINWINDOW_H
