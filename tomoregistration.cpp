#include "tomoregistration.h"
#include "settings.h"
#include "dataProcess.h"
#include "myVtkInteractorStyleImage.h"
#include "MyInteractorStyle.h"
#include "landmark.h"
#include "ITKRegistration.h"
#include "PolydataInteractorStyle.h"

std::vector<std::vector<double>> sourceLandmark;
std::vector<std::vector<double>> targetLandmark;

std::vector<std::vector<double>> sourceFiducialLandmark;
std::vector<std::vector<double>> targetFiducialLandmark;

std::vector<std::vector<double>> sourceCentroid;
std::vector<std::vector<double>> targetCentroid;

vtkStandardNewMacro(MyInteractorStyle);
vtkStandardNewMacro(myVtkInteractorStyleImage);
vtkStandardNewMacro(PolydataInteractorStyle);

void AlignFrames(Frame sourceFrame,Frame targetFrame,vtkTransform* transform);
void AlignLandmarkFrame(Frame2 sourceFrame,Frame2 targetFrame,vtkTransform* transform);
vtkActor2D* Matrix(vtkSmartPointer<vtkMatrix4x4> MAT);

TomoRegistration::TomoRegistration(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	widget.setupUi(this);
	setWindowState(Qt::WindowMaximized);
	//Initialization of fiducial landmarks
	targetFiducialLandmark.clear();
	sourceFiducialLandmark.clear();
	sourceLandmark.clear();
	targetLandmark.clear();
	sourceCentroid.clear();
	targetCentroid.clear();

	std::vector<double> V;
	for(unsigned int i=0; i<6; i++){
		V.push_back(0);
	}
	for(unsigned int j=0; j<4; j++){
		targetFiducialLandmark.push_back(V);
		sourceFiducialLandmark.push_back(V);
	}
	/*Add a progress bar & combo box*/
	this->progressBar = new QProgressBar(widget.statusBar);
	this->progressBar->setMaximumSize(250,19);
	this->progressBar->setValue(0);
	this->progressBar->setMaximum(100);
	this->progressBar->setAlignment(Qt::AlignCenter);
	this->comboBox = new QComboBox;
	this->comboBox->addItem("Profile - Profile");
	this->comboBox->addItem("Profile - Tomo");
	this->comboBox->addItem("Fiducial Landmark");
	this->comboBox->setCurrentIndex(2);
	
	this->widget.toolBar->addWidget(this->comboBox);
	this->widget.toolBar->addAction(this->widget.actionApply);
	this->widget.toolBar->addAction(this->widget.actionReset);
	widget.statusBar->addWidget(this->widget.toolBar);
	//this->widget.statusBar->addWidget(this->comboBox);
	widget.statusBar->addPermanentWidget(this->widget.DicomSeriesToolBar);
	widget.statusBar->addPermanentWidget(progressBar,1);
	
	this->Reset();
	this->DICOM = NULL;
	this->DICOMS = NULL;
	this->RawReader = vtkSmartPointer<vtkImageReader>::New();

	/*set qt signal connection*/
	this->Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
	//vtk-Qt signal
	//this->Connections->Connect(this->widget.qvtkWidget1->GetRenderWindow()->GetInteractor(),vtkCommand::KeyPressEvent,this,SLOT(slot_clicked(vtkObject*,unsigned long,void*,void*)));
	//this->Connections->Connect(this->widget.qvtkWidget2->GetRenderWindow()->GetInteractor(),vtkCommand::KeyPressEvent,this,SLOT(slot_clicked(vtkObject*,unsigned long,void*,void*)));
	//this->Connections->Connect(this->widget.qvtkWidget1->GetRenderWindow()->GetInteractor(),vtkCommand::KeyPressEvent,this,SLOT(slot_Box(vtkObject*,unsigned long,void*,void*)));
	//this->Connections->Connect(this->widget.qvtkWidget2->GetRenderWindow()->GetInteractor(),vtkCommand::KeyPressEvent,this,SLOT(slot_Box(vtkObject*,unsigned long,void*,void*)));
}

TomoRegistration::~TomoRegistration()
{

}

