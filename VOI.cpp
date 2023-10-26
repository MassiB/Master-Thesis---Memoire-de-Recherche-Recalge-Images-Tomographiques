#include "VOI.h"


VOI::VOI()
{

}

VOI::~VOI()
{

}

/***************Extract volume & centroid calculation****************/
void VOI::Centroid(vtkSmartPointer<vtkImageData> extractedImageData,std::vector<double> vec,vtkRenderer* ren,int number)
{
	/*this function is used to calculate the centroid of fiducial landmark according the scalar values of each voxels
	extractedImageData = data input.
	vec = the boxWidget dimension.
	ren = the current rendering.
	number = current boxWidget.
	/***first we need to get the image data spacing***/
	double gs[3];
	extractedImageData->GetSpacing(gs);
	/**and then get the pixel representation**/
	vec[0] = vec[0] / gs[0];
	vec[1] = vec[1] / gs[0];
	vec[2] = vec[2] / gs[1];
	vec[3] = vec[3] / gs[1];
	vec[4] = vec[4] / gs[2];
	vec[5] = vec[5] / gs[2];

	for(unsigned int i=0;i<6;i++){
		vec[i] = static_cast<int>(ceil(vec[i]));
    }

	/*****centroid calculation*****/
	vtkSmartPointer<vtkExtractVOI> extractVOI = vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputConnection(extractedImageData->GetProducerPort());
	extractVOI->SetVOI(vec[0],vec[1],vec[2],vec[3],vec[4],vec[5]);
	extractVOI->Update();

	vtkSmartPointer<vtkImageDataGeometryFilter> imageDataGeometryFilter = vtkSmartPointer<vtkImageDataGeometryFilter>::New();
	imageDataGeometryFilter->SetInputConnection(extractVOI->GetOutputPort());
	imageDataGeometryFilter->Update();

	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter = vtkSmartPointer<vtkCenterOfMass>::New();
	centerOfMassFilter->SetInput(imageDataGeometryFilter->GetOutput());
	centerOfMassFilter->SetUseScalarsAsWeights(true);
	centerOfMassFilter->Update();
 
	double center[3];
	centerOfMassFilter->GetCenter(center);

	vec.clear();
	vec.push_back(center[0]);
	vec.push_back(center[1]);
	vec.push_back(center[2]);
	if(this->window == true){
		targetCentroid.push_back(vec);
	}else{
		sourceCentroid.push_back(vec);
	}
	//============================== Annotation ====================================//
	std::ostringstream posx, posy, posz;
	posx << center[0];
	posy << center[1];
	posz << center[2];
	std::string c;
	std::stringstream out;
	out << number;
	c = out.str();
    char buff[100];
    strcpy_s(buff," Fiducial Marker ");
	strcat_s(buff,c.c_str());
	strcat_s(buff,"\n");
	strcat_s(buff,posx.str().c_str());
	strcat_s(buff,"  ");
	strcat_s(buff,posy.str().c_str());
	strcat_s(buff,"  ");
	strcat_s(buff,posz.str().c_str());

	vtkSmartPointer<vtkVectorText> text = vtkSmartPointer<vtkVectorText>::New();
	text->SetText(buff);
	vtkSmartPointer<vtkPolyDataMapper> textMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	textMapper->SetInputConnection(text->GetOutputPort());
	vtkSmartPointer<vtkFollower> textActor = vtkSmartPointer<vtkFollower>::New();
	textActor->SetMapper(textMapper);
	textActor->SetScale(2,2,2);
	textActor->SetPosition(vec[0],vec[1],vec[2]);
	ren->AddActor(textActor);
	textActor->SetCamera(ren->GetActiveCamera());
}

