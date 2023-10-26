#include "dataProcess.h"
#include "TransformData.h"

vtkStandardNewMacro(MyDataProcessInteractor);
std::vector<double> cubeDimension;


static void KeypressCallbackFunction ( vtkObject* caller, long unsigned int eventId,
          void* clientData, void* callData );

static void CreateImage(vtkSmartPointer<vtkImageData> image,unsigned char *color1,unsigned char *color2);


DataProcess::DataProcess()
{
	widget.setupUi(this);
	cubeDimension.clear();
	for(unsigned int i=0;i<6;i++){
		cubeDimension.push_back((i%2)*20);
	}
	
	this->widget.comboBox->addItem("obj file");
	this->widget.comboBox->addItem("dicom dir");
	this->widget.comboBox->addItem("raw file");
	this->widget.comboBox->setCurrentIndex(0);
}

DataProcess::~DataProcess()
{

}

void DataProcess::OpenFile()
{
	/***Open data according the combo box value***/
	QFileDialog *dialog = new QFileDialog;
	if(this->widget.comboBox->currentIndex()==0){
		QString file = dialog->getOpenFileName(this,"obj file","open file dialog",tr("Images (*.obj)"));
		if(file.isNull()){
			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
		}else{
			this->data = this->in->ReadOBJ(file.toStdString().c_str());
			this->Visualize(this->data->GetProducerPort());
			widget.addObject->setEnabled(true);
			widget.extractLarge->setEnabled(true);
			widget.ExtractSurface->setEnabled(false);
			widget.FillHoles->setEnabled(true);
			widget.clipping->setEnabled(true);
			widget.Transform->setEnabled(true);
			widget.Export->setEnabled(true);
		}
	}else if(this->widget.comboBox->currentIndex()==1){
		QString file = dialog->getExistingDirectory(this,tr("Open dicom Directory"),"DICOM Folder",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
		if(file.isNull()){
			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
		}else{
			this->widget.addObject->setEnabled(false);
			this->widget.extractLarge->setEnabled(false);
			this->widget.ExtractSurface->setEnabled(true);
			this->widget.FillHoles->setEnabled(false);
			this->widget.Transform->setEnabled(false);
			this->widget.clipping->setEnabled(true);
			this->widget.Export->setEnabled(true);
			this->dicom = this->in->ReadDicomDirectory(file,false);
			this->ReadDicomDir(this->dicom,0);
		}
	}else{
		QString file = dialog->getOpenFileName(this,"raw file","",tr("image(*.raw)"));
		if(file.isNull()){
			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
		}else{
			dataDimension* dialog = new dataDimension;
			dialog->raise();
			if(dialog->exec() == QDialog::Accepted){
				vtkSmartPointer<vtkImageReader> ImageReader = vtkSmartPointer<vtkImageReader>::New();
				ImageReader->SetFileName(file.toStdString().c_str());
				ImageReader->SetDataExtent(0,dialog->DataDimensionVector()[0]-1,0,dialog->DataDimensionVector()[1]-1,0,dialog->DataDimensionVector()[2]-1);
				ImageReader->SetDataSpacing(dialog->VoxelDimensionVector()[0],dialog->VoxelDimensionVector()[1],dialog->VoxelDimensionVector()[2]);
				ImageReader->SetFileDimensionality(3);
				ImageReader->SetDataByteOrderToLittleEndian();
				if(dialog->dataType()){
					ImageReader->SetDataScalarTypeToShort();
				}else{
					ImageReader->SetDataScalarTypeToDouble();
				}
				ImageReader->Update();
				this->dicom = ImageReader->GetOutput();
				if(dialog->Tomography()){
					this->ReadDicomDir(this->dicom,1);
				}else{
					this->ReadDicomDir(this->dicom,2);
				}
				this->widget.addObject->setEnabled(false);
				this->widget.extractLarge->setEnabled(false);
				this->widget.ExtractSurface->setEnabled(true);
				this->widget.FillHoles->setEnabled(false);
				this->widget.Transform->setEnabled(false);
				this->widget.clipping->setEnabled(true);
				this->widget.Export->setEnabled(true);
			}
		}
	}
}

/********************* Read data ********************/
void DataProcess::ReadDicomDir(vtkImageData* img,int type)
{
	double range[2];
	img->GetScalarRange(range);
	vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	contourFilter->SetInput(img);
	contourFilter->GenerateValues(1,range[0],range[1]);

	// Map the contours to graphical primitives
	vtkSmartPointer<vtkPolyDataMapper> contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	contourMapper->SetInputConnection(contourFilter->GetOutputPort());
	
	// Create an actor for the contours
	vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();
	contourActor->SetMapper(contourMapper);
	// Create the outline
	vtkSmartPointer<vtkOutlineFilter> outlineFilter = vtkSmartPointer<vtkOutlineFilter>::New();
	outlineFilter->SetInput(img);
	
	vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	outlineMapper->SetInputConnection(outlineFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outlineMapper);


	this->TypeOfData = 1; //dicom series

	vtkSmartPointer<MyDataProcessInteractor> style = vtkSmartPointer<MyDataProcessInteractor>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
	renderer->AddVolume(this->in->GetVolume(img,type));
	renderer->AddActor(contourActor);
	renderer->AddActor(outlineActor);

	style->SetRENDERER(renderer);
	renderer->AddActor2D(this->in->ImageInfo(this->dicom));
	renderer->SetBackground(0.9,0.9,0.9);
	renderer->GradientBackgroundOn();

	/*Set camera position*/
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCamera();

	this->widget.qvtkWidget->SetRenderWindow(renderWindow);
	this->widget.qvtkWidget->GetInteractor()->GetInteractorStyle()->SetCurrentRenderer(renderer);
	this->in->Axes(this->widget.qvtkWidget->GetInteractor()->GetInteractorStyle()->GetCurrentRenderer(),this->widget.qvtkWidget->GetRenderWindow()->GetInteractor());
	this->Slider(this->widget.qvtkWidget->GetRenderWindow()->GetInteractor(),img,contourFilter);
	this->widget.qvtkWidget->GetInteractor()->SetInteractorStyle(style);
	this->widget.qvtkWidget->GetRenderWindow()->Render();
	this->widget.qvtkWidget->update();
}

void DataProcess::Visualize(vtkAlgorithmOutput* pData)
{
	this->TypeOfData = 0; /*polydata file*/
	vtkSmartPointer<vtkElevationFilter> elevationFilter = vtkSmartPointer<vtkElevationFilter>::New();
	elevationFilter->SetInputConnection(pData);
	elevationFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Mapper->SetInputConnection(elevationFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(Mapper);

	vtkSmartPointer<MyDataProcessInteractor> style = vtkSmartPointer<MyDataProcessInteractor>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
	renderer->AddActor(actor);
	renderer->AddActor2D(this->in->DataInfo(this->data));
	renderer->SetBackground(0.9,0.9,0.9);
    renderer->GradientBackgroundOn();
	style->SetRENDERER(renderer);
	renderer->AddViewProp(this->in->Annotation(15,"Press B to set a BoxWidget",2));
	renderer->ResetCamera();

	vtkSmartPointer<vtkCallbackCommand> keypressCallback =
    vtkSmartPointer<vtkCallbackCommand>::New();
 
  // Allow the observer to access the sphereSource
  //keypressCallback->SetClientData(sphereSource);
  //keypressCallback->SetCallback(KeypressCallbackFunction );



	this->widget.qvtkWidget->SetRenderWindow(renderWindow);
	this->widget.qvtkWidget->GetInteractor()->GetInteractorStyle()->SetCurrentRenderer(renderer);
	this->in->Axes(widget.qvtkWidget->GetInteractor()->GetInteractorStyle()->GetCurrentRenderer(),widget.qvtkWidget->GetRenderWindow()->GetInteractor());
	this->Button(widget.qvtkWidget->GetRenderWindow()->GetInteractor(),widget.qvtkWidget->GetInteractor()->GetInteractorStyle()->GetCurrentRenderer());
	//this->widget.qvtkWidget->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::KeyPressEvent, keypressCallback);
	this->widget.qvtkWidget->GetInteractor()->SetInteractorStyle(style);
	this->widget.qvtkWidget->GetRenderWindow()->Render();	
	this->widget.qvtkWidget->update();
}

/********************** Button Widget ***********************/
void DataProcess::ExtractLargestSurface()
{
	/*Extract the largest connected meshes of a polydata*/
	vtkSmartPointer<vtkPolyDataConnectivityFilter> conFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	conFilter->SetInput(this->data);
	conFilter->SetExtractionModeToLargestRegion();
	conFilter->Update();
	this->data=conFilter->GetOutput();
	this->Visualize(conFilter->GetOutputPort());
}

void DataProcess::ExtractSurface()
{
	/*this function is used to extract surface of a volume according the marching cube value*/
	MarchingCubesValue* dialog = new MarchingCubesValue;
	dialog->raise();
	if(dialog->exec() == QDialog::Accepted){
		this->data = this->in->ExtractSurface(this->dicom,dialog->mcValue())->GetOutput();
		this->TypeOfData = 0; /*polydata file*/
		//==============================================================================//
		double bounds[6];
		this->data->GetBounds(bounds);

		vtkSmartPointer<vtkPolyDataToImageStencil> pol2stenc = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
		pol2stenc->SetInput(this->data);
		pol2stenc->SetOutputOrigin(this->dicom->GetOrigin());
		pol2stenc->SetOutputSpacing(this->dicom->GetSpacing());
		pol2stenc->SetOutputWholeExtent(this->dicom->GetExtent());
		pol2stenc->Update();
		// cut the corresponding white image and set the background:
		vtkSmartPointer<vtkImageStencil> imgstenc = vtkSmartPointer<vtkImageStencil>::New();
		imgstenc->SetInput(this->dicom);
		imgstenc->SetStencil(pol2stenc->GetOutput());
		imgstenc->ReverseStencilOff();
		imgstenc->SetBackgroundValue(0);
		imgstenc->Update();
		//this->dicom->DeepCopy(imgstenc->GetOutput());

	  //==============================================================================//

		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		Mapper->SetInputConnection(this->data->GetProducerPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(Mapper);

		vtkSmartPointer<MyDataProcessInteractor> style = vtkSmartPointer<MyDataProcessInteractor>::New();
		vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		renderWindow->AddRenderer(renderer);
		renderer->AddActor(actor);
		renderer->SetBackground(0.9,0.9,0.9);
		renderer->GradientBackgroundOn();
		renderer->AddVolume(this->in->GetVolume(this->dicom,0));
		style->SetRENDERER(renderer);
		renderer->AddActor2D(this->in->ImageInfo(this->dicom));
		renderer->ResetCamera();

		widget.qvtkWidget->SetRenderWindow(renderWindow);
		widget.qvtkWidget->GetInteractor()->GetInteractorStyle()->SetCurrentRenderer(renderer);
		this->in->Axes(widget.qvtkWidget->GetInteractor()->GetInteractorStyle()->GetCurrentRenderer(),widget.qvtkWidget->GetRenderWindow()->GetInteractor());
		widget.qvtkWidget->GetInteractor()->SetInteractorStyle(style);
		widget.qvtkWidget->GetRenderWindow()->Render();
		widget.qvtkWidget->update();

		this->widget.addObject->setEnabled(true);
		this->widget.extractLarge->setEnabled(true);
		this->widget.FillHoles->setEnabled(true);
		this->widget.clipping->setEnabled(true);
		this->widget.Transform->setEnabled(true);
	}
}

void DataProcess::TrimCells()
{
    vtkSmartPointer<MyDataProcessInteractor> style = vtkSmartPointer<MyDataProcessInteractor>::New();
    vtkSmartPointer<vtkDataSetSurfaceFilter> te = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    te->Update(); 

    vtkExtractCells* extractCells = vtkExtractCells::New();
    extractCells->SetInputConnection(te->GetOutputPort());
    extractCells->Update();
    
    vtkSmartPointer<vtkFillHolesFilter> fillHolesFilter = vtkSmartPointer<vtkFillHolesFilter>::New();
    fillHolesFilter->SetInput(this->data);
    fillHolesFilter->Update();
    
    vtkSmartPointer<vtkDecimatePro> deci = vtkSmartPointer<vtkDecimatePro>::New();
    deci->SetInputConnection(te->GetOutputPort());
    deci->PreSplitMeshOn();
    deci->PreserveTopologyOff();
    
    vtkSmartPointer<vtkSmoothPolyDataFilter> smoother= vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    smoother->SetInputConnection(extractCells->GetOutputPort());
    smoother->SetNumberOfIterations(50);
    
    this->Visualize(fillHolesFilter->GetOutputPort());
}

void DataProcess::AddData()
{
	/*this function is used to add a polydata to the current data*/
    QFileDialog *dialog = new QFileDialog;
    QString file = dialog->getOpenFileName(this,"obj file","open file dialog",tr("Images (*.obj)"));
    if(file.isNull()){
        connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
    }else{
        vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
        input->ShallowCopy(this->data);
        vtkSmartPointer<vtkPolyData> addinput = vtkSmartPointer<vtkPolyData>::New();
		addinput->ShallowCopy(this->in->ReadOBJ(file));
        vtkSmartPointer<vtkAppendPolyData> AppendPolyData =  vtkSmartPointer<vtkAppendPolyData>::New();
        AppendPolyData->AddInputConnection(input->GetProducerPort());
        AppendPolyData->AddInputConnection(addinput->GetProducerPort());
        AppendPolyData->Update();
  
        vtkSmartPointer<vtkCleanPolyData> AppendData =  vtkSmartPointer<vtkCleanPolyData>::New();
        AppendData->SetInputConnection(AppendPolyData->GetOutputPort());
        AppendData->Update();
        
		this->data = AppendData->GetOutput();
        this->Visualize(AppendData->GetOutputPort());
    }
}

void DataProcess::Clipping()
{
	/*this function is used to clip polydata or volume according the cube dimension*/
	if(this->TypeOfData==0){/*this->TypeOfData==0*/
		vtkSmartPointer<vtkIdFilter> cellIdFilter = vtkSmartPointer<vtkIdFilter>::New();
		cellIdFilter->SetInput(this->data);
		cellIdFilter->SetCellIds(true);
		cellIdFilter->SetPointIds(false);
		cellIdFilter->SetIdsArrayName("CellIds");
		cellIdFilter->Update();

		vtkSmartPointer<vtkIdFilter> pointIdFilter = vtkSmartPointer<vtkIdFilter>::New();
		pointIdFilter->SetInputConnection(cellIdFilter->GetOutputPort());
		pointIdFilter->SetCellIds(false);
		pointIdFilter->SetPointIds(true);
		pointIdFilter->SetIdsArrayName("PointIds");
		pointIdFilter->Update();

		vtkDataSet* sphereWithIds = pointIdFilter->GetOutput();

		vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
		cubeSource->SetBounds(cubeDimension[0],cubeDimension[1],cubeDimension[2],cubeDimension[3],cubeDimension[4],cubeDimension[5]);
		cubeSource->Update();
	
		vtkSmartPointer<vtkDelaunay3D> delaunayFilter = vtkSmartPointer<vtkDelaunay3D>::New();
		delaunayFilter->SetInputConnection(cubeSource->GetOutputPort());
		delaunayFilter->BoundingTriangulationOff();
		delaunayFilter->Update();
		
		vtkSmartPointer<vtkElevationFilter> elevationFilter = vtkSmartPointer<vtkElevationFilter>::New();
		elevationFilter->SetInputConnection(delaunayFilter->GetOutputPort());
		elevationFilter->Update();

		vtkSmartPointer<vtkImplicitDataSet> implicitCube = vtkSmartPointer<vtkImplicitDataSet>::New();
		implicitCube->SetDataSet(elevationFilter->GetOutput());

		vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
		clipper->SetClipFunction(implicitCube);
		clipper->SetInput(sphereWithIds);
		//clipper->SetInputData(sphereWithIds);
		clipper->GenerateClipScalarsOn();
		clipper->Update();

		vtkPolyData* clipped = clipper->GetOutput();
		this->data = clipped;
		//vtkIdTypeArray* clippedCellIds = vtkIdTypeArray::SafeDownCast(clipped->GetCellData()->GetArray("CellIds"));

		this->Visualize(this->data->GetProducerPort());
	}else{
		/***first we need to get the image data spacing***/
		double gs[3];
		this->dicom->GetSpacing(gs);
		/**and then get the pixel representation**/
		cubeDimension[0] = cubeDimension[0] / gs[0];
		cubeDimension[1] = cubeDimension[1] / gs[0];
		cubeDimension[2] = cubeDimension[2] / gs[1];
		cubeDimension[3] = cubeDimension[3] / gs[1];
		cubeDimension[4] = cubeDimension[4] / gs[2];
		cubeDimension[5] = cubeDimension[5] / gs[2];

		for(unsigned int i=0;i<6;i++){
			cubeDimension[i] = static_cast<int>(ceil(cubeDimension[i]));
		}

		/***extract the volume of interest***/
		vtkSmartPointer<vtkExtractVOI> extractVOI = vtkSmartPointer<vtkExtractVOI>::New();
		extractVOI->SetInputConnection(this->dicom->GetProducerPort());
		extractVOI->SetVOI(cubeDimension[0],cubeDimension[1],cubeDimension[2],cubeDimension[3],cubeDimension[4],cubeDimension[5]);
		extractVOI->Update();

		this->dicom = extractVOI->GetOutput();
		this->ReadDicomDir(this->dicom,0);
	}
}

void DataProcess::Export()
{
	/*Export the data to the main window*/
	QMessageBox mb(QMessageBox::Question,"Export Data","As source or target?\n",QMessageBox::Yes | QMessageBox::No);
	mb.setButtonText(QMessageBox::Yes, "Source");
	mb.setButtonText(QMessageBox::No, "Target"); 

	switch(mb.exec()){
	case QMessageBox::Yes:
		this->in->GetCleanData(this->data,this->dicom,0,this->TypeOfData);
		break;
	case QMessageBox::No:
		this->in->GetCleanData(this->data,this->dicom,1,this->TypeOfData);
		break;
	}
}

void DataProcess::FillHoles()
{
	/*fill the hole of the polydata*/
	vtkSmartPointer<vtkFillHolesFilter> fillHolesFilter = vtkSmartPointer<vtkFillHolesFilter>::New();
	fillHolesFilter->SetInputConnection(this->data->GetProducerPort());
	fillHolesFilter->SetHoleSize(100000.0);

	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputConnection(fillHolesFilter->GetOutputPort());
	normals->ConsistencyOn();
	normals->SplittingOff();
	normals->Update();

	normals->GetOutput()->GetPointData()->SetNormals(this->data->GetPointData()->GetNormals());

	this->Visualize(normals->GetOutputPort());
}

void DataProcess::Transform()
{
	/*apply the transformation to the pokydata or a volume*/
	TransformData *dialog = new TransformData;
	dialog->raise();
	if(dialog->exec() == QDialog::Accepted){
		/***Apply the matrix & scale transformation from TransformData dialog***/
		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
		transform->Scale(dialog->ScalingValue(),dialog->ScalingValue(),dialog->ScalingValue());
		transform->Concatenate(dialog->MatrixOfTransformation());
		transform->Update();

		vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformFilter->SetInputConnection(this->data->GetProducerPort());
		transformFilter->SetTransform(transform);
		transformFilter->Update();

		this->data = transformFilter->GetOutput();
		this->Visualize(this->data->GetProducerPort());
	}
}

/*************************Utilities***************************/
void DataProcess::Slider(vtkRenderWindowInteractor *interactor, vtkImageData* img, vtkContourFilter* contour)
{
	double range[2];
	img->GetScalarRange(range);

	vtkSliderRepresentation2D* sliderRep = vtkSliderRepresentation2D::New();
	sliderRep->SetMinimumValue(range[0]);
	sliderRep->SetMaximumValue(range[1]);
	sliderRep->SetValue(range[1]);
	sliderRep->SetTitleText("Contour value");
	sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
	sliderRep->GetPoint1Coordinate()->SetValue(300,30);
	sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
	sliderRep->GetPoint2Coordinate()->SetValue(100,30);
	sliderRep->GetSliderProperty()->SetColor(0,1,0);
	//sliderRep->PlaceWidget(); use this function to place the widget
	// Change the color of the text indicating what the slider controls
	sliderRep->GetTitleProperty()->SetColor(0,0,0);
	sliderRep->GetTitleProperty()->SetFontFamilyToCourier();
	sliderRep->GetTitleProperty()->SetFontSize(8);
	sliderRep->GetTitleProperty()->SetOpacity(0.8);
	sliderRep->GetTitleProperty()->SetJustificationToCentered();
	sliderRep->GetTitleProperty()->SetVerticalJustificationToTop();
	// Change the color of the text displaying the value
	sliderRep->GetLabelProperty()->SetColor(1,1,1);
	sliderRep->GetLabelProperty()->SetFontFamilyToCourier();
	sliderRep->GetLabelProperty()->SetFontSize(14);
	// Change the color of the knob when the mouse is held on it
	sliderRep->GetSelectedProperty()->SetColor(1,0,0);
	// Change the color of the bar
	sliderRep->GetTubeProperty()->SetColor(0,0,0.1);
	// Change the color of the ends of the bar
	sliderRep->GetCapProperty()->SetColor(0,0,0.3);

	sliderRep->SetSliderWidth(.02);
	sliderRep->SetLabelHeight(.05);
 
	vtkSliderWidget* sliderWidget = vtkSliderWidget::New();
	sliderWidget->SetInteractor(interactor);
	sliderWidget->SetRepresentation(sliderRep);
	sliderWidget->SetAnimationModeToAnimate();
	sliderWidget->EnabledOn();

	vtkSmartPointer<vtkSliderCallback> callback = vtkSmartPointer<vtkSliderCallback>::New();
	callback->ContourFilter = contour;
	sliderWidget->AddObserver(vtkCommand::InteractionEvent,callback);
}

void DataProcess::Button(vtkRenderWindowInteractor *interactor,vtkSmartPointer<vtkRenderer> renderer)
{
	vtkSmartPointer<vtkImageData> image1 = vtkSmartPointer<vtkImageData>::New();
	vtkSmartPointer<vtkImageData> image2 = vtkSmartPointer<vtkImageData>::New();
	unsigned char color1[3] ={0, 0, 128};
	unsigned char color2[3] ={128, 128, 128};
	CreateImage(image1, color1, color2);
	CreateImage(image2, color2, color1);

	// Create the widget and its representation
	vtkTexturedButtonRepresentation2D* buttonRepresentation = vtkTexturedButtonRepresentation2D::New();
	buttonRepresentation->SetNumberOfStates(2);
	buttonRepresentation->SetButtonTexture(0,image1);
	buttonRepresentation->SetButtonTexture(1,image2);
	buttonRepresentation->GetProperty()->SetOpacity(0.7);

	vtkButtonWidget* buttonWidget = vtkButtonWidget::New();
	buttonWidget->SetInteractor(interactor);
	buttonWidget->SetRepresentation(buttonRepresentation);

	vtkSmartPointer<vtkCoordinate> upperRight = vtkSmartPointer<vtkCoordinate>::New();
	upperRight->SetCoordinateSystemToNormalizedDisplay();
	upperRight->SetValue(1.0,0.0);
 
	double bds[6];
	double sz = 50.0;
	bds[0] = upperRight->GetComputedDisplayValue(renderer)[0] - sz;
	bds[1] = bds[0] + sz;
	bds[2] = upperRight->GetComputedDisplayValue(renderer)[1] - sz;
	bds[3] = bds[2] + sz;
	bds[4] = bds[5] = 0.0;
 
	// Scale to 1, default is .5
	buttonRepresentation->SetPlaceFactor(1);
	buttonRepresentation->PlaceWidget(bds);
 
	buttonWidget->On();

}

void CreateImage(vtkSmartPointer<vtkImageData> image,unsigned char* color1,unsigned char* color2)
{
  // Specify the size of the image data
	image->SetDimensions(10,10,1);

	image->SetNumberOfScalarComponents(3);
	image->SetScalarTypeToUnsignedChar();

	int* dims = image->GetDimensions();
 
	// Fill the image with
	for(unsigned int y = 0;y<dims[1];y++){
		for(unsigned int x = 0;x<dims[0];x++){
			unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(x,y,0));
			if(x<5){
				pixel[0] = color1[0];
				pixel[1] = color1[1];
				pixel[2] = color1[2];
			}else{
				pixel[0] = color2[0];
				pixel[1] = color2[1];
				pixel[2] = color2[2];
			}
		}
	}
}



void KeypressCallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData) )
{
  // Prove that we can access the sphere source
  vtkSphereSource* sphereSource =
    static_cast<vtkSphereSource*>(clientData);
  std::cout << "Radius is " << sphereSource->GetRadius() << std::endl;
}


	/*
	QFileDialog *dialog = new QFileDialog;
	QString file = dialog->getSaveFileName(this, "Save data","file name",tr("Images (*.obj)"));
	if(file.isNull()){
		connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	}else{
		vtkSmartPointer<vtkOBJExporter> vtkExporter = vtkSmartPointer<vtkOBJExporter>::New();
		vtkExporter->SetFilePrefix(file.toStdString().c_str());
		vtkExporter->SetRenderWindow(widget.qvtkWidget->GetRenderWindow());
		vtkExporter->Write();
	}*/