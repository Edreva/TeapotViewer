// this file contains the implementations of function defined in MainWindow class

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogeditshrinkfilter.h"

using std::map;
using std::vector;

//Constructor for vtkBoxWidgetCallback class
vtkBoxWidgetCallback* vtkBoxWidgetCallback::New()
{
    return new vtkBoxWidgetCallback;
}
//Function to set the actor for which the boxwidget transform will be affecting
void vtkBoxWidgetCallback::SetActor( vtkSmartPointer<vtkActor> actor )
{
    m_actor = actor;
}
//Function which transforms the actor based on the user's interaction with the box widget
void vtkBoxWidgetCallback::Execute(vtkObject *caller, unsigned long, void*)
{
    vtkSmartPointer<vtkBoxWidget2> boxWidget =
    vtkBoxWidget2::SafeDownCast(caller);

    vtkSmartPointer<vtkTransform> t =
    vtkSmartPointer<vtkTransform>::New();

    vtkBoxRepresentation::SafeDownCast( boxWidget->GetRepresentation() )->GetTransform( t );
    this->m_actor->SetUserTransform( t );
}
//Constructor for vtkPlaneWidgetCallback class
vtkPlaneWidgetCallback* vtkPlaneWidgetCallback::New()
{
    return new vtkPlaneWidgetCallback;
}