/*********************OPEN DATA******************/
void TomoRegistration::OpenSource()
{
	/*Open source file according the current combobox index.*/
	QFileDialog* dialog = new QFileDialog;
	switch(this->comboBox->currentIndex())
	{
	case 0:
		{
			/*this case is used for profile-profile registration.
			The data is an obj file.*/
			QString file = dialog->getOpenFileName(this,"obj file","",tr("image(*.obj)"));
			if(file.isNull()){
				connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
			}else{
				this->DATA = this->ReadOBJ(file); // set the source object file
				this->ReadSurface(this->DATA,this->widget.qvtkWidget1);
			}
		}
		break;
	case 1:
		{
			/*this case is used for profile-tomo registration.
			The data is an obj file.*/
			QString file = dialog->getOpenFileName(this,"obj file","",tr("image(*.obj)"));
			if(file.isNull()){
				connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
			}else{
				this->DATA = this->ReadOBJ(file); // set the source object file
				this->ReadSurface(this->DATA,this->widget.qvtkWidget1);
			}
		}
		break;
	case 2:
		{
			/*this case is used for surface registration of an object file with CT,MRI or PET.
			The data can be a series of dicom file or a raw file.*/
			QSettings settings("_settings.ini",QSettings::IniFormat);
			settings.beginGroup("Fiducial_Landmark");
			if(settings.value("sourceRawFile").toBool()==true){
				QString File = dialog->getOpenFileName(this,"raw file","",tr("image(*.raw)"));
				if(File.isNull()){
					connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
				}else{
					dataDimension* d = new dataDimension;
					d->raise();
					if(d->exec() == QDialog::Accepted){
						this->ReadRawImages(File,d->DataDimensionVector(),d->VoxelDimensionVector(),d->dataType(),this->widget.qvtkWidget1);
					}
				}
			}else{
				//For series of Dicom images (PET)
				QString file = dialog->getExistingDirectory(this,tr("Open dicom Directory"),"DICOM Folder",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
				if(file.isNull()){
					connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
				}else{
					this->DICOMS = this->ReadDicomDirectory(file,false); //set the source image data
					this->DisplayVolume(this->DICOMS,"3D model from dicom series",this->widget.qvtkWidget1,0);
				}
			}
			settings.endGroup();
		}
		break;
	}
}

void TomoRegistration::OpenTarget()
{
	QSettings settings("_settings.ini",QSettings::IniFormat);
	QFileDialog* dialog = new QFileDialog;
	switch(this->comboBox->currentIndex())
	{
	case 0:
		{
			/*this case is used for profile-profile registration.
			The data is an obj file.*/
			QString file = dialog->getOpenFileName(this,"obj file","",tr("Image(*.obj)"));
			if(file.isNull()){
			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
			}else{
				this->DATA2 = this->ReadOBJ(file);//set the target object file
				this->ReadSurface(this->DATA2,this->widget.qvtkWidget2);
			}
		}
		break;
	case 1:
		{
			/*this case is used for surface registration of an object file with CT,MRI or PET.
			The data can be a series of dicom file or a raw file.*/
			settings.beginGroup("Profile/Profile-Profile/PET");
			if(settings.value("ProfileTomoRaw").toBool()==true){
				QString File = dialog->getOpenFileName(this,"raw file","",tr("image(*.raw)"));
				if(File.isNull()){
					connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
				}else{
					dataDimension* d = new dataDimension;
					d->raise();
					if(d->exec() == QDialog::Accepted){
						this->ReadRawImages(File,d->DataDimensionVector(),d->VoxelDimensionVector(),d->dataType(),this->widget.qvtkWidget1);
					}
				}
			}else{
				QString file = dialog->getExistingDirectory(this,tr("Open dicom Directory"),"DICOM Folder",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
				if(file.isNull()){
					connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
				}else{
					this->DICOM = this->ReadDicomDirectory(file,false);
					this->PaulViola(this->DICOM,this->DICOMS);
					this->ReadDicomSeries(this->DICOM,this->widget.qvtkWidget2);
				}
			}
			settings.endGroup();
		}
		break;
	case 2:
		{
			/*this case is used for fiducial landmarks registration. 
			The data can be a series of dicom file or a raw file.*/
			settings.beginGroup("Fiducial_Landmark");
			if(settings.value("targetRawFile").toBool()==true){
				QString File = dialog->getOpenFileName(this,"raw file","",tr("image(*.raw)"));
				if(File.isNull()){
					connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
				}else{
					dataDimension* d = new dataDimension;
					d->raise();
					if(d->exec() == QDialog::Accepted){
						this->ReadRawImages(File,d->DataDimensionVector(),d->VoxelDimensionVector(),d->dataType(),this->widget.qvtkWidget2);
					}
				}
			}else{
				/*in the case of PET images*/
				QString file = dialog->getExistingDirectory(this,tr("Open dicom Directory"),"DICOM Folder",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
				if(file.isNull()){
					connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
				}else{
					this->DICOM = this->ReadDicomDirectory(file,true);
					this->DisplayVolume(this->DICOM,"3D model from dicom series",this->widget.qvtkWidget2,0);
				}
			}
			settings.endGroup();
		}
		break;
	}
}

/************Read & Display Data*****************/
void TomoRegistration::ReadSurface(vtkPolyData* polydata,QVTKWidget* qwin)
{
	/*Read a polydata through the vtk pipeline
	qwin = qvtkWidget*/
	/*Sphere source used for landmark*/
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetRadius(0);
	sphere->SetCenter(0.0,0.0,0.0);
	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInput(sphere->GetOutput());
	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	sphereActor->SetMapper(sphereMapper);
	sphereActor->PickableOff();

    vtkSmartPointer<vtkPolyDataMapper> SurfaceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	SurfaceMapper->SetInput(polydata);
	/*Change the color for target image*/
    vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
    Actor->SetMapper(SurfaceMapper);
	if(qwin == widget.qvtkWidget2){
		Actor->GetProperty()->SetColor(1.0,0.27,0.0);
	}
	/*Apply a trackball camera interactor*/
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
    renderer->AddActor(Actor);
	renderer->AddActor(sphereActor);
	renderer->AddActor2D(this->DataInfo(polydata));
    renderer->AutomaticLightCreationOn();
	renderer->SetBackground(0.9,0.9,0.9);
	renderer->GradientBackgroundOn();

	vtkPointPicker *picker = vtkPointPicker::New();
	picker->SetTolerance(0.01);

	vtkMyCallback *callback = vtkMyCallback::New();
	callback->SetSelectionActor(sphereActor);
	callback->SetRenderer(renderer);
	
	if(qwin == widget.qvtkWidget1){
		callback->SetWindow(true);
		renderer->AddViewProp(Annotation(20,"Source image",2));
	}else{
		callback->SetWindow(false);
		renderer->AddViewProp(Annotation(20,"Target image",2));
	}
	//Set Camera position
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCamera();

	qwin->SetRenderWindow(renderWindow);
	qwin->GetInteractor()->GetInteractorStyle()->SetCurrentRenderer(renderer);
	this->Axes(qwin->GetInteractor()->GetInteractorStyle()->GetCurrentRenderer(),qwin->GetRenderWindow()->GetInteractor());
	//this->Compass(qwin->GetRenderWindow()->GetInteractor());
	qwin->GetInteractor()->SetInteractorStyle(style);
	/**********************ADD OBSERVERS***********************/
	qwin->GetInteractor()->AddObserver(vtkMyCallback::EndPickEvent, callback);
	qwin->GetInteractor()->SetPicker(picker);
	//qwin->GetInteractor()->AddObserver(vtkCommand::KeyPressEvent,this,&TomoRegistration::KeyCallbackFunction);
	qwin->GetRenderWindow()->Render();
    qwin->update();
}

vtkSmartPointer<vtkPolyData> TomoRegistration::ReadOBJ(QString file)
{
	/*Read .obj file
	file = path.
	*/
	vtkSmartPointer<vtkOBJReader> object = vtkSmartPointer<vtkOBJReader>::New();
	object->SetFileName(file.toStdString().c_str());
	this->widget.statusBar->showMessage("Loading object file...",2000);
	object->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::LoadObjFile);
	object->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	object->Update();
	return object->GetOutput();
}

vtkSmartPointer<vtkImageData> TomoRegistration::ReadDicomDirectory(QString file,bool smooth)
{
	/*
	file = path.
	smooth = true to apply the gaussian smooth filter on the vtkImageData
	*/
	vtkSmartPointer<vtkDICOMImageReader> Read = vtkSmartPointer<vtkDICOMImageReader>::New();
	Read->SetDirectoryName(file.toStdString().c_str());
	Read->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::LoadDicomDirectory);
	Read->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	Read->Update();
	if(smooth){
		vtkSmartPointer<vtkImageGaussianSmooth> gaussianSmoothFilter = vtkSmartPointer<vtkImageGaussianSmooth>::New();
		gaussianSmoothFilter->SetInputConnection(Read->GetOutputPort());
		gaussianSmoothFilter->Update();

		return gaussianSmoothFilter->GetOutput();
	}else{
		return Read->GetOutput();
	}
}

void TomoRegistration::ReadDicomSeries(vtkImageData* dicom,QVTKWidget* qwin)
{
	/*this function is used to read a series of dicom file and use image viewer to display the result.
	This function is called in the case to perform a surface profile-PET registration
	dicom = iamge data.
	qwin = qvtkWidget window.
	*/
	double *range = dicom->GetScalarRange();
    dicom->SetNumberOfScalarComponents(1); 
	dicom->Update();     
    /*******************************Apply Colors*******************************/
    vtkLookupTable *bwLut = vtkLookupTable::New();
    bwLut->SetTableRange(range);
    bwLut->SetNumberOfColors(64);
    bwLut->SetSaturationRange(0,1);
    bwLut->SetHueRange(0.66667,0);
    bwLut->SetValueRange(0,0.66667);   
    bwLut->Build();
    vtkImageMapToColors* Reader = vtkImageMapToColors::New();
    Reader->SetInput(dicom);
    Reader->SetOutputFormatToRGBA();
    Reader->SetLookupTable(bwLut);
    Reader->GetOutput()->SetScalarTypeToFloat(); // option
    Reader->Update();
    /******************************Visualization*******************************/
    vtkImageViewer2* imageViewer = vtkImageViewer2::New();
    imageViewer->SetInputConnection(Reader->GetOutputPort());  
    vtkTextProperty* sliceTextProp = vtkTextProperty::New();
    sliceTextProp->SetFontFamilyToCourier();
    sliceTextProp->SetFontSize(20);
    sliceTextProp->SetVerticalJustificationToBottom();
    sliceTextProp->SetJustificationToLeft();
    vtkTextMapper* sliceTextMapper = vtkTextMapper::New();
    std::string msg = StatusMessage::Format(imageViewer->GetSliceMin(),imageViewer->GetSliceMax());
    sliceTextMapper->SetInput(msg.c_str());
    sliceTextMapper->SetTextProperty(sliceTextProp);
    vtkActor2D* sliceTextActor = vtkActor2D::New();
    sliceTextActor->SetMapper(sliceTextMapper);
    sliceTextActor->SetPosition(15,10);
    vtkTextProperty* usageTextProp = vtkTextProperty::New();
    usageTextProp->SetFontFamilyToCourier();
    usageTextProp->SetFontSize(14);
    usageTextProp->SetVerticalJustificationToTop();
    usageTextProp->SetJustificationToLeft();
    vtkTextMapper* usageTextMapper = vtkTextMapper::New();
    usageTextMapper->SetInput("\n-Slice with Up/Down-Key\n"
    "-Zoom with pressed right\n mouse button while dragging");
    usageTextMapper->SetTextProperty(usageTextProp);
    vtkActor2D* usageTextActor = vtkActor2D::New();
    usageTextActor->SetMapper(usageTextMapper);
    usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
    usageTextActor->GetPositionCoordinate()->SetValue(0.05,0.95);
    myVtkInteractorStyleImage* myInteractorStyle = myVtkInteractorStyleImage::New();
    myInteractorStyle->SetImageViewer(imageViewer);
    myInteractorStyle->SetStatusMapper(sliceTextMapper);
    imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
    imageViewer->GetRenderer()->AddActor2D(usageTextActor);
    imageViewer->GetRenderer()->AddViewProp(Annotation(20,"Dicom series",2));
    
	qwin->SetRenderWindow(imageViewer->GetRenderWindow());
	qwin->GetRenderWindow()->GetInteractor()->SetInteractorStyle(myInteractorStyle);
    imageViewer->Render();
	qwin->update();    
}

void TomoRegistration::ReadRawImages(QString file,std::vector<int> dataExtent,std::vector<double> voxelSize,bool type,QVTKWidget* qwin)
{
	/*use vtkImageData to read any rawfile according the data extent and voxels dimension.
	file = path.
	dataExtent = 3D image dimension (voxel).
	voxelSize = voxle dimension (mm).
	type = data type (short or double).
	qwin = the qvtkWidget window.
	*/
	this->RawReader->SetFileName(file.toStdString().c_str());
	this->RawReader->SetFileDimensionality(3);
	this->RawReader->SetDataExtent(0,dataExtent[0]-1,0,dataExtent[1]-1,0,dataExtent[2]-1);
	this->RawReader->SetDataSpacing(voxelSize[0],voxelSize[1],voxelSize[2]);
	this->RawReader->SetDataByteOrderToLittleEndian();
	if(type){
		this->RawReader->SetDataScalarTypeToShort();
	}else{
		this->RawReader->SetDataScalarTypeToDouble();
	}
	this->RawReader->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::LoadRawFile);
	this->RawReader->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	this->RawReader->Update();

	/*reslice the data image if it's superior to 512x512x512 to reduce the number of slices it seems impossible
	to move smoothly to interact with the object.*/
	vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
	if(dataExtent[0] == 512 && dataExtent[1] == 512 && dataExtent[2] == 512){
		reslice->SetInputConnection(this->RawReader->GetOutputPort());
		reslice->SetOutputExtent(0,255, 0,255, 0,255);
		double p[3];
		p[0] = (dataExtent[1] * voxelSize[0]) / 256;
		p[1] = (dataExtent[1] * voxelSize[1]) / 256;
		p[2] = (dataExtent[2] * voxelSize[2]) / 256;
		reslice->SetOutputSpacing(p[0],p[1],p[2]);
		reslice->SetOutputDimensionality(3);
		reslice->SetInterpolationModeToCubic();
		reslice->AutoCropOutputOn();
		reslice->SetResliceAxesOrigin(0,0,0);
		reslice->Update();
	}

	if(qwin == this->widget.qvtkWidget2){
		if(dataExtent[0] == 512 && dataExtent[1] == 512 && dataExtent[2] == 512){
			this->DICOM = reslice->GetOutput();
		}else{
			this->DICOM = this->RawReader->GetOutput();
		}
	}else{
		if(dataExtent[0] == 512 && dataExtent[1] == 512 && dataExtent[2] == 512){
			this->DICOMS = reslice->GetOutput();
		}else{
			this->DICOMS = this->RawReader->GetOutput();
		}
	}

	/************Visualize the volume***********/
	if(qwin == widget.qvtkWidget1){
		this->DisplayVolume(this->DICOMS,"3D raw file",qwin,1);
	}else{
		this->DisplayVolume(this->DICOM,"3D raw file",qwin,1);
	}
}

