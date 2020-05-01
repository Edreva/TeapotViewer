//mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/// @file mainwindow.h This file contains the declarations for the mainwindow class and callback classes

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
#include <vtkCenterOfMass.h>
#include <vtkClipDataSet.h>
#include <vtkColorSeries.h>
#include <vtkColorTransferFunction.h>
#include <vtkCurvatures.h>
#include <vtkCommand.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkHexahedron.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkLight.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPlane.h>
#include <vtkPlaneWidget.h>
#include <vtkPNGWriter.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkPyramid.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkScalarBarActor.h>
#include <vtkShrinkFilter.h>
#include <vtkSmartPointer.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
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
#include "dialogeditcurvaturefilter.h"

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


//Callback functions
///@brief Class for providing a callback function enabling boxwidget functionality
class vtkBoxWidgetCallback : public vtkCommand
{
public:
  static vtkBoxWidgetCallback *New();
  void SetActor( vtkSmartPointer<vtkActor> actor );
  virtual void Execute( vtkObject *caller, unsigned long, void* );
  vtkSmartPointer<vtkActor> m_actor;
};
///@brief Class for providing a callback function enabling plane widget functionality
class vtkPlaneWidgetCallback : public vtkCommand
{
public:
    static vtkPlaneWidgetCallback *New();
    virtual void Execute( vtkObject *caller, unsigned long, void* );
    vtkPlaneWidgetCallback():Plane(0),Actor(0){}
    vtkPlane *Plane;
    vtkActor *Actor;
};
/**
 * @brief Class for creating the main window of the application
 * @author Ewan Drever-Smith
 * @author Wenxiang LUO
 * @version 1.0
 * @date 02/05/2020
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ///@brief Default Constructor
    ///@param parent Specifies the parent widget to this window, if there is one
    MainWindow(QWidget *parent = nullptr);
    ///@brief Default Deconstructor
    ~MainWindow();
public slots:
    ///@brief Allows user to choose a file (.mod or .stl) to open using file dialog
    ///@note resets actions, radiobuttons and checkboxes to their default state
    void open();
    ///@brief Allows user to select the colour of the camera light by opening a colour picker dialog
    ///@note Also enables the remove light button
    void setLightColor();
    ///@brief Updates the intensity of the light
    ///@note Updates position of slider to match updated value
    void setLightIntensitySpinBox();
    ///@brief Updates the intensity of the light
    ///@note Updates spin box to match updated value
    void setLightIntensitySlider();
    ///@brief Removes the lights from the renderer
    void resetLight();
    ///@brief Allows user to select the colour of the model by opening a colour picker dialog
    void selectedObjectColor();
    ///@brief Allows user to enable or disable the edge visibility of the model
    void visableEdge(int);
    ///@brief Allows user to select the colour of the background by opening a colour picker dialog
    void setBackgroundColor();
    ///@brief Displays one of the three primitive shapes (tetrahedron, pyramid, hexahedron)
    void primitiveShape(int);
    ///@brief Resets the camera to its default position and orientation
    void resetCamera();
    ///@brief Adds orientation widget to the bottom left of the screen which tracks the camera orientation
    void displayOrientationWidget(bool);
    ///@brief Adds the plane widget to the screen, used by clipping filter to determine where to clip
    void displayPlaneWidget(bool);
    ///@brief adds box widget that allows model to be edited
    void displayBoxWidget(bool);
    ///@brief Enables the filter corresponding to the ID passed
    /**
     * @note Table shows each ID's corresponding filter
     * ID|    Filter      |
     * --|----------------|
     * 0 |    No filter   |
     * 1 |  Clip Filter   |
     * 2 | Shrink Filter  |
     * 3 | Smooth Filter  |
     * 4 |Curvature Filter|
    */
    void applyFilter(int);
    ///@brief Allows the shrink filter's properties to be changed
    ///@param double the shrink filters shrink factor parameter
    void editShrinkFilter(double);
    ///@brief Handles the change of the curvature filter's properties
    ///@param int The minimum curvature limit
    ///@param int The maximum curvature limit
    ///@param int The curvature type (min, max, gauss, mean)
    ///@param int The colour scheme
    void editCurvatureFilter(int,int,int,int);
    ///@brief Selects which filter editor dialog to open based on what filter is currently active
    void loadFilterEditor();
    ///@brief Opens the shrink filter editor dialog box
    void loadShrinkFilterDialog();
    ///@brief Opens the curvature filter editor dialog box
    void loadCurvatureFilterDialog();
    ///@brief Opens a .mod file
    void openMOD(QString);
    ///@brief Opens a .stl file
    void openSTL(QString);
    ///@brief convert MOD to STL
    void conversion(Model*);
    ///@brief Allow the user to save a screenshot of the current camera view
    ///@note Does not save the background, only the actors
    void handleScreenshot();

private:
    Ui::MainWindow *ui;

    vtkSmartPointer<vtkSTLReader> STLReader;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkScalarBarActor> scalarBar;
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
    vtkSmartPointer<vtkCurvatures> curvatureFilter;
    vtkSmartPointer<vtkCellArray> cell;
    vtkSmartPointer<vtkPoints> pointData;

    vector<double> value; // store the RGB value of light
    double intensity; // store the intensity of light
    vector<vtkSmartPointer<vtkActor>> primitiveShapeActor; // store all the actor for primitive shape
    vector<vtkSmartPointer<vtkActor>>::const_iterator shapeItor; // iterator for primitive shape
    int previousMinLim;
    int previousMaxLim;
    int previousType;
    int previousScheme;
    QButtonGroup* filterButton; //button group to link radio buttons for the filter
};
#endif // MAINWINDOW_H