//Class that updates the plane representation based on the plane widget
void vtkPlaneWidgetCallback::Execute(vtkObject *caller, unsigned long, void*)
{
    vtkImplicitPlaneWidget2 *iPlaneWidget = reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
    vtkImplicitPlaneRepresentation *rep = reinterpret_cast<vtkImplicitPlaneRepresentation*>(iPlaneWidget->GetRepresentation());
    rep->GetPlane(this->Plane);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //sets the window icon (top left) other instances of the icon handled in cmakelists
    this->setWindowIcon(QIcon(QString("Group32ModelViewerLogo.ico")));
    this->setWindowTitle("Group32ModelViewer");

    ui->setupUi(this);
    // create a STL reader to read a STL file
    STLReader = vtkSmartPointer<vtkSTLReader>::New();
    // create a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    // create an actor used for set the properties of the model
    actor = vtkSmartPointer<vtkActor>::New();
    // create a light to provides light setting
    light = vtkSmartPointer<vtkLight>::New();
    // create color to set related information
    color = vtkSmartPointer<vtkNamedColors>::New();
    // create a mapper to hold a the information of the model
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    // create an actor for primitive shape
    shapeActor = vtkSmartPointer<vtkActor>::New();
    // create a camera
    camera = vtkSmartPointer<vtkCamera>::New();
    // create axes to use for orientation widget
    axes = vtkSmartPointer<vtkAxesActor>::New();
    // creates interactable orientation widget object
    orientationMarker = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    // stores all the cells in the mod files
    cell = vtkSmartPointer<vtkCellArray>::New();
    // store all the vector in Mod file
    pointData = vtkSmartPointer<vtkPoints>::New();

    //Creates a plane object which can be used by the clip filter
    plane = vtkSmartPointer<vtkPlane>::New();

    // set the background color of the render window
    renderer->SetBackground(0.8, 0.8, 0.8);
    // set the light
    light->SetLightTypeToHeadlight();
    light->SetPosition(5, 5, 15);
    light->SetPositional(true);
    light->SetConeAngle(10);
    light->SetFocalPoint(0, 0, 0);
    light->SetDiffuseColor(1, 1, 1);
    light->SetAmbientColor(1, 1, 1);
    light->SetSpecularColor(1, 1, 1);

    // link the render window to Qt widget
    ui->openGLWidget->SetRenderWindow(renderWindow);
    ui->openGLWidget->GetRenderWindow()->AddRenderer(renderer);

    // Set up Orientation Widget and link to axes
    orientationMarker->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    orientationMarker->SetOrientationMarker( axes );
    orientationMarker->SetInteractor( ui->openGLWidget->GetRenderWindow()->GetInteractor() );
    orientationMarker->SetViewport( 0.0, 0.0, 0.4, 0.4 );

    // Set up plane widget... could be used to aid with clip filter
    rep = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
    planeWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
    planeWidget->SetInteractor( ui->openGLWidget->GetRenderWindow()->GetInteractor() );
    planeWidgetCallback = vtkSmartPointer<vtkPlaneWidgetCallback>::New();
    //planeWidget->HandlesOff();
    ui->actionDisplayPlaneWidget->setEnabled(false); //widget disabled unless clip filter is on

    //Set up box widget
    boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();
    boxWidget->SetInteractor(ui->openGLWidget->GetRenderWindow()->GetInteractor());
    //set up a callback for the interactor so it can manipulate the actor
    boxWidgetCallback = vtkSmartPointer<vtkBoxWidgetCallback>::New();


    // set short cut for some operations
    ui->actionOpen->setShortcut(tr("Ctrl+O"));
    ui->changeLightColourButton->setShortcut(tr("Alt+C"));
    ui->objectColor->setShortcut(tr("Shift+C"));
    ui->resetCameraButton->setShortcut(tr("Ctrl+R"));

    ui->actionOpen->setIcon(QIcon("openIcon.png"));
    ui->actionScreenshot->setIcon(QIcon("screenshotIcon.png"));

    // set the position of the text on the label
    ui->lightLabel->setAlignment(Qt::AlignCenter);
    ui->objectColorLabel->setAlignment(Qt::AlignCenter);

    // set the range of the spin box and its initial value, single step
    ui->intensity->setMinimum(0);
    ui->intensity->setMaximum(1);
    ui->intensity->setValue(0.3);
    ui->intensity->setSingleStep(0.1);

    // set the range of the slider and its initial value
    ui->intensitySlider->setMinimum(0);
    ui->intensitySlider->setMaximum(10);
    ui->intensitySlider->setValue(3);

    // disable all meanless widget before loading a model
    ui->intensity->setEnabled(false);
    ui->intensitySlider->setEnabled(false);
    ui->removeLight->setEnabled(false);
    ui->changeLightColourButton->setEnabled(false);
    ui->objectColor->setEnabled(false);
    ui->edgeCheck->setEnabled(false);
    ui->noFilter->setEnabled(false);
    ui->clipfilter->setEnabled(false);
    ui->shrinkfilter->setEnabled(false);
    ui->smoothFilter->setEnabled(false);
    ui->curvatureFilter->setEnabled(false);
    ui->resetCameraButton->setEnabled(false);
    ui->editFilterButton->setEnabled(false);

    // set initial state of check box and radio buttons
    ui->edgeCheck->setChecked(false);
    ui->noFilter->setChecked(true);
    ui->noShape->setChecked(true);

    // create a button group for radio buttons (filter)
    filterButton = new QButtonGroup(this);
    filterButton->addButton(ui->noFilter, 0);
    filterButton->addButton(ui->clipfilter, 1);
    filterButton->addButton(ui->shrinkfilter, 2);
    filterButton->addButton(ui->smoothFilter, 3);
    filterButton->addButton(ui->curvatureFilter, 4);

    // create a button group for radio vuttons (primitive shape)
    QButtonGroup* shapeButton = new QButtonGroup(this);
    shapeButton->addButton(ui->noShape, 0);
    shapeButton->addButton(ui->pyramid, 1);
    shapeButton->addButton(ui->tetrahedron, 2);
    shapeButton->addButton(ui->hexahedron, 3);

    // initialize the vector for light color
	for (size_t i = 0; i < 3; i++)
	{
		value.push_back(1);
	}

    // connect operations to its widgets
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionDisplayOrientationWidget, SIGNAL(toggled(bool)), this, SLOT(displayOrientationWidget(bool)));
    connect(ui->actionDisplayPlaneWidget, SIGNAL(toggled(bool)), this, SLOT(displayPlaneWidget(bool)));
    connect(ui->actionDisplayBoxWidget, SIGNAL(toggled(bool)), this, SLOT(displayBoxWidget(bool)));
    connect(ui->actionScreenshot, SIGNAL(triggered()), this, SLOT(handleScreenshot()));
    connect(ui->changeLightColourButton, SIGNAL(clicked()), this, SLOT(setLightColor()));
    connect(ui->intensity, SIGNAL(valueChanged(double)), this, SLOT(setLightIntensitySpinBox()));
    connect(ui->intensitySlider, SIGNAL(valueChanged(int)), this, SLOT(setLightIntensitySlider()));
    connect(ui->removeLight, SIGNAL(clicked()), this, SLOT(resetLight()));
    connect(ui->objectColor, SIGNAL(clicked()), this, SLOT(selectedObjectColor()));
    connect(ui->edgeCheck, SIGNAL(stateChanged(int)), this, SLOT(visableEdge(int)));
    connect(ui->backgroundColor, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
    connect(filterButton, SIGNAL(buttonClicked(int)), this, SLOT(applyFilter(int)));
    connect(shapeButton, SIGNAL(buttonClicked(int)), this, SLOT(primitiveShape(int)));
    connect(ui->resetCameraButton, SIGNAL(clicked()), this, SLOT(resetCamera()));
    connect(ui->editFilterButton, SIGNAL(clicked()), this, SLOT(loadFilterEditor()));
    connect(ui->actionFullscreen, SIGNAL(toggled(bool)),this, SLOT(handleFullscreen(bool)));

    //loads the logo to begin with
    openSTL(QString("logo.stl"));
    ui->noShape->setChecked(true);
    ui->noFilter->setChecked(true);
    ui->edgeCheck->setChecked(false);
    ui->actionDisplayPlaneWidget->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//function to allow the window to be made fullscreen
void MainWindow::handleFullscreen(bool checked)
{
    if(checked)
        this->setWindowState(Qt::WindowFullScreen);
    else
    {
        this->setWindowState(Qt::WindowMinimized);
        this->setWindowState(Qt::WindowActive);
    }
}
//function to convert the current vtk window to a png and save it
void MainWindow::handleScreenshot()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Screenshot"),"./",tr("*.png"));

    if (filename.isEmpty()) //if no user exits file dialog rest of function is skipped
        return;

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(renderWindow);
    windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha channel
    windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
    windowToImageFilter->Update();

    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(filename.toLatin1().data());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();

}
//function to select which filter editing dialog box to load based off of which filter is currently selected
void MainWindow::loadFilterEditor()
{
    switch(filterButton->checkedId())
    {
    case 0 :
        break;
    case 1 :
        if(ui->actionDisplayPlaneWidget->isChecked())
            ui->actionDisplayPlaneWidget->setChecked(false);
        else
            ui->actionDisplayPlaneWidget->setChecked(true);
        break;
    case 2 :
        loadShrinkFilterDialog();
        break;
    case 3 :
       break;
    case 4 :
        loadCurvatureFilterDialog();
    }
}
//function to load dialog widget and link its signal to a function which edits the shrink filter
void MainWindow::loadShrinkFilterDialog()
{
    DialogEditShrinkFilter *shrinkFilterDialog(new DialogEditShrinkFilter(this,shrinkFilter->GetShrinkFactor()));
    connect(shrinkFilterDialog, SIGNAL(shrinkFactorChanged(double)), this, SLOT(editShrinkFilter(double)));
    shrinkFilterDialog->show();
    return;
}
//
void MainWindow::loadCurvatureFilterDialog()
{
    DialogEditCurvatureFilter *curvatureFilterDialog(new DialogEditCurvatureFilter(this, previousMinLim, previousMaxLim, previousType, previousScheme));
    connect(curvatureFilterDialog, SIGNAL(valuesChanged(int,int,int,int)), this, SLOT(editCurvatureFilter(int,int,int,int)));
    curvatureFilterDialog->show();
    return;
}
//This function allows the properties of the shrink filter to be changed.
void MainWindow::editShrinkFilter(double shrinkFactor)
{
    shrinkFilter->SetShrinkFactor(shrinkFactor);
    shrinkFilter->Update();
    ui->openGLWidget->GetRenderWindow()->Render();
    return;
}
//function to handle the change of the curvature filter's minimum and maximum scalar limit
void MainWindow::editCurvatureFilter(int minLim, int maxLim, int type, int scheme)
{
    previousMinLim = minLim;
    previousMaxLim = maxLim;
    previousType = type;
    previousScheme = scheme;
    switch (type) //TODO change int type to enum for better clarity
    {
    case 0:
        curvatureFilter->SetCurvatureTypeToMinimum();
        break;
    case 1:
        curvatureFilter->SetCurvatureTypeToMaximum();
        break;
    case 2:
        curvatureFilter->SetCurvatureTypeToGaussian();
        break;
    case 3:
        curvatureFilter->SetCurvatureTypeToMean();
        break;
    }
    curvatureFilter->Update();

    vtkSmartPointer<vtkColorSeries> colorSeries = vtkSmartPointer<vtkColorSeries>::New();
    colorSeries->SetColorScheme(scheme);

    vtkSmartPointer<vtkColorTransferFunction> lut = vtkSmartPointer<vtkColorTransferFunction>::New();
    lut->SetColorSpaceToHSV();

    // Use a color series to create a transfer function
    int numColors = colorSeries->GetNumberOfColors();
    for (int i = 0; i < numColors; i++)
    {
        vtkColor3ub color = colorSeries->GetColor(i);
        double dColor[3];
        dColor[0] = static_cast<double> (color[0]) / 255.0;
        dColor[1] = static_cast<double> (color[1]) / 255.0;
        dColor[2] = static_cast<double> (color[2]) / 255.0;
        double t = minLim + (maxLim - minLim) / (numColors - 1) * i;
        lut->AddRGBPoint(t, dColor[0], dColor[1], dColor[2]);
    }

    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(minLim, maxLim);
    //actor->SetMapper(mapper);
    renderer->RemoveActor2D(scalarBar);
    scalarBar =  vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetTitle(curvatureFilter->GetOutput()->GetPointData()->GetScalars()->GetName());
    scalarBar->SetNumberOfLabels(5);

    renderer->AddActor2D(scalarBar);

    //do i make curvature filter a private var or just generate new one each time a change is made
    ui->openGLWidget->GetRenderWindow()->Render();
}
//function to open a file dialog and allow the user to select the file they wish to open
void MainWindow::open()
{
    ui->noShape->setChecked(true);

    // set the filter for STL file
    QString filter = "Model Files (*.stl *.mod)";
    // obtain the file name
    QString filename = QFileDialog::getOpenFileName(this, QString("Open STL file"), "./", filter);

    if (actor == nullptr)
        actor = vtkSmartPointer<vtkActor>::New();

    if(filename.endsWith(".stl"))
        openSTL(filename);

    else if(filename.endsWith(".mod"))
        openMOD(filename);

    else //terminate if filename isnt valid
    {
        QMessageBox::critical(this, "Error", "Invalid file name", QMessageBox::Ok);
        return;
    }

    resetCamera();
    //reset actions
    ui->actionDisplayOrientationWidget->setChecked(false);
    ui->actionDisplayPlaneWidget->setChecked(false);
    ui->actionDisplayBoxWidget->setChecked(false);
    ui->actionDisplayBoxWidget->setEnabled(true);

    // reset all other functions
    ui->noShape->setChecked(true);
    ui->noFilter->setChecked(true);
    ui->edgeCheck->setChecked(false);

    shapeActor = nullptr;
}
//function to handle opening a .stl file
void MainWindow::openSTL(QString filename)
{
    // remove all the actors
    for(shapeItor = primitiveShapeActor.begin(); shapeItor != primitiveShapeActor.end(); shapeItor++)
    {
        renderer->RemoveActor(*shapeItor);
    }
    renderer->RemoveActor(shapeActor);
    renderer->RemoveActor(actor);
    renderer->RemoveActor(scalarBar);

    // read the file
    STLReader->SetFileName(filename.toLatin1().data());
    STLReader->Update();

    mapper->SetInputConnection(STLReader->GetOutputPort());

    // set properties and data of the actor
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(color->GetColor3d("IndianRed").GetData());

    // Calculate centre of mass of the object
    vtkSmartPointer<vtkCenterOfMass> centerOfMass = vtkSmartPointer<vtkCenterOfMass>::New();
    centerOfMass->SetInputData(mapper->GetInput());
    centerOfMass->SetUseScalarsAsWeights(false);
    centerOfMass->Update();
    double com[3];
    centerOfMass->GetCenter(com);

    //Create string containing rounded centre of mass to display on gui
    QString center = QString::number(com[0],'f',1);
    center.append(", ");
    center.append(QString::number(com[1],'f',1));
    center.append(", ");
    center.append(QString::number(com[2],'f',1));

    //create string containing precise centre of mass to display as tooltip
    QString preciseCenter = QString::number(com[0]);
    preciseCenter.append(", ");
    preciseCenter.append(QString::number(com[1]));
    preciseCenter.append(", ");
    preciseCenter.append(QString::number(com[2]));

    //update model statistics
    ui->pointLabel->setText(QVariant(mapper->GetInput()->GetNumberOfPoints()).toString());
    ui->cellLabel->setText(QVariant(mapper->GetInput()->GetNumberOfCells()).toString());
    ui->centreLabel->setText(center);
    ui->centreLabel->setToolTip(preciseCenter);
    ui->weightLabel->setText(QString("-"));

    // render the actor
    renderer->AddActor(actor);

    // enable some of the meaningful operations after loading the file
    ui->changeLightColourButton->setEnabled(true);
    ui->intensity->setEnabled(true);
    ui->intensitySlider->setEnabled(true);
    ui->objectColor->setEnabled(true);
    ui->edgeCheck->setEnabled(true);
    ui->noFilter->setEnabled(true);
    ui->clipfilter->setEnabled(true);
    ui->shrinkfilter->setEnabled(true);
    ui->curvatureFilter->setEnabled(true);
    ui->smoothFilter->setEnabled(true);
    ui->resetCameraButton->setEnabled(true);
}
//function to handle opening a .stl file
void MainWindow::openMOD(QString filename)
{
    ui->actionDisplayBoxWidget->setEnabled(false);
    ui->actionDisplayBoxWidget->setChecked(false);
    // remove all the actors
    for(shapeItor = primitiveShapeActor.begin(); shapeItor != primitiveShapeActor.end(); shapeItor++)
    {
        renderer->RemoveActor(*shapeItor);
    }
    renderer->RemoveActor(shapeActor);
    renderer->RemoveActor(actor);
    renderer->RemoveActor(scalarBar);

    // read the file and declare vectors to store the data
    Model loadMOD(filename.toStdString());

    QString preciseCenter = QString::number(loadMOD.getModelCentre().get_i());
    preciseCenter.append(", ");
    preciseCenter.append(QString::number(loadMOD.getModelCentre().get_j()));
    preciseCenter.append(", ");
    preciseCenter.append(QString::number(loadMOD.getModelCentre().get_k()));

    QString center = QString::number(loadMOD.getModelCentre().get_i(),'f',1);
    center.append(", ");
    center.append(QString::number(loadMOD.getModelCentre().get_j(),'f',1));
    center.append(", ");
    center.append(QString::number(loadMOD.getModelCentre().get_k(),'f',1));

    //update model statistics
    ui->pointLabel->setText(QVariant(int(loadMOD.getNumberOfVertices())).toString());
    ui->cellLabel->setText(QVariant(int(loadMOD.getNumberOfCells())).toString());
    ui->weightLabel->setText(QString::number(loadMOD.getModelWeight(), 'f', 3));
    ui->weightLabel->setToolTip(QString::number(loadMOD.getModelWeight()));
    ui->centreLabel->setText(center);
    ui->centreLabel->setToolTip(preciseCenter);

    vector<Tetrahedron> tetrData = loadMOD.getTetra();
    vector<Pyramid> pyramidData = loadMOD.getPyramid();
    vector<Hexahedron> hexData = loadMOD.getHex();
	// declare a vector to store the color of each cell
	vector<int> shapeColor;

    // obtain the number of the primitive shapes
    int shapeNumber = hexData.size() + pyramidData.size() + tetrData.size();

    // declare vectors to store the information different types of primitive shapes
    vector<vtkSmartPointer<vtkHexahedron>> hexSource(hexData.size());
    vector<vtkSmartPointer<vtkPyramid>> pyramidSource(pyramidData.size());
    vector<vtkSmartPointer<vtkTetra>> tetrSource(tetrData.size());

    // remove light
    renderer->RemoveLight(light);

    // declare a vector to store all the information related to actors
    primitiveShapeActor.resize(shapeNumber);
    vector<vtkSmartPointer<vtkCellArray>> cellData(shapeNumber);
    vector<vtkSmartPointer<vtkPoints>> pointData(shapeNumber);
    vector<vtkSmartPointer<vtkUnstructuredGrid>> ugData(shapeNumber);
    vector<vtkSmartPointer<vtkDataSetMapper>> mapperData(shapeNumber);


    // declare variable for iteration
    size_t i, j, k;

    for(i = 0; i < hexData.size(); i++)
    {
        hexSource[i] = vtkSmartPointer<vtkHexahedron>::New();
        pointData[i] = vtkSmartPointer<vtkPoints>::New();
        primitiveShapeActor[i] = vtkSmartPointer<vtkActor>::New();
		shapeColor = hexData[i].getColorRGB();

        float p0[3] = {hexData[i].getVertex()[0].get_i(), hexData[i].getVertex()[0].get_j(), hexData[i].getVertex()[0].get_k()};
        float p1[3] = {hexData[i].getVertex()[1].get_i(), hexData[i].getVertex()[1].get_j(), hexData[i].getVertex()[1].get_k()};
        float p2[3] = {hexData[i].getVertex()[2].get_i(), hexData[i].getVertex()[2].get_j(), hexData[i].getVertex()[2].get_k()};
        float p3[3] = {hexData[i].getVertex()[3].get_i(), hexData[i].getVertex()[3].get_j(), hexData[i].getVertex()[3].get_k()};
        float p4[3] = {hexData[i].getVertex()[4].get_i(), hexData[i].getVertex()[4].get_j(), hexData[i].getVertex()[4].get_k()};
        float p5[3] = {hexData[i].getVertex()[5].get_i(), hexData[i].getVertex()[5].get_j(), hexData[i].getVertex()[5].get_k()};
        float p6[3] = {hexData[i].getVertex()[6].get_i(), hexData[i].getVertex()[6].get_j(), hexData[i].getVertex()[6].get_k()};
        float p7[3] = {hexData[i].getVertex()[7].get_i(), hexData[i].getVertex()[7].get_j(), hexData[i].getVertex()[7].get_k()};

        float* data[8] = {p0, p1, p2, p3, p4, p5, p6, p7};

        for(size_t m = 0; m < 8; m++)
        {
            pointData[i]->InsertNextPoint(data[m]);
            hexSource[i]->GetPointIds()->SetId(m, m);
        }

        cellData[i] = vtkSmartPointer<vtkCellArray>::New();
        cellData[i]->InsertNextCell(hexSource[i]);

        ugData[i] = vtkSmartPointer<vtkUnstructuredGrid>::New();
        ugData[i]->SetPoints(pointData[i]);
        ugData[i]->InsertNextCell(hexSource[i]->GetCellType(), hexSource[i]->GetPointIds());

        mapperData[i] = vtkSmartPointer<vtkDataSetMapper>::New();
        mapperData[i]->SetInputData(ugData[i]);

        primitiveShapeActor[i]->SetMapper(mapperData[i]);
		primitiveShapeActor[i]->GetProperty()->SetColor(shapeColor[0] / 255.0, shapeColor[1] / 255.0, shapeColor[2] / 255.0);
    }

    for(j = 0; j < pyramidData.size(); j++)
    {
        int offset = i;

        pyramidSource[j] = vtkSmartPointer<vtkPyramid>::New();
        pointData[j + offset] = vtkSmartPointer<vtkPoints>::New();
        primitiveShapeActor[j + offset] = vtkSmartPointer<vtkActor>::New();
		shapeColor = pyramidData[j].getColorRGB();

        float p0[3] = {pyramidData[j].getVertex()[0].get_i(), pyramidData[j].getVertex()[0].get_j(), pyramidData[j].getVertex()[0].get_k()};
        float p1[3] = {pyramidData[j].getVertex()[1].get_i(), pyramidData[j].getVertex()[1].get_j(), pyramidData[j].getVertex()[1].get_k()};
        float p2[3] = {pyramidData[j].getVertex()[2].get_i(), pyramidData[j].getVertex()[2].get_j(), pyramidData[j].getVertex()[2].get_k()};
        float p3[3] = {pyramidData[j].getVertex()[3].get_i(), pyramidData[j].getVertex()[3].get_j(), pyramidData[j].getVertex()[3].get_k()};
        float p4[3] = {pyramidData[j].getVertex()[4].get_i(), pyramidData[j].getVertex()[4].get_j(), pyramidData[j].getVertex()[4].get_k()};
        float* data[5] = {p0, p1, p2, p3, p4};

        for(size_t m = 0; m < 5; m++)
        {
            pointData[j + offset]->InsertNextPoint(data[m]);
            pyramidSource[j]->GetPointIds()->SetId(m, m);
        }

        cellData[j + offset] = vtkSmartPointer<vtkCellArray>::New();
        cellData[j + offset]->InsertNextCell(pyramidSource[j]);

        ugData[j + offset] = vtkSmartPointer<vtkUnstructuredGrid>::New();
        ugData[j + offset]->SetPoints(pointData[j+offset]);
        ugData[j + offset]->InsertNextCell(pyramidSource[j]->GetCellType(), pyramidSource[j]->GetPointIds());

        mapperData[j + offset] = vtkSmartPointer<vtkDataSetMapper>::New();
        mapperData[j + offset]->SetInputData(ugData[j + offset]);

        primitiveShapeActor[j + offset]->SetMapper(mapperData[j + offset]);
		primitiveShapeActor[j + offset]->GetProperty()->SetColor(shapeColor[0] / 255.0, shapeColor[1] / 255.0, shapeColor[2] / 255.0);
    }

    for(k = 0; k < tetrData.size(); k++)
    {
        int offset = i + j;

        tetrSource[k] = vtkSmartPointer<vtkTetra>::New();
        pointData[k + offset] = vtkSmartPointer<vtkPoints>::New();
        primitiveShapeActor[k + offset] = vtkSmartPointer<vtkActor>::New();
		shapeColor = tetrData[k].getColorRGB();

        pointData[k + offset]->InsertNextPoint(tetrData[k].getVertex()[0].get_i(), tetrData[k].getVertex()[0].get_j(), tetrData[k].getVertex()[0].get_k());
        pointData[k + offset]->InsertNextPoint(tetrData[k].getVertex()[1].get_i(), tetrData[k].getVertex()[1].get_j(), tetrData[k].getVertex()[1].get_k());
        pointData[k + offset]->InsertNextPoint(tetrData[k].getVertex()[2].get_i(), tetrData[k].getVertex()[2].get_j(), tetrData[k].getVertex()[2].get_k());
        pointData[k + offset]->InsertNextPoint(tetrData[k].getVertex()[3].get_i(), tetrData[k].getVertex()[3].get_j(), tetrData[k].getVertex()[3].get_k());

        for(size_t m = 0; m < 4; m++)
        {
            tetrSource[k]->GetPointIds()->SetId(m, m);
        }

        cellData[k + offset] = vtkSmartPointer<vtkCellArray>::New();
        cellData[k + offset]->InsertNextCell(tetrSource[k]);

        ugData[k+ offset] = vtkSmartPointer<vtkUnstructuredGrid>::New();
        ugData[k+ offset]->SetPoints(pointData[k + offset]);
        ugData[k+ offset]->SetCells(VTK_TETRA, cellData[k + offset]);

        mapperData[k + offset] = vtkSmartPointer<vtkDataSetMapper>::New();
        mapperData[k + offset]->SetInputData(ugData[k + offset]);

        primitiveShapeActor[k + offset]->SetMapper(mapperData[k + offset]);
		primitiveShapeActor[k + offset]->GetProperty()->SetColor(shapeColor[0] / 255.0, shapeColor[1] / 255.0, shapeColor[2] / 255.0);
    }


    for(size_t n = 0; n < primitiveShapeActor.size(); n++)
    {
        renderer->AddActor(primitiveShapeActor[n]);
    }
    ui->openGLWidget->GetRenderWindow()->Render();

	QMessageBox::StandardButton result = QMessageBox::question(this, "Conversion", "Convert this MOD file to STL file",
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (result == QMessageBox::Yes)
	{
		conversion(&loadMOD);
	}

    // disable some functions
    ui->intensity->setEnabled(false);
    ui->intensitySlider->setEnabled(false);
    ui->removeLight->setEnabled(false);
    ui->changeLightColourButton->setEnabled(false);
    ui->objectColor->setEnabled(false);
    ui->edgeCheck->setEnabled(false);
    ui->noFilter->setEnabled(false);
    ui->clipfilter->setEnabled(false);
    ui->shrinkfilter->setEnabled(false);
    ui->curvatureFilter->setEnabled(false);
    ui->smoothFilter->setEnabled(false);
    ui->resetCameraButton->setEnabled(true);
}
// This function will display the orientation widget
void MainWindow::displayOrientationWidget(bool checked)
{
    if(checked)
    {
        orientationMarker->SetEnabled( 1 );
        orientationMarker->InteractiveOff(); //This stops the widget to be moved by the user -- I feel it is a little too obtrusive otherwise
    }
    else
        orientationMarker->SetEnabled( 0 );
    ui->openGLWidget->GetRenderWindow()->Render();
}
// This function displays the plane widget
void MainWindow::displayPlaneWidget(bool checked)
{
    if(checked)
    {
        planeWidgetCallback->Plane = plane;
        planeWidgetCallback->Actor = actor;
        planeWidget->SetRepresentation(rep);
        planeWidget->AddObserver(vtkCommand::InteractionEvent,planeWidgetCallback);
        planeWidget->On();
    }
    else
        planeWidget->Off();
    ui->openGLWidget->GetRenderWindow()->Render();
}
// This function displays the box widget
void MainWindow::displayBoxWidget(bool checked)
{
    if(checked)
    {
        vtkSmartPointer<vtkActor> pActor = nullptr;
        if (actor != nullptr)
        {
            pActor = actor;
        }
        else if (shapeActor != nullptr)
        {
            pActor = shapeActor;
        }
        else
        {
            return;
        }
       boxWidget->GetRepresentation()->SetPlaceFactor( 1 ); // Default is 0.5
       boxWidget->GetRepresentation()->PlaceWidget(pActor->GetBounds());
       boxWidgetCallback->SetActor(pActor);
       boxWidget->AddObserver( vtkCommand::InteractionEvent, boxWidgetCallback );
       boxWidget->On();
    }
    else
    {
       boxWidget->Off();
    }
    ui->openGLWidget->GetRenderWindow()->Render();
}
// this function would set color of the light
void MainWindow::setLightColor()
{

    double redComponent, greenComponent, blueConponent;
    // obtain the color selected by user
    QColor selectedColor = QColorDialog::getColor(Qt::white, this,
                                           "Select the object color", QColorDialog::ShowAlphaChannel);
    // check the validity of the selected color
    if(!selectedColor.isValid())
    {
        return ;
    }

    // calculate the RGB conponents
    redComponent = selectedColor.red() / 255.0;
    greenComponent = selectedColor.green() / 255.0;
    blueConponent = selectedColor.blue() / 255.0;

        light->SetColor(redComponent,greenComponent,blueConponent);
        // add the light to the renderer
        renderer->AddLight(light);
        ui->openGLWidget->GetRenderWindow()->Render();

        // enable reset light operation
        ui->removeLight->setEnabled(true);
}

// this function would set intensity of the light
void MainWindow::setLightIntensitySpinBox()
{
    // obtain the value fron the spin box
    intensity = ui->intensity->text().toDouble();
    ui->intensitySlider->setValue(intensity * 10);

    // set light intensity
    light->SetIntensity(intensity);
    // add new light source to renderer
    renderer->AddLight(light);
    ui->openGLWidget->GetRenderWindow()->Render();

    // enable reset light operation
    ui->removeLight->setEnabled(true);
}

void MainWindow::setLightIntensitySlider()
{
    // obtain the value fron the slider
    intensity = ui->intensitySlider->value() / 10.0;
    ui->intensity->setValue(intensity);

    // set light intensity
    light->SetIntensity(intensity);
    // add new light source to renderer
    renderer->AddLight(light);
    ui->openGLWidget->GetRenderWindow()->Render();

    // enable reset light operation
    ui->removeLight->setEnabled(true);
}

// this function would reset light
void MainWindow::resetLight()
{
    // remove all the lights
    renderer->RemoveAllLights();
    ui->openGLWidget->GetRenderWindow()->Render();

    // disable reset light operation
    ui->removeLight->setEnabled(false);
}

// this function can change the object color to a user selected one
void MainWindow::selectedObjectColor()
{
    double redComponent, greenComponent, blueConponent;
    // obtain the color selected by user
    QColor selectedColor = QColorDialog::getColor(Qt::white, this,
                                           "Select the object color", QColorDialog::ShowAlphaChannel);
    // check the validity of the selected color
    if(!selectedColor.isValid())
    {
        return ;
    }

    // calculate the RGB conponents
    redComponent = selectedColor.red() / 255.0;
    greenComponent = selectedColor.green() / 255.0;
    blueConponent = selectedColor.blue() / 255.0;

    // render the actor using the new color
    actor->GetProperty()->SetColor(redComponent, greenComponent, blueConponent);
    ui->openGLWidget->GetRenderWindow()->Render();
}

// this function could set the edge visibility of the object
void MainWindow::visableEdge(int checked)
{
    // if the check box is checked, show edge
    if(checked)
    {
        actor->GetProperty()->EdgeVisibilityOn();
    }
    else
    {
        actor->GetProperty()->EdgeVisibilityOff();
    }

    ui->openGLWidget->GetRenderWindow()->Render();
}

// this function could set background color
void MainWindow::setBackgroundColor()
{
    double redComponent, greenComponent, blueConponent;
    // obtain the color selected by user
    QColor selectedColor = QColorDialog::getColor(Qt::white, this,
                                           "Select the object color", QColorDialog::ShowAlphaChannel);

    if(!(selectedColor.isValid())) //If no color selected function returns before any changes are made.
        return;

    // calculate the RGB conponents
    redComponent = selectedColor.red() / 255.0;
    greenComponent = selectedColor.green() / 255.0;
    blueConponent = selectedColor.blue() / 255.0;

    // render the new background
    renderer->SetBackground(redComponent, greenComponent, blueConponent);
    ui->openGLWidget->GetRenderWindow()->Render();
}

// this function could apply filter settings
void MainWindow::applyFilter(int buttonID)
{
    ui->editFilterButton->setEnabled(true);
    ui->actionDisplayPlaneWidget->setEnabled(false);
    renderer->RemoveActor(scalarBar);
    ui->actionDisplayPlaneWidget->setChecked(false);
    ui->actionDisplayBoxWidget->setChecked(false);
    ui->actionDisplayBoxWidget->setEnabled(true);
    switch(buttonID)
    {
    case 0:
    {
        mapper->SetInputConnection(STLReader->GetOutputPort());
        actor->SetMapper(mapper);
        ui->editFilterButton->setEnabled(false);
        break;
    }
    // apply clip filter
    case 1:
    {
        ui->actionDisplayPlaneWidget->setEnabled(true);
        ui->actionDisplayBoxWidget->setEnabled(false);
        rep->SetPlaceFactor(1.25);
        rep->PlaceWidget(actor->GetBounds());
        rep->SetNormal(plane->GetNormal());
        vtkSmartPointer<vtkClipDataSet> filter = vtkSmartPointer<vtkClipDataSet>::New();
        filter->SetInputConnection(STLReader->GetOutputPort());
        filter->SetClipFunction(plane.Get());
        mapper->SetInputConnection(filter->GetOutputPort());
        actor->SetMapper(mapper);
        break;
    }
    // apply shrink filter
    case 2:
    {
        shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
        mapper->SetInputConnection(shrinkFilter->GetOutputPort());
        shrinkFilter->SetInputConnection(STLReader->GetOutputPort());
        shrinkFilter->SetShrinkFactor(0.9);
        shrinkFilter->Update();

        actor->SetMapper(mapper);
        break;
    }
    //apply smooth filter
    case 3:
    {
        vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
        smoothFilter->SetInputConnection(STLReader->GetOutputPort());
        smoothFilter->SetNumberOfIterations(15);
        smoothFilter->SetRelaxationFactor(0.1);
        smoothFilter->FeatureEdgeSmoothingOff();
        smoothFilter->BoundarySmoothingOn();
        smoothFilter->Update();
        vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
        normalGenerator->SetInputConnection(smoothFilter->GetOutputPort());
        normalGenerator->ComputePointNormalsOn();
        normalGenerator->ComputeCellNormalsOn();
        normalGenerator->Update();

        mapper->SetInputConnection(normalGenerator->GetOutputPort());
        actor->SetMapper(mapper);
        ui->editFilterButton->setEnabled(false);
        break;
    }
    //apply curvature filter
    case 4:
    {
        curvatureFilter = vtkSmartPointer<vtkCurvatures>::New();
        curvatureFilter->SetInputConnection(STLReader->GetOutputPort());
        curvatureFilter->SetCurvatureTypeToGaussian();
        previousType = 2;
        curvatureFilter->Update();
        double scalarRange[2];
        curvatureFilter->GetOutput()->GetScalarRange(scalarRange);
        previousMinLim = int(scalarRange[0]);
        previousMaxLim = int(scalarRange[1]);

        // Build a lookup table
        int scheme = 32; //selects colour scheme 32
        previousScheme = scheme;
        vtkSmartPointer<vtkColorSeries> colorSeries = vtkSmartPointer<vtkColorSeries>::New();
        colorSeries->SetColorScheme(scheme);

        vtkSmartPointer<vtkColorTransferFunction> lut = vtkSmartPointer<vtkColorTransferFunction>::New();
        lut->SetColorSpaceToHSV();

        // Use a color series to create a transfer function
        int numColors = colorSeries->GetNumberOfColors();
        for (int i = 0; i < numColors; i++)
        {
            vtkColor3ub color = colorSeries->GetColor(i);
            double dColor[3];
            dColor[0] = static_cast<double> (color[0]) / 255.0;
            dColor[1] = static_cast<double> (color[1]) / 255.0;
            dColor[2] = static_cast<double> (color[2]) / 255.0;
            double t = scalarRange[0] + (scalarRange[1] - scalarRange[0]) / (numColors - 1) * i;
            lut->AddRGBPoint(t, dColor[0], dColor[1], dColor[2]);
        }
        mapper->SetInputConnection(curvatureFilter->GetOutputPort());
        mapper->SetLookupTable(lut);
        mapper->SetScalarRange(scalarRange);
        actor->SetMapper(mapper);

        scalarBar =  vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar->SetLookupTable(mapper->GetLookupTable());
        scalarBar->SetTitle(curvatureFilter->GetOutput()->GetPointData()->GetScalars()->GetName());
        scalarBar->SetNumberOfLabels(5);

        renderer->AddActor2D(scalarBar);
        break;
    }
    }

    ui->openGLWidget->GetRenderWindow()->Render();
}

// this function could add one of the primitive shapes
void MainWindow::primitiveShape(int checked)
{
    //remove box widget
    ui->actionDisplayBoxWidget->setChecked(false);
    ui->actionDisplayBoxWidget->setEnabled(true);

    // remove all the actors
    renderer->RemoveActor(actor);
    renderer->RemoveActor(scalarBar);
    for(shapeItor = primitiveShapeActor.begin(); shapeItor != primitiveShapeActor.end(); shapeItor++)
    {
        renderer->RemoveActor(*shapeItor);
    }
    // remove light
    renderer->RemoveLight(light);

	if(shapeActor == nullptr)
		shapeActor = vtkSmartPointer<vtkActor>::New();

    // render primitive shape selected by user
    switch(checked)
    {
    case 0:
    {
        renderer->RemoveActor(shapeActor);
        //update model statistics
        ui->pointLabel->setText(QString("-"));
        ui->cellLabel->setText(QString("-"));
        break;
    }
    // render pyramid
    case 1:
    {
        vtkSmartPointer<vtkPyramid> pyramid = vtkSmartPointer<vtkPyramid>::New();
        vtkSmartPointer<vtkPoints> point = vtkSmartPointer<vtkPoints>::New();
        float p0[3] = {1, 1, 1};
        float p1[3] = {-1, 1, 1};
        float p2[3] = {-1, -1, 1};
        float p3[3] = {1, -1, 1};
        float p4[3] = {0, 0, 0};
        float* data[5] = {p0, p1, p2, p3, p4};

        for(size_t i = 0; i < 5; i++)
        {
            point->InsertNextPoint(data[i]);
            pyramid->GetPointIds()->SetId(i, i);
        }

        vtkSmartPointer<vtkCellArray> cell = vtkSmartPointer<vtkCellArray>::New();
        cell->InsertNextCell(pyramid);

        vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
        ug->SetPoints(point);
        ug->InsertNextCell(pyramid->GetCellType(), pyramid->GetPointIds());

        vtkSmartPointer<vtkDataSetMapper> pyramidMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        pyramidMapper->SetInputData(ug);

        shapeActor->SetMapper(pyramidMapper);
        renderer->AddActor(shapeActor);

        //update model statistics
        ui->pointLabel->setText(QVariant(pyramidMapper->GetInput()->GetNumberOfPoints()).toString());
        ui->cellLabel->setText(QVariant(pyramidMapper->GetInput()->GetNumberOfCells()).toString());

        break;
    }
    // render tetrahedron
    case 2:
    {
        vtkSmartPointer<vtkTetra> tetra = vtkSmartPointer<vtkTetra>::New();
        vtkSmartPointer<vtkPoints> point = vtkSmartPointer<vtkPoints>::New();

        point->InsertNextPoint(0, 0, 0);
        point->InsertNextPoint(1, 0, 0);
        point->InsertNextPoint(0, 1, 0);
        point->InsertNextPoint(0, 0, 1);

        for(size_t i = 0; i < 4; i++)
        {
            tetra->GetPointIds()->SetId(i, i);
        }

        vtkSmartPointer<vtkCellArray> cell = vtkSmartPointer<vtkCellArray>::New();
        cell->InsertNextCell(tetra);

        vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
        ug->SetPoints(point);
        ug->SetCells(VTK_TETRA, cell);

        vtkSmartPointer<vtkDataSetMapper> tetraMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        tetraMapper->SetInputData(ug);

        shapeActor->SetMapper(tetraMapper);
        renderer->AddActor(shapeActor);

        //update model statistics
        ui->pointLabel->setText(QVariant(tetraMapper->GetInput()->GetNumberOfPoints()).toString());
        ui->cellLabel->setText(QVariant(tetraMapper->GetInput()->GetNumberOfCells()).toString());

        break;
    }
    // render hexahedron
    case 3:
    {
        vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();
        vtkSmartPointer<vtkPoints> point = vtkSmartPointer<vtkPoints>::New();
        float p0[3] = {0, 0, 0};
        float p1[3] = {1, 0, 0};
        float p2[3] = {1, 1, 0};
        float p3[3] = {0, 1, 0};
        float p4[3] = {0, 0, 1};
        float p5[3] = {1, 0, 1};
        float p6[3] = {1, 1, 1};
        float p7[3] = {0, 1, 1};
        float* data[8] = {p0, p1, p2, p3, p4, p5, p6, p7};

        for(size_t i = 0; i < 8; i++)
        {
            point->InsertNextPoint(data[i]);
            hex->GetPointIds()->SetId(i, i);
        }

        vtkSmartPointer<vtkCellArray> cell = vtkSmartPointer<vtkCellArray>::New();
        cell->InsertNextCell(hex);

        vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
        ug->SetPoints(point);
        ug->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

        vtkSmartPointer<vtkDataSetMapper> hexMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        hexMapper->SetInputData(ug);

        shapeActor->SetMapper(hexMapper);
        renderer->AddActor(shapeActor);

        //update model statistics
        ui->pointLabel->setText(QVariant(hexMapper->GetInput()->GetNumberOfPoints()).toString());
        ui->cellLabel->setText(QVariant(hexMapper->GetInput()->GetNumberOfCells()).toString());

        break;
    }
    }

    ui->openGLWidget->GetRenderWindow()->Render();

    // disable some functions
    ui->intensity->setEnabled(false);
    ui->intensitySlider->setEnabled(false);
    ui->removeLight->setEnabled(false);
    ui->changeLightColourButton->setEnabled(false);
    ui->objectColor->setEnabled(false);
    ui->edgeCheck->setEnabled(false);
    ui->noFilter->setEnabled(false);
    ui->clipfilter->setEnabled(false);
    ui->shrinkfilter->setEnabled(false);
    ui->curvatureFilter->setEnabled(false);
    ui->smoothFilter->setEnabled(false);
    ui->resetCameraButton->setEnabled(true);

    actor = nullptr;
    // reset all other functions
    ui->noFilter->setChecked(true);
    ui->edgeCheck->setChecked(false);

	if (ui->actionDisplayBoxWidget->isChecked() == true)
	{

	}

	resetCamera();
}

// this function could reset camera
void MainWindow::resetCamera()
{
	vtkSmartPointer<vtkRenderer> pRenderer = ui->openGLWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
	vtkSmartPointer<vtkCamera> pCamera = pRenderer->GetActiveCamera();

	pCamera->SetFocalPoint(0.0,0.0,0.0);
	pCamera->SetPosition(0, 0, 1);
	pCamera->SetViewUp(0, 1, 0);
	//ui->openGLWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->Modified();
	pRenderer->ResetCamera();
    ui->openGLWidget->GetRenderWindow()->Render();
}

void MainWindow::conversion(Model* loadMOD)
{
	QString filename = QFileDialog::getSaveFileName(this, "Save file", "./", "STL file (*.stl)");

	vtkSmartPointer<vtkSTLWriter> write = vtkSmartPointer<vtkSTLWriter>::New();
	write->SetFileName(filename.toStdString().c_str());

        vector < vtkSmartPointer<vtkTriangle>> triangleVector;
	size_t cellNum = loadMOD->getHex().size() + loadMOD->getPyramid().size() + loadMOD->getTetra().size();
	int triangleNum = 0;
	vector<Tetrahedron> tetrData = loadMOD->getTetra();
	vector<Pyramid> pyramidData = loadMOD->getPyramid();
	vector<Hexahedron> hexData = loadMOD->getHex();
	vector<Vector> vectorData = loadMOD->getVector();

	pointData->Initialize();

	for (size_t i = 0; i < loadMOD->getVector().size(); i++)
	{
		double vertex[3] = { loadMOD->getVector()[i].get_i(), loadMOD->getVector()[i].get_j(), loadMOD->getVector()[i].get_k() };
		pointData->InsertNextPoint(vertex);
	}

	for (size_t i = 0; i < loadMOD->getTetra().size(); i++)
	{
		vtkSmartPointer<vtkTriangle> triangle0 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle0);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, tetrData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, tetrData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, tetrData[i].getVectorNumber(vectorData, 2));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle1 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle1);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, tetrData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, tetrData[i].getVectorNumber(vectorData, 2));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, tetrData[i].getVectorNumber(vectorData, 3));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle2 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle2);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, tetrData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, tetrData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, tetrData[i].getVectorNumber(vectorData, 3));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle3 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle3);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, tetrData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, tetrData[i].getVectorNumber(vectorData, 2));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, tetrData[i].getVectorNumber(vectorData, 3));
		cell->InsertNextCell(triangleVector[triangleNum++]);
	}

	for (size_t i = 0; i < loadMOD->getPyramid().size(); i++)
	{
		vtkSmartPointer<vtkTriangle> triangle0 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle0);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, pyramidData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, pyramidData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, pyramidData[i].getVectorNumber(vectorData, 4));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle1 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle1);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, pyramidData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, pyramidData[i].getVectorNumber(vectorData, 2));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, pyramidData[i].getVectorNumber(vectorData, 4));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle2 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle2);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, pyramidData[i].getVectorNumber(vectorData, 2));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, pyramidData[i].getVectorNumber(vectorData, 3));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, pyramidData[i].getVectorNumber(vectorData, 4));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle3 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle3);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, pyramidData[i].getVectorNumber(vectorData, 3));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, pyramidData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, pyramidData[i].getVectorNumber(vectorData, 4));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle4 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle4);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, pyramidData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, pyramidData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, pyramidData[i].getVectorNumber(vectorData, 3));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle5 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle5);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, pyramidData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, pyramidData[i].getVectorNumber(vectorData, 2));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, pyramidData[i].getVectorNumber(vectorData, 3));
		cell->InsertNextCell(triangleVector[triangleNum++]);
	}

	for (size_t i = 0; i < loadMOD->getHex().size(); i++)
	{
		vtkSmartPointer<vtkTriangle> triangle0 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle0);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 5));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle1 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle1);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 4));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 5));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle2 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle2);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 2));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 6));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle3 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle3);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 5));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 6));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle4 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle4);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 2));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 3));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 7));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle5 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle5);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 2));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 6));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 7));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle6 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle6);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 3));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 4));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle7 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle7);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 3));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 7));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 4));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle8 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle8);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 1));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 2));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle9 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle9);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 0));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 3));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 2));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle10 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle10);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 4));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 5));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 6));
		cell->InsertNextCell(triangleVector[triangleNum++]);

		vtkSmartPointer<vtkTriangle> triangle11 = vtkSmartPointer<vtkTriangle>::New();
		triangleVector.push_back(triangle11);
		triangleVector[triangleNum]->GetPointIds()->SetId(0, hexData[i].getVectorNumber(vectorData, 4));
		triangleVector[triangleNum]->GetPointIds()->SetId(1, hexData[i].getVectorNumber(vectorData, 7));
		triangleVector[triangleNum]->GetPointIds()->SetId(2, hexData[i].getVectorNumber(vectorData, 6));
		cell->InsertNextCell(triangleVector[triangleNum++]);
	}

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->Initialize();
	polyData->SetPolys(cell);
	polyData->SetPoints(pointData);

	write->SetInputData(polyData);
	write->Update();
	write->Write();
}