void TomoRegistration::DisplayDicomSeries()
{
	if(this->DICOM){//target dicom series
		this->ReadDicomSeries(this->DICOM,this->widget.qvtkWidget2);
	}
	if(this->DICOMS){//source dicom series
		this->ReadDicomSeries(this->DICOMS,this->widget.qvtkWidget1);
	}
}

/********************REGISTRATION**********************/
void TomoRegistration::ApplyRegistration()
{
	/*Apply the registration according to the current combox index.
	Show warning if the user did a mistake.*/
	if((this->comboBox->currentIndex()==0)||(this->comboBox->currentIndex()==1)){
		if(!this->DATA){
			QMessageBox::warning(this,"Warning","Could not find source image!");	
		}else if(!this->DATA2){
			QMessageBox::warning(this,"Warning","Could not find target image!");
		}else{
			QSettings settings("_settings.ini", QSettings::IniFormat);
			settings.beginGroup("Profile/Profile-Profile/PET");
			if(settings.value("radioButtonICP").toBool()){
				this->ICP(this->DATA,this->DATA2);
			}else{
				if((sourceLandmark.size()!=targetLandmark.size())||sourceLandmark.size()<3){
					QMessageBox::warning(this,"Warning","You need at least 3 landmarks for source and target data.");
				}else{
					this->LandmarkBased();
				}
			}
			settings.endGroup();
		}
	}else if(this->comboBox->currentIndex()==2){
		if(!this->DICOMS){
			QMessageBox::warning(this,"Warning","Could not find source volume!");	
		}else if(!this->DICOM){
			QMessageBox::warning(this,"Warning","Could not find target volume!");
		}else if(sourceCentroid.size() != 3 || targetCentroid.size() != 3){
			QMessageBox::warning(this,"Warning","You need at least 3 BoxWdigets!");
		}else{
			this->FiducialLandmarkRegistration();
		}
	}
}
        /************Surface************/
void TomoRegistration::LandmarkBased()
{
	/*the following algorithm is used to match each surfaces by applying
	the transformation that modify one surface to best match the second surface according to the position of landmarks (at least 3).
	this may use the ICP algorithm to finetune the landmark transformation result.*/
	double sPoint1[3]={sourceLandmark[0][0],sourceLandmark[0][1],sourceLandmark[0][2]};
	double sPoint2[3]={sourceLandmark[1][0],sourceLandmark[1][1],sourceLandmark[1][2]};
	double sPoint3[3]={sourceLandmark[2][0],sourceLandmark[2][1],sourceLandmark[2][2]};
	Frame2 frame1(sPoint1,sPoint2,sPoint3);

	double tPoint1[3]={targetLandmark[0][0],targetLandmark[0][1],targetLandmark[0][2]};
	double tPoint2[3]={targetLandmark[1][0],targetLandmark[1][1],targetLandmark[1][2]};
	double tPoint3[3]={targetLandmark[2][0],targetLandmark[2][1],targetLandmark[2][2]};
	Frame2 frame2(tPoint1,tPoint2,tPoint3);
	
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	//AlignFrames(frame1,frame2,transform);
	AlignLandmarkFrame(frame1,frame2,transform);
    /**************************Apply the Transformation************************/
    vtkSmartPointer<vtkTransformPolyDataFilter> transData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transData->SetInput(this->DATA);
    transData->SetTransform(transform);
    transData->Update();
	/***********************************USE ICP********************************/
	QSettings settings("_settings.ini", QSettings::IniFormat);
	settings.beginGroup("Profile/Profile-Profile/PET");
	vtkSmartPointer<vtkIterativeClosestPointTransform> icp = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	icp->SetSource(transData->GetOutput());
	icp->SetTarget(this->DATA2);
	
	if(settings.value("radioButtonRigid").toBool()){
		icp->GetLandmarkTransform()->SetModeToRigidBody();
	}else if(settings.value("radioButtonSimilarity").toBool()){
		icp->GetLandmarkTransform()->SetModeToSimilarity();
	}else{
		icp->GetLandmarkTransform()->SetModeToAffine();
	}
	
	icp->SetMaximumNumberOfIterations(settings.value("spinBoxNumberOfIteration").toInt());
	if(settings.value("radioButtonLandmarkICP").toBool()){
		icp->StartByMatchingCentroidsOn();
	}

    icp->CheckMeanDistanceOn();
    icp->SetMeanDistanceModeToRMS();
    icp->Modified();
    icp->Update();
	/***************************transform with ICP*****************************/
	vtkSmartPointer<vtkTransformPolyDataFilter> transICP = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transICP->SetInput(transData->GetOutput());
	transICP->SetTransform(icp);
	transICP->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::RegistrationProgressFunction);
	transICP->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	transICP->Update();
    /************************4x4 Matrix of Transformation**********************/
	vtkSmartPointer<vtkMatrix4x4> m1 = transform->GetMatrix();
    vtkSmartPointer<vtkMatrix4x4> m2 = icp->GetMatrix();
	vtkSmartPointer<vtkMatrix4x4> m = vtkSmartPointer<vtkMatrix4x4>::New();
	m->Multiply4x4(m1,m2,m);
    /******************************Visualization*******************************/
	vtkSmartPointer<vtkPolyDataMapper> sourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sourceMapper->SetInput(this->DATA);
    vtkSmartPointer<vtkActor> sourceActor = vtkSmartPointer<vtkActor>::New();
    sourceActor->SetMapper(sourceMapper);

    vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	targetMapper->SetInput(this->DATA2);
    vtkSmartPointer<vtkActor> targetActor = vtkSmartPointer<vtkActor>::New();
    targetActor->SetMapper(targetMapper);
    targetActor->GetProperty()->SetColor(1.0,0.27,0.0);

    vtkSmartPointer<vtkPolyDataMapper> solutionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	if(settings.value("radioButtonLandmarkICP").toBool()){
		solutionMapper->SetInputConnection(transICP->GetOutputPort());
	}else{
		solutionMapper->SetInput(transData->GetOutput());
	}
    vtkSmartPointer<vtkActor> solutionActor = vtkSmartPointer<vtkActor>::New();
    solutionActor->SetMapper(solutionMapper);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
    renderer->AddActor(solutionActor);
	renderer->AddActor(targetActor);

	if(this->DICOM && this->comboBox->currentIndex()==1){
		renderer->AddVolume(this->GetVolume(this->DICOM,0));
	}
	if(settings.value("radioButtonLandmarkICP").toBool()){
		renderer->AddViewProp(Annotation(20,"Landmark + ICP result",2));
		renderer->AddActor2D(Matrix(m));
	}else{
		renderer->AddViewProp(Annotation(20,"Landmark result",2));
		renderer->AddActor2D(Matrix(m1));
	}
    renderer->SetBackground(0.9,0.9,0.9);
    renderer->GradientBackgroundOn();
	settings.endGroup();

	widget.qvtkWidget3->SetRenderWindow(renderWindow);
	widget.qvtkWidget3->GetInteractor()->SetInteractorStyle(style);
	this->Axes(renderer,widget.qvtkWidget3->GetRenderWindow()->GetInteractor());
	renderer->ResetCamera();
	widget.qvtkWidget3->update();
}