void VOI::Centroid2(vtkSmartPointer<vtkImageData> extractedImageData,std::vector<double> vec,vtkRenderer* ren,int number)
{
	/*this is the second method to calculate the centroid*/
	double gs[3];
	extractedImageData->GetSpacing(gs);
	/**and then get the pixel representation**/
	vec[0] = vec[0] / gs[0];
	vec[1] = vec[1] / gs[0];
	vec[2] = vec[2] / gs[1];
	vec[3] = vec[3] / gs[1];
	vec[4] = vec[4] / gs[2];
	vec[5] = vec[5] / gs[2];

	for(unsigned int i=0;i<6;i++){
		vec[i] = static_cast<int>(ceil(vec[i]));
    }

	double xc=0, yc=0, zc=0;
	double totalScalar=0;
	/***find the total sum of weight***/
	for(unsigned int k=vec[4]; k<vec[5]; k++){
		for(unsigned int j=vec[2]; j<vec[3]; j++){
			for(unsigned int i=vec[0]; i<vec[1]; i++){
				totalScalar += extractedImageData->GetScalarComponentAsDouble(i,j,k,0);		 
			}
		}
	}
	/****find x centroid****/
	for(unsigned int k=vec[4]; k<vec[5]; k++){
		for(unsigned int j=vec[2]; j<vec[3]; j++){
			for(unsigned int i=vec[0]; i<vec[1]; i++){
				xc += i*extractedImageData->GetScalarComponentAsDouble(i,j,k,0);
			}
		}
	}
	xc = (xc/totalScalar)*gs[0];
	/****find y centroid****/
	for(unsigned int k=vec[4]; k<vec[5]; k++){
		for(unsigned int i=vec[0]; i<vec[1]; i++){
			for(unsigned int j=vec[2]; j<vec[3]; j++){
				yc += j*extractedImageData->GetScalarComponentAsDouble(i,j,k,0);
			}
		}
	}
	yc = (yc/totalScalar)*gs[1];
	/****find z centroid****/
	for(unsigned int j=vec[2]; j<vec[3]; j++){
		for(unsigned int i=vec[0]; i<vec[1]; i++){
			for(unsigned int k=vec[4]; k<vec[5]; k++){
				zc += k*extractedImageData->GetScalarComponentAsDouble(i,j,k,0);
			}
		}
	}
	zc = (zc/totalScalar)*gs[2];
	vec.clear();
	vec.push_back(xc);
	vec.push_back(yc);
	vec.push_back(zc);
	if(this->window == true){
		targetCentroid.push_back(vec);
	}else{
		sourceCentroid.push_back(vec);
	}
	//============================== Annotation ====================================//
	std::ostringstream posx, posy, posz;
	posx << xc;
	posy << yc;
	posz << zc;
	std::string c;
	std::stringstream out;
	out << number;
	c = out.str();
    char buff[100];
    strcpy_s(buff," Fiducial Marker ");
	strcat_s(buff,c.c_str());
	strcat_s(buff,"\n");
	strcat_s(buff,posx.str().c_str());
	strcat_s(buff,"  ");
	strcat_s(buff,posy.str().c_str());
	strcat_s(buff,"  ");
	strcat_s(buff,posz.str().c_str());

	vtkSmartPointer<vtkVectorText> text = vtkSmartPointer<vtkVectorText>::New();
	text->SetText(buff);
	vtkSmartPointer<vtkPolyDataMapper> textMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	textMapper->SetInputConnection(text->GetOutputPort());
	vtkSmartPointer<vtkFollower> textActor = vtkSmartPointer<vtkFollower>::New();
	textActor->SetMapper(textMapper);
	textActor->SetScale(2,2,2);
	textActor->SetPosition(vec[0],vec[1],vec[2]);
	ren->AddActor(textActor);
	textActor->SetCamera(ren->GetActiveCamera());
}

