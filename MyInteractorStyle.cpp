#include "MyInteractorStyle.h"
#include "VOI.h"

void myBoxCallback::SetCurrentBox(int a)
{
	this->flag = a;
}

void myBoxCallback::SetCurrentWindow(bool a)
{
	this->win = a;
}

void myBoxCallback::SetRender(vtkRenderer* render)
{
	this->ren = render;
}

void myBoxCallback::Execute(vtkObject *caller, unsigned long, void*)
{
	vtkTransform *t = vtkTransform::New();
	vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
	widget->GetTransform(t);
	widget->GetProp3D()->SetUserTransform(t);
	//get the cube bounds
	double bounds[6];
	widget->GetProp3D()->GetBounds(bounds);
	this->vec.clear(); 
	for(unsigned int i=0; i<6; i++){
		this->vec.push_back(bounds[i]);
	}
	if(this->win == true){
		/*true for target window*/
		targetFiducialLandmark.erase(targetFiducialLandmark.begin() + this->flag - 1);
		targetFiducialLandmark.insert(targetFiducialLandmark.begin() + this->flag - 1, this->vec);
	}else{
		/*false for source window*/
		sourceFiducialLandmark.erase(sourceFiducialLandmark.begin() + this->flag - 1);
		sourceFiducialLandmark.insert(sourceFiducialLandmark.begin() + this->flag - 1, this->vec);
	}
	t->Delete();
}

//===================================================================//

void MyInteractorStyle::SetRenderer(vtkRenderer* render)
{
	this->ren = render;
}

void MyInteractorStyle::CurrentWindow(bool win)
{
	this->window = win;
}

void MyInteractorStyle::OnKeyPress()
{
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	//get key event
	if(key == "b"){
		/*press b to add box widget*/
		if(this->count < 3){
			this->count +=1;
			/*Create an actor for box widget*/
			vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
			cube->SetBounds(0,20,0,20,0,20);
			vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			cubeMapper->SetInput(cube->GetOutput());
			vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
			cubeActor->SetMapper(cubeMapper);
			cubeActor->GetProperty()->SetColor(0.0,0.0,1.0);
			cubeActor->GetProperty()->SetOpacity(0.5);
			//add cube to renderer
			this->ren->AddActor(cubeActor);
			//call the box widget
			vtkBoxWidget* boxWidget = vtkBoxWidget::New();
			boxWidget->SetInteractor(this->Interactor);
			boxWidget->SetPlaceFactor(1.25);
			boxWidget->SetProp3D(cubeActor);
			boxWidget->PlaceWidget();
			boxWidget->GetFaceProperty()->SetColor(0.0,0.0,1.0);
			boxWidget->GetFaceProperty()->SetOpacity(0.5);
			boxWidget->GetHandleProperty()->SetColor(0.3,1.0,0.5);
			boxWidget->GetHandleProperty()->SetOpacity(0.5);
			boxWidget->SetHandleSize(0.005);
			//callback function
			myBoxCallback* callback = myBoxCallback::New();
			callback->SetCurrentBox(this->count);
			callback->SetRender(this->ren);
			callback->SetCurrentWindow(this->window);
			boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
			boxWidget->On();
		}
	}
	if(key == "d"){
		/***Press d to add a ruler***/
		vtkPointHandleRepresentation3D* handle = vtkPointHandleRepresentation3D::New();
		handle->SmoothMotionOn();
		vtkDistanceRepresentation3D* rep = vtkDistanceRepresentation3D::New();
		rep->SetHandleRepresentation(handle);
		vtkDistanceWidget* w = vtkDistanceWidget::New();
		w->SetRepresentation(rep);
		w->CreateDefaultRepresentation();
		static_cast<vtkDistanceRepresentation*>(w->GetRepresentation())->SetLabelFormat("%-#6.3g mm");
		w->SetInteractor(this->Interactor);
		w->On();
	}
	if(key == "v"){
		/*Press v to extract the volume of interest
		you need at least 3 boxes*/
		if(this->count < 4){
			voi->SetRENDER(this->ren);
			if(this->window == true){//true for target image
				targetCentroid.clear();
				voi->SetCurrentWindow(true);
				voi->ExtractVolume(this->imageData,targetFiducialLandmark);//targetFiducialLandmark =  target boxWidget dimension
			}else{// for source image
				sourceCentroid.clear();
				voi->SetCurrentWindow(false);
				voi->ExtractVolume(this->imageData,sourceFiducialLandmark);//targetFiducialLandmark =  source boxWidget dimension
			}
		}
	}
	if(key == "x")
	{
		this->Interactor->Render();
	}
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}

void MyInteractorStyle::SetImageData(vtkImageData* img)
{
	this->imageData = img;
}