void TomoRegistration::ICP(vtkPolyData* source,vtkPolyData* target)
{
	/*This function is used for surface registration
	the following algorithm is used to minimize the distance between
	two surfaces. The core of the algorithm is to match each vertex in one surface with the closest 
	surface point on the other, then apply the transformation that modify one surface to best match the other in a least square sense.
	This has to be iterated by settings the maximum number of iteration to get proper convergence of the surfaces.*/
	QSettings settings("_settings.ini", QSettings::IniFormat);
	settings.beginGroup("Profile/Profile-Profile/PET");
    vtkSmartPointer<vtkIterativeClosestPointTransform> icp = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	icp->SetSource(source);
	icp->SetTarget(target);
	icp->SetMaximumNumberOfIterations(settings.value("spinBoxNumberOfIteration").toInt());
	if(settings.value("checkBoxMatchingCentroid").toBool()){
		icp->StartByMatchingCentroidsOn();
	}

	if(settings.value("radioButtonRigid").toBool()){
		icp->GetLandmarkTransform()->SetModeToRigidBody();
	}else if(settings.value("radioButtonSimilarity").toBool()){
		icp->GetLandmarkTransform()->SetModeToSimilarity();
	}else{
		icp->GetLandmarkTransform()->SetModeToAffine();
	}   
    icp->CheckMeanDistanceOn();
    icp->SetMeanDistanceModeToRMS();
    icp->Modified();
    icp->Update();
	settings.endGroup();
	/***************************transform with ICP*****************************/
	vtkSmartPointer<vtkTransformPolyDataFilter> transICP = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transICP->SetInput(source);
	transICP->SetTransform(icp);
	transICP->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::RegistrationProgressFunction);
	transICP->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	transICP->Update();
    /************************4x4 Matrix of Transformation**********************/
    vtkSmartPointer<vtkMatrix4x4> matrix = icp->GetMatrix();
    /******************************Visualization*******************************/
	vtkSmartPointer<vtkPolyDataMapper> sourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sourceMapper->SetInput(source);
    vtkSmartPointer<vtkActor> sourceActor = vtkSmartPointer<vtkActor>::New();
    sourceActor->SetMapper(sourceMapper);

    vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	targetMapper->SetInput(target);
    vtkSmartPointer<vtkActor> targetActor = vtkSmartPointer<vtkActor>::New();
    targetActor->SetMapper(targetMapper);
    targetActor->GetProperty()->SetColor(1.0,0.27,0.0);

    vtkSmartPointer<vtkPolyDataMapper> solutionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    solutionMapper->SetInputConnection(transICP->GetOutputPort());
    vtkSmartPointer<vtkActor> solutionActor = vtkSmartPointer<vtkActor>::New();
    solutionActor->SetMapper(solutionMapper);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
    renderer->AddActor(solutionActor);
	renderer->AddActor(targetActor);
	//renderer->AddActor(sourceActor);
	if(this->DICOM && this->comboBox->currentIndex() == 1){
		renderer->AddVolume(this->GetVolume(this->DICOM,0));
	}
    renderer->AddViewProp(Annotation(20,"ICP result",2));
    renderer->SetBackground(0.9,0.9,0.9);
    renderer->GradientBackgroundOn();
	renderer->AddActor2D(Matrix(matrix));

	widget.qvtkWidget3->SetRenderWindow(renderWindow);
	widget.qvtkWidget3->GetInteractor()->SetInteractorStyle(style);
	this->Axes(renderer,widget.qvtkWidget3->GetRenderWindow()->GetInteractor());
	renderer->ResetCamera();
	widget.qvtkWidget3->update();
}
        /************Fiducial***********/
void TomoRegistration::PaulViola(vtkImageData* source,vtkImageData* target)
{
	
	//ITKRegistration a;

	//a.VTKimage2ITKimage(this->DICOM);
	/*
	cout<<"fixed path: "<<fix<<endl;
	cout<<"moving path: "<<mov<<endl;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
	renderer->AddVolume(this->GetVolume(a.VolumeRegistration(fix,mov)));
	renderer->AddViewProp(Annotation(20,"3D Model",2));
	renderer->SetBackground(0.9,0.9,0.9);
	renderer->GradientBackgroundOn();
	this->widget.qvtkWidget3->SetRenderWindow(renderWindow);
	this->widget.qvtkWidget3->GetInteractor()->SetInteractorStyle(style);
	this->widget.qvtkWidget3->update();*/
}

void TomoRegistration::FiducialLandmarkRegistration()
{
	/*the following algorithm is used to match two volumes by applying a rigid transformation 
	that modify the source volume to best match the target volume according to the position of the fiducial markers.
	this may use the ICP algorithm or Mutual Information algorithm to finetune the result.*/

	/**Resampling at the same resolution***/
	this->ChangeResolution(this->DICOMS, this->DICOM);
	/*read the source centroid*/
	QSettings settings("_settings.ini", QSettings::IniFormat);
	settings.beginGroup("Fiducial_Landmark");
	double sPoint1[3]={sourceCentroid[0][0],sourceCentroid[0][1],sourceCentroid[0][2]};
	double sPoint2[3]={sourceCentroid[1][0],sourceCentroid[1][1],sourceCentroid[1][2]};
	double sPoint3[3]={sourceCentroid[2][0],sourceCentroid[2][1],sourceCentroid[2][2]};
	Frame2 frame1(sPoint1,sPoint2,sPoint3);
	/*read the target centroid*/
	double tPoint1[3]={targetCentroid[0][0],targetCentroid[0][1],targetCentroid[0][2]};
	double tPoint2[3]={targetCentroid[1][0],targetCentroid[1][1],targetCentroid[1][2]};
	double tPoint3[3]={targetCentroid[2][0],targetCentroid[2][1],targetCentroid[2][2]};
	Frame2 frame2(tPoint1,tPoint2,tPoint3);
	/*Apply the rigid landmark transformation*/
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	AlignLandmarkFrame(frame1,frame2,transform);
	/*set vol to use set the usertransform*/
	vtkSmartPointer<vtkVolume> vol = vtkSmartPointer<vtkVolume>::New();
	if(settings.value("sourceRawFile").toBool()){
		if(settings.value("FMS_MRI").toBool()){
			vol = this->GetVolume(this->DICOMS,2);
		}else{
			vol = this->GetVolume(this->DICOMS,1);
		}
	}else{
		vol = this->GetVolume(this->DICOMS,0);
	}
	/****************** find the matrix of transformation *********************/
	vtkSmartPointer<vtkMatrix4x4> mTransform = transform->GetMatrix();
	vtkSmartPointer<vtkMatrix4x4> matrixOfTransformation = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkTransform> Transformation = vtkSmartPointer<vtkTransform>::New();

	if(settings.value("FiducialMarkers_landmark").toBool()){
		Transformation->SetMatrix(mTransform);
	}else if(settings.value("FiducialMarkers_ICP").toBool()){
		vtkSmartPointer<vtkIterativeClosestPointTransform> icp = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
		icp->SetSource(this->ImageData2Polydata(this->DICOMS,transform,true));
		icp->SetTarget(this->ImageData2Polydata(this->DICOM,transform,false));
		icp->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::RegistrationProgressFunction);
		icp->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
		icp->SetMaximumNumberOfIterations(20);
		icp->StartByMatchingCentroidsOn();
		icp->GetLandmarkTransform()->SetModeToRigidBody();//rigide transformation
		icp->SetMeanDistanceModeToRMS();
		icp->Update();
		vtkSmartPointer<vtkMatrix4x4> icp_matrix = icp->GetMatrix();
		matrixOfTransformation->Multiply4x4(icp_matrix,mTransform,matrixOfTransformation);
		Transformation->SetMatrix(matrixOfTransformation);
	}
	//set the appropriate transformation
	vol->SetUserTransform(Transformation);
	vol->Update();
	/************************Interaction & visualisation***********************/
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
	if(settings.value("targetRawFile").toBool()){
		if(settings.value("FMT_MRI").toBool()){
			renderer->AddVolume(this->GetVolume(this->DICOM,2));
		}else{
			renderer->AddVolume(this->GetVolume(this->DICOM,1));
		}
	}else{
		renderer->AddVolume(this->GetVolume(this->DICOM,0));
	}
	settings.endGroup();

	renderer->AddVolume(vol);
	renderer->AddViewProp(Annotation(20,"Fiducial markers result",2));
	renderer->SetBackground(0.9,0.9,0.9);
	renderer->GradientBackgroundOn();
	renderer->AddActor2D(Matrix(Transformation->GetMatrix()));

	this->widget.qvtkWidget3->SetRenderWindow(renderWindow);
	this->widget.qvtkWidget3->GetInteractor()->GetInteractorStyle()->SetCurrentRenderer(renderer);
	this->Axes(this->widget.qvtkWidget3->GetInteractor()->GetInteractorStyle()->GetCurrentRenderer(),this->widget.qvtkWidget3->GetRenderWindow()->GetInteractor());
	this->widget.qvtkWidget3->GetInteractor()->SetInteractorStyle(style);
	this->widget.qvtkWidget3->GetRenderWindow()->Render();
	renderer->ResetCamera();
	this->widget.qvtkWidget3->update();
}

/*********************OPEN TOOLS***********************/
void TomoRegistration::OpenSettings()
{
	settings *dialog = new settings;
    dialog->raise();
    dialog->exec();
}

void TomoRegistration::OpenDataProcess()
{
	DataProcess *dialog = new DataProcess;
	dialog->take(this);
	dialog->raise();
	dialog->exec();
}
// get data from the DataProcess dialog
void TomoRegistration::GetCleanData(vtkPolyData* data,vtkImageData* image,int a,int type)
{   
	/*this function is used to import data from DataProcess GUI.
	image = an obj file.
	image = volume or a raw file.
	a =  is used to identify if the data is considered as a source or as a target
	type = is set to identify which kind of data is imported (data or image)
	*/
	if(type==0){
		if(a==0){
			this->DATA = data;
			this->ReadSurface(this->DATA,this->widget.qvtkWidget1);
		}else{
			this->DATA2 = data;
			this->ReadSurface(this->DATA2,this->widget.qvtkWidget2);
		}
		this->widget.statusBar->showMessage("Importing data...",2000);
	}else if(type==1){
		if(a==0){
			this->DICOMS = image;
			this->DisplayVolume(this->DICOMS,"Imported source data",this->widget.qvtkWidget1,0);
		}else{
			this->DICOM = image;
			this->DisplayVolume(this->DICOM,"Imported target data",this->widget.qvtkWidget2,0);
		}

	}
}