void VOI::ExtractVolume(vtkSmartPointer<vtkImageData> img,std::vector<std::vector<double>> vector)
{
	/*
	img = the volume data.
	vector = boxWidget dimension. 
	*/
	/***************************Visualisation****************************/
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->SetInteractorStyle(style);
	vtkSmartPointer<vtkRenderer> Renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(Renderer);
	/*************************volume of interest*************************/
	std::vector<vtkSmartPointer<vtkVolume>> volumeVector;

	for(unsigned int i=0; i<3; i++){
		//centroidVector.clear();
		std::vector<double> centroidVector(vector[i].begin(),vector[i].end());
		vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
		volumeMapper->SetInputConnection(img->GetProducerPort());
		volumeMapper->AutoAdjustSampleDistancesOff();
		volumeMapper->CroppingOn();
		volumeMapper->SetCroppingRegionPlanes(vector[i][0],vector[i][1],vector[i][2],vector[i][3],vector[i][4],vector[i][5]);
		volumeMapper->SetCroppingRegionFlagsToSubVolume();
		/*Adjust opacity*/
		vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
		vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
		compositeOpacity->AddPoint(img->GetScalarRange()[1]*15/255,0.0);
		compositeOpacity->AddPoint(img->GetScalarRange()[1]*70/255,0.2);
		compositeOpacity->AddPoint(img->GetScalarRange()[1]*75/255,0.3);
		compositeOpacity->AddPoint(img->GetScalarRange()[1]*78/255,1);
		compositeOpacity->AddPoint(img->GetScalarRange()[1]*82/255,1);
		compositeOpacity->AddPoint(img->GetScalarRange()[1]*90/255,0.3);
		compositeOpacity->AddPoint(img->GetScalarRange()[1]*135/255,0.2);
		compositeOpacity->AddPoint(img->GetScalarRange()[1]*152/255,0.2);
		compositeOpacity->AddPoint(img->GetScalarRange()[1],0.2);
		compositeOpacity->Update();
		/*Adjust color*/
		vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
		color->AddRGBPoint(img->GetScalarRange()[1]*0/255,0.0,0.0,0.0);
		color->AddRGBPoint(img->GetScalarRange()[1]*20/255,0.0,0.0,1.0);
		color->AddRGBPoint(img->GetScalarRange()[1]*50/255,0.9,0.37,0.27);
		color->AddRGBPoint(img->GetScalarRange()[1]*80.0/255,0.15,0.15,0.5);
		color->AddRGBPoint(img->GetScalarRange()[1]*120.0/255,1.0,1.0,1.0);
		color->AddRGBPoint(img->GetScalarRange()[1]*160.0/255,1.0,0.0,0.0);
		color->AddRGBPoint(img->GetScalarRange()[1]*200.0/255,0.0,0.0,0.0);
		color->AddRGBPoint(img->GetScalarRange()[1],1.0,1.0,1.0);
		/*set volume property*/
		volumeProperty->SetInterpolationTypeToLinear();
		volumeProperty->SetColor(color);
		volumeProperty->SetScalarOpacity(compositeOpacity);
		volumeProperty->ShadeOn();
		volumeProperty->SetAmbient(0.2);
		volumeProperty->SetDiffuse(0.6);
		volumeProperty->SetSpecular(5.0);

		vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
		volume->SetMapper(volumeMapper);
		volume->SetProperty(volumeProperty);

		volumeVector.push_back(volume);
		this->Centroid2(img,centroidVector,Renderer,i+1);
		centroidVector.clear();
	}

	for(unsigned int i=0; i<3; i++){
		Renderer->AddVolume(volumeVector[i]);
	}
	Renderer->SetBackground(0.5,0.5,0.94);
	Renderer->GradientBackgroundOn();
	Renderer->AddViewProp(this->annotation(20,this->window,2));
	Renderer->ResetCamera();
	this->axes(Renderer,interactor);
	renderWindow->Render();
	interactor->Start();
}

/**************************Data encapsulation***************************/
void VOI::SetRENDER(vtkRenderer* a)
{
	this->rend = a;
}

void VOI::SetCurrentWindow(bool a)
{
	this->window = a;
}

/*****************************Utilities******************************/
void VOI::axes(vtkSmartPointer<vtkRenderer> ren, vtkRenderWindowInteractor *iren)
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

vtkSmartPointer<vtkCornerAnnotation> VOI::annotation(int size,bool win,int Pos)
{
	const char *text;
	if(win == true){
		text = "Extracted target volume";
	}else{
		text = "Extracted source volume";
	}
	vtkSmartPointer<vtkCornerAnnotation> annot = vtkSmartPointer<vtkCornerAnnotation>::New();
	annot->GetTextProperty()->SetFontFamilyToCourier();
    annot->SetLinearFontScaleFactor(2);
    annot->SetNonlinearFontScaleFactor(1);
    annot->SetMaximumFontSize(size);
    annot->UseBoundsOn();
    annot->SetText(Pos, text);
    return annot;
}