/**********************READ VOLUME**********************/
vtkSmartPointer<vtkVolume> TomoRegistration::GetVolume(vtkImageData* DCM, int tomo)
{
	/*DCM is the image data. tomo is used to set the volume property according the type of tomography
	0 = PET.
	1 = CT.
	2 = MRI.
	/*Extract the volume according to the scalar value of your dicom series*/
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapper->SetInputConnection(DCM->GetProducerPort());
	volumeMapper->AutoAdjustSampleDistancesOff();
	volumeMapper->SetBlendModeToComposite();

	/********************set color & opacity*******************/
    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();

	switch(tomo)
	{
	case 0:
		{
			/*This case is used to set color and opacity for PET image*/
			compositeOpacity->AddPoint(DCM->GetScalarRange()[0],0.0);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*3/255,0.0);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*15/255,0.1);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*70/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*75/255,0.3);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*78/255,1);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*82/255,1);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*90/255,0.3);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*135/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*152/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1],0.2);

			color->AddRGBPoint(DCM->GetScalarRange()[1]*0/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*3/255,1.0,0.3,0.3);//
			color->AddRGBPoint(DCM->GetScalarRange()[1]*20/255,0.0,0.0,1.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*50/255,0.9,0.37,0.27);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*80.0/255,0.15,0.15,0.5);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*120.0/255,1.0,1.0,1.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*160.0/255,1.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*200.0/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1],1.0,1.0,1.0);

			volumeProperty->SetAmbient(0.5);
			volumeProperty->SetDiffuse(0.6);
			volumeProperty->SetSpecular(5.0);
		}
		break;
	case 1:
		{
			/*This case is used to set opacity for CT image*/
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*15/255,0.0);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*70/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*75/255,0.3);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*78/255,0.4);//1
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*82/255,0.4);//1
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*90/255,0.3);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*135/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*152/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1],0.2);

			color->AddRGBPoint(DCM->GetScalarRange()[1]*0/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*20/255,0.1,0.1,0.1);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*50/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*80.0/255,0.1,0.1,0.1);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*120.0/255,1.0,1.0,1.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*160.0/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*200.0/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1],1.0,1.0,1.0);

			volumeProperty->SetAmbient(0.3);
			volumeProperty->SetDiffuse(0.4);
			volumeProperty->SetSpecular(2.0);
		}
		break;
	case 2:
		{
			/*This case is used to set opacity for MRI image*/
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*15/255,0.0);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*70/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*75/255,0.3);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*78/255,0.4);//1
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*82/255,0.4);//1
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*90/255,0.3);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*135/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1]*152/255,0.2);
			compositeOpacity->AddPoint(DCM->GetScalarRange()[1],0.2);

			color->AddRGBPoint(DCM->GetScalarRange()[1]*0/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*20/255,0.1,0.1,0.1);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*50/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*80.0/255,0.1,0.1,0.1);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*120.0/255,1.0,1.0,1.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*160.0/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1]*200.0/255,0.0,0.0,0.0);
			color->AddRGBPoint(DCM->GetScalarRange()[1],1.0,1.0,1.0);
		}
		break;
	}

	/*set volume property*/
	volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->SetColor(color);
	volumeProperty->SetScalarOpacity(compositeOpacity);
    volumeProperty->ShadeOn();

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
    
	return volume;
}

void TomoRegistration::DisplayVolume(vtkImageData* img,const char* annot,QVTKWidget* qwin,int tomo)
{
	/*
	img = image input.
	annot = annotation.
	qwin = qvtkWidget.
	tomo = PET, CT or MRI.
	*/
	/*Create the outline*/
	vtkSmartPointer<vtkOutlineFilter> outlineFilter = vtkSmartPointer<vtkOutlineFilter>::New();
	outlineFilter->SetInput(img);

	vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	outlineMapper->SetInputConnection(outlineFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outlineMapper);

	/**set the interactor**/
	vtkSmartPointer<MyInteractorStyle> style = vtkSmartPointer<MyInteractorStyle>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
	
	if(tomo == 2){
		renderer->AddVolume(GetVolume(img,2));
	}else if(tomo == 1){
		renderer->AddVolume(GetVolume(img,1));
	}else{
		renderer->AddVolume(GetVolume(img,0));
	}
	renderer->AddActor(outlineActor);
	renderer->AddViewProp(Annotation(20,annot,2));
	renderer->SetBackground(0.9,0.9,0.9);
	renderer->GradientBackgroundOn();
	renderer->AddActor2D(this->ImageInfo(img));
	style->SetRenderer(renderer);
	style->SetImageData(img);
	if(qwin == this->widget.qvtkWidget2){
		style->CurrentWindow(true); // used for target image
	}else{
		style->CurrentWindow(false);// used for source image
	}
	/*Set camera position*/
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCamera();

	qwin->SetRenderWindow(renderWindow);
	qwin->GetInteractor()->GetInteractorStyle()->SetCurrentRenderer(renderer);
	this->Axes(qwin->GetInteractor()->GetInteractorStyle()->GetCurrentRenderer(),qwin->GetRenderWindow()->GetInteractor());
	qwin->GetInteractor()->SetInteractorStyle(style);
	qwin->GetRenderWindow()->Render();
	renderer->ResetCamera();
	qwin->update();
}

void TomoRegistration::Get3D()
{
	if(this->DICOM){
		this->DisplayVolume(this->DICOM,"3D model from dicom series",this->widget.qvtkWidget2,0);
	}
	if(this->DICOMS){
		this->DisplayVolume(this->DICOMS,"3D model from dicom series",this->widget.qvtkWidget1,0);
	}
}

/**********************SURFACE EXTRACTION**********************/
vtkSmartPointer<vtkPolyDataConnectivityFilter> TomoRegistration::ExtractSurface(vtkImageData* DCM, double value)
{
	/*Extract surface using marching cubes algorithm*/
	vtkSmartPointer<vtkMarchingCubes> mc = vtkSmartPointer<vtkMarchingCubes>::New();
	mc->SetInputConnection(DCM->GetProducerPort());
	mc->ComputeNormalsOn();
	mc->SetValue(0,value); // set the isovalue
	mc->Update();
	/*********Mesh the largest contour using Marching Cubes algorithm**********/
	vtkSmartPointer<vtkPolyDataConnectivityFilter> mcFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	mcFilter->SetInput(mc->GetOutput());
	mcFilter->SetExtractionModeToLargestRegion();
	mcFilter->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::KeyCallbackFunction);
	mcFilter->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	mcFilter->Update();

	return mcFilter;// return a polydata
}

void TomoRegistration::GetSurface()
{
	/*this function is used to extract the surface of a volume*/
	if(this->comboBox->currentIndex() == 1)
	{
		if(!this->DICOM){
			QMessageBox mbPP(QMessageBox::Information,"Get Dicom files","Please load dicom directory!", QMessageBox::Ok);
			mbPP.exec();
		}else{
			MarchingCubesValue* dialog = new MarchingCubesValue;
			dialog->raise();
			if(dialog->exec() == QDialog::Accepted){
				this->DATA2 = ExtractSurface(this->DICOM,dialog->mcValue())->GetOutput();
				this->ReadSurface(ExtractSurface(this->DICOM,dialog->mcValue())->GetOutput(),widget.qvtkWidget2);
			}
		}
	}
}

vtkSmartPointer<vtkPolyData> TomoRegistration::ImageData2Polydata(vtkImageData* imageData,vtkTransform* transform,bool a)
{
	/*this function is used to transform scalar volume to a polydata volume*/
	vtkSmartPointer<vtkImageDataGeometryFilter> imageDataGeometryFilter = vtkSmartPointer<vtkImageDataGeometryFilter>::New();
	imageDataGeometryFilter->SetInputConnection(imageData->GetProducerPort());
	imageDataGeometryFilter->Update();

	/**************Apply transform if necessary***********/
    vtkSmartPointer<vtkTransformPolyDataFilter> transData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transData->SetInput(imageDataGeometryFilter->GetOutput());
	transData->SetTransform(transform);
    transData->Update();
	if(a){
		return transData->GetOutput();
	}else{
		return imageDataGeometryFilter->GetOutput();
	}

}