//
//vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper1 = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
//	volumeMapper1->SetInputConnection(img->GetProducerPort());
//	volumeMapper1->AutoAdjustSampleDistancesOff();
//	volumeMapper1->CroppingOn();
//	volumeMapper1->SetCroppingRegionPlanes(vector[0][0],vector[0][1],vector[0][2],vector[0][3],vector[0][4],vector[0][5]);
//	volumeMapper1->SetCroppingRegionFlagsToSubVolume();
//	std::vector<double> copyRow1(vector[0].begin(),vector[0].end());
//
//	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper2 = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
//	volumeMapper2->SetInputConnection(img->GetProducerPort());
//	volumeMapper2->AutoAdjustSampleDistancesOff();
//	volumeMapper2->CroppingOn();
//	volumeMapper2->SetCroppingRegionPlanes(vector[1][0],vector[1][1],vector[1][2],vector[1][3],vector[1][4],vector[1][5]);
//	volumeMapper2->SetCroppingRegionFlagsToSubVolume();
//	std::vector<double> copyRow2(vector[1].begin(),vector[1].end());
//
//	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper3 = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
//	volumeMapper3->SetInputConnection(img->GetProducerPort());
//	volumeMapper3->AutoAdjustSampleDistancesOff();
//	volumeMapper3->CroppingOn();
//	volumeMapper3->SetCroppingRegionPlanes(vector[2][0],vector[2][1],vector[2][2],vector[2][3],vector[2][4],vector[2][5]);
//	volumeMapper3->SetCroppingRegionFlagsToSubVolume();
//	std::vector<double> copyRow3(vector[2].begin(),vector[2].end());
//
//	/*Adjust opacity*/
//    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
//    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
//	compositeOpacity->AddPoint(img->GetScalarRange()[1]*15/255,0.0);
//	compositeOpacity->AddPoint(img->GetScalarRange()[1]*70/255,0.2);
//	compositeOpacity->AddPoint(img->GetScalarRange()[1]*75/255,0.3);
//	compositeOpacity->AddPoint(img->GetScalarRange()[1]*78/255,1);
//	compositeOpacity->AddPoint(img->GetScalarRange()[1]*82/255,1);
//	compositeOpacity->AddPoint(img->GetScalarRange()[1]*90/255,0.3);
//	compositeOpacity->AddPoint(img->GetScalarRange()[1]*135/255,0.2);
//	compositeOpacity->AddPoint(img->GetScalarRange()[1]*152/255,0.2);
//	compositeOpacity->AddPoint(img->GetScalarRange()[1],0.2);
//	compositeOpacity->Update();
//	/*Adjust color*/
//    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
//	color->AddRGBPoint(img->GetScalarRange()[1]*0/255,0.0,0.0,0.0);
//	color->AddRGBPoint(img->GetScalarRange()[1]*20/255,0.0,0.0,1.0);
//	color->AddRGBPoint(img->GetScalarRange()[1]*50/255,0.9,0.37,0.27);
//	color->AddRGBPoint(img->GetScalarRange()[1]*80.0/255,0.15,0.15,0.5);
//    color->AddRGBPoint(img->GetScalarRange()[1]*120.0/255,1.0,1.0,1.0);
//    color->AddRGBPoint(img->GetScalarRange()[1]*160.0/255,1.0,0.0,0.0);
//    color->AddRGBPoint(img->GetScalarRange()[1]*200.0/255,0.0,0.0,0.0);
//    color->AddRGBPoint(img->GetScalarRange()[1],1.0,1.0,1.0);
//	*set volume property*/
//	volumeProperty->SetInterpolationTypeToLinear();
//    volumeProperty->SetColor(color);
//    volumeProperty->SetScalarOpacity(compositeOpacity);
//    volumeProperty->ShadeOn();
//    volumeProperty->SetAmbient(0.2);
//    volumeProperty->SetDiffuse(0.6);
//    volumeProperty->SetSpecular(5.0);
//
//    vtkSmartPointer<vtkVolume> volume1 = vtkSmartPointer<vtkVolume>::New();
//    volume1->SetMapper(volumeMapper1);
//    volume1->SetProperty(volumeProperty);
//
//	vtkSmartPointer<vtkVolume> volume2 = vtkSmartPointer<vtkVolume>::New();
//    volume2->SetMapper(volumeMapper2);
//    volume2->SetProperty(volumeProperty);
//
//	vtkSmartPointer<vtkVolume> volume3 = vtkSmartPointer<vtkVolume>::New();
//    volume3->SetMapper(volumeMapper3);
//    volume3->SetProperty(volumeProperty);



	//std::vector<double> copyRow1(vector[0].begin(),vector[0].end());
	//std::vector<double> copyRow2(vector[1].begin(),vector[1].end());
	//std::vector<double> copyRow3(vector[2].begin(),vector[2].end());
	///*compute the centroid*/
	//this->Centroid(img,copyRow1,Renderer,1);
	//this->Centroid(img,copyRow2,Renderer,2);
	//this->Centroid(img,copyRow3,Renderer,3);