/********************* ALIGN FRAMES  ******************/
void AlignFrames(Frame sourceFrame,Frame targetFrame,vtkTransform* transform)
{
	//==================SOURCE MATRIX====================//
	vtkSmartPointer<vtkMatrix3x3> matOA = vtkSmartPointer<vtkMatrix3x3>::New();
	for(unsigned int i=0;i<3;i++){
		matOA->SetElement(0,i,sourceFrame.XDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOA->SetElement(1,i,sourceFrame.YDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOA->SetElement(2,i,sourceFrame.ZDirection[i]);
	}
	matOA->Invert();
	//===================TARGET MATRIX====================//
	vtkSmartPointer<vtkMatrix3x3> matOB = vtkSmartPointer<vtkMatrix3x3>::New();
	for(unsigned int i=0;i<3;i++){
		matOB->SetElement(0,i,targetFrame.XDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOB->SetElement(1,i,targetFrame.YDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOB->SetElement(2,i,targetFrame.ZDirection[i]);
	}
	//matOB->Invert();
	//=====================TRANSFORM=======================//
	vtkSmartPointer<vtkMatrix3x3> MatrixOfRotation = vtkSmartPointer<vtkMatrix3x3>::New();
	MatrixOfRotation->Multiply3x3(matOA,matOB,MatrixOfRotation);

	vtkSmartPointer<vtkMatrix4x4> ResultMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
	for(unsigned int i=0;i<3;i++){
		for(unsigned int j=0;j<3;j++){
			ResultMatrix->SetElement(i,j,MatrixOfRotation->GetElement(i,j));
		}
	}
	for(unsigned int j=0;j<3;j++){
		ResultMatrix->SetElement(3,j,0);
	}

	double a[3][3];
	for(unsigned int i=0;i<3;i++){
		for(unsigned int j=0;j<3;j++){
			a[i][j] = MatrixOfRotation->GetElement(i,j);
		}
	}

	double Trans[3];
	vtkMath::Multiply3x3(a,sourceFrame.centroid,Trans);
	vtkMath::Subtract(targetFrame.centroid,Trans,Trans);

	for(unsigned int i=0;i<3;i++){
		ResultMatrix->SetElement(i,3,Trans[i]);
	}
	ResultMatrix->SetElement(3,3,1);

	transform->SetMatrix(ResultMatrix);
}

void AlignLandmarkFrame(Frame2 sourceFrame,Frame2 targetFrame,vtkTransform* transform)
{
	/*This function takes two frames and finds the matrix M of the rigid transformation.*/
	vtkSmartPointer<vtkLandmarkTransform> landmarkTransform = vtkSmartPointer<vtkLandmarkTransform>::New();
	
	vtkSmartPointer<vtkPoints> sourcePoints = vtkSmartPointer<vtkPoints>::New();
	sourcePoints->InsertNextPoint(sourceFrame.origin);
	double sX[3];
	vtkMath::Add(sourceFrame.origin,sourceFrame.XDirection,sX);
	sourcePoints->InsertNextPoint(sX);
	double sY[3];
	vtkMath::Add(sourceFrame.origin,sourceFrame.YDirection,sY);
	sourcePoints->InsertNextPoint(sY);
	double sZ[3];
	vtkMath::Add(sourceFrame.origin,sourceFrame.ZDirection,sZ);
	sourcePoints->InsertNextPoint(sZ);
 
	vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New();
	targetPoints->InsertNextPoint(targetFrame.origin);
	double tX[3];
	vtkMath::Add(targetFrame.origin,targetFrame.XDirection,tX);
	targetPoints->InsertNextPoint(tX);
	double tY[3];
	vtkMath::Add(targetFrame.origin,targetFrame.YDirection,tY);
	targetPoints->InsertNextPoint(tY);
	double tZ[3];
	vtkMath::Add(targetFrame.origin,targetFrame.ZDirection,tZ);
	targetPoints->InsertNextPoint(tZ);
 
	landmarkTransform->SetSourceLandmarks(sourcePoints);
	landmarkTransform->SetTargetLandmarks(targetPoints);
	landmarkTransform->SetModeToRigidBody();
	landmarkTransform->Update();
 
	vtkMatrix4x4* M = landmarkTransform->GetMatrix();
	transform->SetMatrix(M);
}

/******************* CALLBACK FUNCTIONS ***************/
/*these functions are used to monitor filters*/
void TomoRegistration::KeyCallbackFunction(vtkObject* caller,long unsigned int eventId,void* callData)
{
	vtkPolyDataConnectivityFilter* marchingCubes = static_cast<vtkPolyDataConnectivityFilter*>(caller);
	this->setCursor(Qt::WaitCursor);
	this->progressBar->setValue(marchingCubes->GetProgress()*100);
}

void TomoRegistration::KeyCallbackFunctionEnd(vtkObject* caller,long unsigned int eventId,void* callData)
{
	this->progressBar->setValue(0);
	this->setCursor(Qt::ArrowCursor);
}

void TomoRegistration::RegistrationProgressFunction(vtkObject* caller,long unsigned int eventId,void* callData)
{
	vtkTransformPolyDataFilter* Progress = static_cast<vtkTransformPolyDataFilter*>(caller);
	this->setCursor(Qt::WaitCursor);
	this->progressBar->setValue(Progress->GetProgress()*100);
}

void TomoRegistration::LoadRawFile(vtkObject* caller,long unsigned int eventId,void* callData)
{
	vtkImageReader* pro = static_cast<vtkImageReader*>(caller);
	this->setCursor(Qt::WaitCursor);
	this->progressBar->setValue(pro->GetProgress()*100);
	this->widget.statusBar->showMessage("Loading raw file...",2000);
}

void TomoRegistration::LoadObjFile(vtkObject* caller,long unsigned int eventId,void* callData)
{
	vtkOBJReader* progress = static_cast<vtkOBJReader*>(caller);
	this->setCursor(Qt::WaitCursor);
	this->progressBar->setValue(progress->GetProgress()*100);
	this->widget.statusBar->showMessage("Loading obj file...",2000);
}

void TomoRegistration::LoadDicomDirectory(vtkObject* caller,long unsigned int eventId,void* callData)
{
	vtkDICOMImageReader* progress = static_cast<vtkDICOMImageReader*>(caller);
	this->setCursor(Qt::WaitCursor);
	this->progressBar->setValue(progress->GetProgress()*100);
	this->widget.statusBar->showMessage("Loading dicom directory...",2000);
}

/********************** SIGNALS ************************/
void TomoRegistration::slot_clicked(vtkObject* caller,unsigned long,void*,void*)
{
	vtkRenderWindowInteractor *iren = static_cast<vtkRenderWindowInteractor*>(caller);
	std::string key = iren->GetKeySym();
	if(key.compare("d") == 0){
	}
}

void TomoRegistration::slot_Box(vtkObject* caller,unsigned long,void*,void*)
{
	vtkRenderWindowInteractor *iren = static_cast<vtkRenderWindowInteractor*>(caller);
	std::string key = iren->GetKeySym();
	if((this->comboBox->currentIndex() == 2) || (key.compare("b") == 0)){
	}
}

/********************** Utilities **********************/
void TomoRegistration::Save()
{
	QFileDialog *dialog = new QFileDialog;
	QString file = dialog->getSaveFileName(this, "Save data","file name",tr("Images (*.obj)"));
	if(file.isNull()){
		connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	}else{
		vtkSmartPointer<vtkOBJExporter> vtkExporter = vtkSmartPointer<vtkOBJExporter>::New();
		vtkExporter->SetFilePrefix(file.toStdString().c_str());
		vtkExporter->SetRenderWindow(widget.qvtkWidget3->GetRenderWindow());
		vtkExporter->Write();
	}
}

void TomoRegistration::Reset()
{
	/*this function should be called every time when you start a new registration*/
	this->DATA = NULL;
	this->DATA2 = NULL;
	this->DICOM = NULL;
	this->DICOMS = NULL;

	sourceLandmark.clear();
	targetLandmark.clear();
	sourceFiducialLandmark.clear();
	targetFiducialLandmark.clear();
	sourceCentroid.clear();
	targetCentroid.clear();
	/*fill the source and the target landmark in the case to erase and insert row data*/
	std::vector<double> V;
	for(unsigned int i=0; i<6; i++){
		V.push_back(0);
	}
	for(unsigned int j=0; j<4; j++){
		targetFiducialLandmark.push_back(V);
		sourceFiducialLandmark.push_back(V);
	}

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);
    renderer->ResetCamera();
	widget.qvtkWidget1->SetRenderWindow(renderWindow);
    widget.qvtkWidget1->update();
	vtkSmartPointer<vtkRenderWindow> renderWindow2 = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderWindow2->AddRenderer(renderer2);
	renderer2->ResetCamera();
	widget.qvtkWidget2->SetRenderWindow(renderWindow2);
    widget.qvtkWidget2->update();
	vtkSmartPointer<vtkRenderWindow> renderWindow3 = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer3 = vtkSmartPointer<vtkRenderer>::New();
	renderWindow3->AddRenderer(renderer3);
	renderer3->ResetCamera();
	widget.qvtkWidget3->SetRenderWindow(renderWindow3);
    widget.qvtkWidget3->update();
}

void TomoRegistration::exit()
{
	/*function to quit the software*/
	QMessageBox mb(QMessageBox::Warning,"Exit programm","Are You Sure You Want to Exit?\n",QMessageBox::Yes | QMessageBox::No);
	mb.setButtonText(QMessageBox::Yes, "yes");
	mb.setButtonText(QMessageBox::No, "no"); 

	switch(mb.exec()){
	case QMessageBox::Yes:
		{/*delete the settings file*/
			QFile file("_settings.ini");
			file.remove();
			this->close();
		}
		break;
	case QMessageBox::No:
		mb.close();
		break;
	}
}

void TomoRegistration::Axes(vtkSmartPointer<vtkRenderer> ren, vtkRenderWindowInteractor *iren)
{
	/*generate 3D axes*/
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    vtkOrientationMarkerWidget* OMW = vtkOrientationMarkerWidget::New();
    OMW->SetCurrentRenderer(ren);
    OMW->SetOutlineColor(0.93,0.57,0.13);
    OMW->SetOrientationMarker(axes);
    OMW->SetInteractor(iren);
    OMW->SetViewport(0.0,0.0,0.4,0.4);
    OMW->SetEnabled(1);
	OMW->InteractiveOff();
}

vtkSmartPointer<vtkCornerAnnotation> TomoRegistration::Annotation(int size, const char* ANN, int Pos)
{
	vtkSmartPointer<vtkCornerAnnotation> annotation = vtkSmartPointer<vtkCornerAnnotation>::New();
	annotation->GetTextProperty()->SetFontFamilyToCourier();
    annotation->SetLinearFontScaleFactor(2);
    annotation->SetNonlinearFontScaleFactor(1);
    annotation->SetMaximumFontSize(size);
    annotation->UseBoundsOn();
    annotation->SetText(Pos, ANN);
    return annotation;
}

vtkSmartPointer<vtkActor2D> TomoRegistration::ImageInfo(vtkImageData* img)
{
	//Get the scalar range of the volume
	std::ostringstream MaxRange, MinRange;
	MinRange << img->GetScalarRange()[0];
	MaxRange << img->GetScalarRange()[1];
	//compute image dimension
	std::ostringstream imgDimensionX, imgDimensionY, imgDimensionZ;
	imgDimensionX << img->GetDimensions()[0];
	imgDimensionY << img->GetDimensions()[1];
	imgDimensionZ << img->GetDimensions()[2];
	//compute voxel dimensions
	std::ostringstream voxelDimensionX, voxelDimensionY, voxelDimensionZ;
	voxelDimensionX << img->GetSpacing()[0];
	voxelDimensionY << img->GetSpacing()[1];
	voxelDimensionZ << img->GetSpacing()[2];

    char buff[150];
    strcpy_s(buff,"-scalar range: ");
	strcat_s(buff,MinRange.str().c_str());
	strcat_s(buff," to ");
	strcat_s(buff,MaxRange.str().c_str());
	strcat_s(buff,"\n-image dimension: ");
	strcat_s(buff,imgDimensionX.str().c_str());
	strcat_s(buff," x ");
	strcat_s(buff,imgDimensionY.str().c_str());
	strcat_s(buff," x ");
	strcat_s(buff,imgDimensionZ.str().c_str());
	strcat_s(buff,"\n-voxel dimension: ");
	strcat_s(buff,voxelDimensionX.str().c_str());
	strcat_s(buff," x ");
	strcat_s(buff,voxelDimensionY.str().c_str());
	strcat_s(buff," x ");
	strcat_s(buff,voxelDimensionZ.str().c_str());

	/*display volume on qwin vtk window*/
	vtkTextProperty* usageTextProp = vtkTextProperty::New();
    usageTextProp->SetFontFamilyToCourier();
    usageTextProp->SetFontSize(14);
    usageTextProp->SetVerticalJustificationToTop();
    usageTextProp->SetJustificationToLeft();
    vtkTextMapper* usageTextMapper = vtkTextMapper::New();
    usageTextMapper->SetInput(buff);
	usageTextMapper->SetTextProperty(usageTextProp);
    
    vtkActor2D* usageTextActor = vtkActor2D::New();
    usageTextActor->SetMapper(usageTextMapper);

    usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
    usageTextActor->GetPositionCoordinate()->SetValue(0.05,0.95);

	return usageTextActor;
}

vtkSmartPointer<vtkActor2D> TomoRegistration::DataInfo(vtkPolyData* polydata)
{
	/*get number of cells and points*/
	int Cells = polydata->GetNumberOfCells();
	int Points = polydata->GetNumberOfPoints();
	std::ostringstream strCells, strPoints;
	strCells << Cells;
	strPoints << Points;
	/*get boundaries*/
	double Sbounds[6];
    polydata->GetBounds(Sbounds);
    double Sx = Sbounds[1]-Sbounds[0];
    double Sy = Sbounds[3]-Sbounds[2];
    double Sz = Sbounds[5]-Sbounds[4];
    std::ostringstream strSBx, strSBy, strSBz;
    strSBx << Sx;
    strSBy << Sy;
    strSBz << Sz;

    char buff[100];
    strcpy_s(buff,"-size: ");
    strcat_s(buff,strSBx.str().c_str());
    strcat_s(buff," x ");
    strcat_s(buff,strSBy.str().c_str());
    strcat_s(buff," x ");
    strcat_s(buff,strSBz.str().c_str());
	strcat_s(buff,"\n-number of cells: ");
	strcat_s(buff,strCells.str().c_str());
	strcat_s(buff,"\n-number of vertices: ");
	strcat_s(buff,strPoints.str().c_str());

	/*display volume on qwin vtk window*/
	vtkTextProperty* usageTextProp = vtkTextProperty::New();
    usageTextProp->SetFontFamilyToCourier();
    usageTextProp->SetFontSize(14);
    usageTextProp->SetVerticalJustificationToTop();
    usageTextProp->SetJustificationToLeft();
    vtkTextMapper* usageTextMapper = vtkTextMapper::New();
    usageTextMapper->SetInput(buff);
	usageTextMapper->SetTextProperty(usageTextProp);
    
    vtkActor2D* usageTextActor = vtkActor2D::New();
    usageTextActor->SetMapper(usageTextMapper);

    usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
    usageTextActor->GetPositionCoordinate()->SetValue(0.05,0.95);

	return usageTextActor;

}

vtkActor2D* Matrix(vtkSmartPointer<vtkMatrix4x4> MAT)
{
	/*this function is used to display the final matrix of transformation*/
	string num;
	ostringstream convert;
	for(unsigned int i=0;i<4;i++){
		for(unsigned int j=0;j<4;j++){
			convert << MAT->GetElement(i,j);
			convert << " ";
		}
		convert <<"\n";
	}
	num = convert.str();
	const char *number = num.c_str();
	char buff[200];
	const char *one = "Matrix of transformation\n\n";
	const char *two = number;
	strcpy_s(buff,one);
	strcat_s(buff,two);

	vtkTextProperty* usageTextProp = vtkTextProperty::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();
	vtkTextMapper* usageTextMapper = vtkTextMapper::New();
	usageTextMapper->SetInput(buff);
	usageTextMapper->SetTextProperty(usageTextProp);
	vtkActor2D* usageTextActor = vtkActor2D::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05,0.9);
	return usageTextActor;
}

void TomoRegistration::Compass(vtkRenderWindowInteractor* interactor)
{
	/*Create the widget and its representation*/
	vtkSmartPointer<vtkCompassRepresentation> compassRepresentation = vtkSmartPointer<vtkCompassRepresentation>::New();
	vtkCompassWidget* compassWidget = vtkCompassWidget::New();
	compassWidget->SetInteractor(interactor);
	compassWidget->SetRepresentation(compassRepresentation);
	compassWidget->EnabledOn();
}

void TomoRegistration::ChangeResolution(vtkImageData* img1, vtkImageData* img2)
{
	/*
	img1 = source.
	img2 = target.
	/*****Get data spacing********/
	double voxel1 = img1->GetSpacing()[0] * img1->GetSpacing()[1] * img1->GetSpacing()[2];
	double voxel2 = img2->GetSpacing()[0] * img2->GetSpacing()[1] * img2->GetSpacing()[2];
	/*********Chance according to the highest resolution**************/
	vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
	vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
	
	if(voxel1 < voxel2){
		reslice->SetOutputSpacing(img1->GetSpacing()[0],img1->GetSpacing()[1],img1->GetSpacing()[2]);
		//reslice->SetInformationInput(img1);
		reslice->SetOutputDimensionality(3);
		reslice->SetInputConnection(img2->GetProducerPort());
		reslice->SetInterpolationModeToCubic();
		reslice->Update();
		this->DICOM = reslice->GetOutput();
	}else{
		reslice->SetOutputSpacing(img2->GetSpacing()[0],img2->GetSpacing()[1],img2->GetSpacing()[2]);
		//reslice->SetInformationInput(img2);
		reslice->SetOutputDimensionality(3);
		reslice->SetInputConnection(img1->GetProducerPort());
		reslice->SetInterpolationModeToCubic();
		reslice->Update();
		this->DICOMS = reslice->GetOutput();
	}
	double bound[6];
	reslice->GetOutput()->GetBounds(bound);
}


/*
void TomoRegistration::Reload(vtkObject* caller,unsigned long,void*,void*)
{
	vtkRenderWindowInteractor *iren = static_cast<vtkRenderWindowInteractor*>(caller);
	std::string key = iren->GetKeySym();
	vtkPolyData* poly = static_cast<vtkPolyData*>(caller);
	if(key.compare("v") == 0){
		if(!poly)
			this->ReadSurface;
	}
}

/*
//==================SOURCE MATRIX====================//
	vtkSmartPointer<vtkMatrix4x4> matOA = vtkSmartPointer<vtkMatrix4x4>::New();
	for(unsigned int i=0;i<3;i++){
		matOA->SetElement(0,i,sourceFrame.XDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOA->SetElement(1,i,sourceFrame.YDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOA->SetElement(2,i,sourceFrame.ZDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOA->SetElement(3,i,0);
	}
	for(unsigned int i=0;i<3;i++){
		matOA->SetElement(3,i,sourceFrame.origin[i]);
	}
	matOA->SetElement(3,3,1);
	matOA->Invert();
	//==================TARGET MATRIX====================//
	vtkSmartPointer<vtkMatrix4x4> matOB = vtkSmartPointer<vtkMatrix4x4>::New();
	for(unsigned int i=0;i<3;i++){
		matOB->SetElement(0,i,targetFrame.XDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOB->SetElement(1,i,targetFrame.YDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOB->SetElement(2,i,targetFrame.ZDirection[i]);
	}
	for(unsigned int i=0;i<3;i++){
		matOB->SetElement(3,i,0);
	}
	for(unsigned int i=0;i<3;i++){
		matOB->SetElement(3,i,targetFrame.origin[i]);
	}
	matOB->SetElement(3,3,1);
	mat
	//====================TRANSFORM======================//
	vtkSmartPointer<vtkMatrix4x4> ResultMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
	ResultMatrix->Multiply4x4(matOA,matOB,ResultMatrix);

	transform->SetMatrix(ResultMatrix);*/





		/*
	else if(this->comboBox->currentIndex() == 2)
	{
		QMessageBox mb(QMessageBox::Question,"Get Dicom series","Source or Target?",QMessageBox::Yes | QMessageBox::No);
		mb.setButtonText(QMessageBox::Yes, "Source"); //DICOMS
		mb.setButtonText(QMessageBox::No, "Target");  //DICOM
		switch(mb.exec()){
		case QMessageBox::Yes:
			if(!this->DICOMS){
				QMessageBox mb(QMessageBox::Information,"Get Dicom files","Source Dicom file missing!", QMessageBox::Ok);
				mb.exec();
			}else{
				this->ReadTargetImage(this->DICOMS,this->widget.qvtkWidget1);
			}
			break;
		case QMessageBox::No:
			if(!this->DICOM){
				QMessageBox mb(QMessageBox::Information,"Get Dicom files","Target Dicom file missing!", QMessageBox::Ok);
				mb.exec();
			}else{
				this->ReadTargetImage(this->DICOM,this->widget.qvtkWidget2);
			}
			break;
		}
	}*/






//if((this->comboBox->currentIndex()==0)||(this->comboBox->currentIndex()==1))
	//{
	//	QString file = dialog->getOpenFileName(this,"obj file","",tr("image(*.obj)"));
	//	if(file.isNull()){
	//		connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	//	}else{
	//		this->DATA = this->ReadOBJ(file); // set the source object file
	//		this->ReadSurface(this->DATA,this->widget.qvtkWidget1);
	//	}
	//}else if(this->comboBox->currentIndex()==2)//.raw or .dcm file for fiducial landmark registration
	//{
	//	//For MRI and CT raw file
	//	QSettings settings("_settings.ini",QSettings::IniFormat);
	//	settings.beginGroup("Fiducial_Landmark");
	//	if(settings.value("sourceRawFile").toBool()==true){
	//		QString File = dialog->getOpenFileName(this,"raw file","",tr("image(*.raw)"));
	//		if(File.isNull()){
	//			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	//		}else{
	//			std::vector<int> dataExtent;
	//			dataExtent.push_back(settings.value("FMSVx").toInt());
	//			dataExtent.push_back(settings.value("FMSVy").toInt());
	//			dataExtent.push_back(settings.value("FMSVz").toInt());
	//			std::vector<double> voxelSize;
	//			voxelSize.push_back(settings.value("sourceVoxelSize_x").toDouble());
	//			voxelSize.push_back(settings.value("sourceVoxelSize_y").toDouble());
	//			voxelSize.push_back(settings.value("sourceVoxelSize_z").toDouble());
	//			bool type;
	//			if(settings.value("sourceShort").toBool()){
	//				type = false;
	//			}else{
	//				type = true;
	//			}
	//			this->ReadRawImages(File,dataExtent,voxelSize,type,this->widget.qvtkWidget1);
	//		}
	//	}else{
	//		//For series of Dicom images (PET)
	//		QString file = dialog->getExistingDirectory(this,tr("Open dicom Directory"),"DICOM Folder",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
	//		if(file.isNull()){
	//			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	//		}else{
	//			vtkSmartPointer<vtkDICOMImageReader> Read = vtkSmartPointer<vtkDICOMImageReader>::New();
	//			Read->SetDirectoryName(file.toStdString().c_str());
	//			Read->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::LoadDicomDirectory);
	//			Read->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	//			Read->Update();
	//			this->DICOMS = Read->GetOutput(); //set the source image data
	//			this->DisplayVolume(Read->GetOutput(),"3D model from dicom series",this->widget.qvtkWidget1,true);
	//		}
	//	}
	//	settings.endGroup();
	//}
















	//if(this->comboBox->currentIndex()==0)//profile-profile registration
	//{
	//	QString file = dialog->getOpenFileName(this,"obj file","",tr("Image(*.obj)"));
	//	if(file.isNull()){
	//		connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	//	}else{
	//		this->DATA2 = this->ReadOBJ(file);// set the target object file
	//		this->ReadSurface(this->DATA2,this->widget.qvtkWidget2);
	//	}
	//}else if(this->comboBox->currentIndex()==1)//profile-tomo registration
	//{
	//	settings.beginGroup("Profile/Profile-Profile/PET");
	//	if(settings.value("ProfileTomoRaw").toBool()==true){
	//		QString File = dialog->getOpenFileName(this,"raw file","",tr("image(*.raw)"));
	//		if(File.isNull()){
	//			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	//		}else{
	//			std::vector<int> dataExtent;
	//			dataExtent.push_back(settings.value("TPTx").toInt());
	//			dataExtent.push_back(settings.value("TPTy").toInt());
	//			dataExtent.push_back(settings.value("TPTz").toInt());
	//			std::vector<double> voxelSize;
	//			voxelSize.push_back(settings.value("VoxelSize_x").toDouble());
	//			voxelSize.push_back(settings.value("VoxelSize_y").toDouble());
	//			voxelSize.push_back(settings.value("VoxelSize_z").toDouble());
	//			bool type;
	//			if(settings.value("SurfaceShort").toBool()){
	//				type = false;
	//			}else{
	//				type = true;
	//			}
	//			this->ReadRawImages(File,dataExtent,voxelSize,type,this->widget.qvtkWidget2);
	//		}
	//	}else{
	//		QString file = dialog->getExistingDirectory(this,tr("Open dicom Directory"),"DICOM Folder",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
	//		if(file.isNull()){
	//			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	//		}else{
	//			vtkSmartPointer<vtkDICOMImageReader> Read = vtkSmartPointer<vtkDICOMImageReader>::New();
	//			Read->SetDirectoryName(file.toStdString().c_str());
	//			Read->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::LoadDicomDirectory);
	//			Read->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	//			Read->Update();
	//			this->DICOM = Read->GetOutput();
	//			this->ReadTargetImage(this->DICOM,this->widget.qvtkWidget2);
	//		}
	//	}
	//	settings.endGroup();
	//}else if(this->comboBox->currentIndex()==2)//.raw or .dcm file fiducial landmark registration
	//{
	//	settings.beginGroup("Fiducial_Landmark");
	//	if(settings.value("targetRawFile").toBool()==true){
	//		QString File = dialog->getOpenFileName(this,"raw file","",tr("image(*.raw)"));
	//		if(File.isNull()){
	//			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	//		}else{
	//			std::vector<int> dataExtent;
	//			dataExtent.push_back(settings.value("FMTVx").toInt());
	//			dataExtent.push_back(settings.value("FMTVy").toInt());
	//			dataExtent.push_back(settings.value("FMTVz").toInt());
	//			std::vector<double> voxelSize;
	//			voxelSize.push_back(settings.value("targetVoxelSize_x").toDouble());
	//			voxelSize.push_back(settings.value("targetVoxelSize_y").toDouble());
	//			voxelSize.push_back(settings.value("targetVoxelSize_z").toDouble());
	//			bool type;
	//			if(settings.value("targetShort").toBool()){
	//				type = false;
	//			}else{
	//				type = true;
	//			}
	//			this->ReadRawImages(File,dataExtent,voxelSize,type,this->widget.qvtkWidget2);
	//		}
	//	}else{
	//		QString file = dialog->getExistingDirectory(this,tr("Open dicom Directory"),"DICOM Folder",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
	//		if(file.isNull()){
	//			connect(dialog,SIGNAL(rejected()),dialog,SLOT(close()));
	//		}else{
	//			vtkSmartPointer<vtkDICOMImageReader> Read = vtkSmartPointer<vtkDICOMImageReader>::New();
	//			Read->SetDirectoryName(file.toStdString().c_str());
	//			Read->AddObserver(vtkCommand::ProgressEvent,this,&TomoRegistration::LoadDicomDirectory);
	//			Read->AddObserver(vtkCommand::EndEvent,this,&TomoRegistration::KeyCallbackFunctionEnd);
	//			Read->Update();
	//			this->DICOM = Read->GetOutput();
	//			this->DisplayVolume(Read->GetOutput(),"3D model from dicom series",this->widget.qvtkWidget2,true);
	//		}
	//	}
	//	settings.endGroup();
	//}