#include "landmark.h"

using namespace std;

vtkMyCallback::vtkMyCallback()
{
	SelectedActor = NULL;
	ren = NULL;
	count = 0;
}

void vtkMyCallback::Execute(vtkObject *caller, unsigned long, void*)
{
	vtkRenderWindowInteractor *iren = reinterpret_cast<vtkRenderWindowInteractor*>(caller); 
	vtkPointPicker *picker = (vtkPointPicker *)iren->GetPicker(); 
	double picked[3];
	picker->GetPickPosition(picked);

	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetPhiResolution(20);
	sphere->SetThetaResolution(20);
	sphere->SetRadius(2);
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();

	vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
	glyph->SetInput(sphere->GetOutput());
	glyph->SetSource(cone->GetOutput());
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleModeToScaleByVector();
	glyph->SetScaleFactor(0.25);
	glyph->Update();

	vtkSmartPointer<vtkAppendPolyData> apd = vtkSmartPointer<vtkAppendPolyData>::New();
	apd->AddInputConnection(glyph->GetOutputPort());
	apd->AddInputConnection(sphere->GetOutputPort());
	apd->Update();
	
	std::string c;
	std::stringstream out;
	out << this->count + 1;
	c = out.str();
    char buff[30];
    strcpy_s(buff," Landmark ");
	strcat_s(buff,c.c_str());
	
	vtkSmartPointer<vtkVectorText> text = vtkSmartPointer<vtkVectorText>::New();
	text->SetText(buff);
	vtkSmartPointer<vtkPolyDataMapper> textMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	textMapper->SetInputConnection(text->GetOutputPort());
	vtkSmartPointer<vtkFollower> textActor = vtkSmartPointer<vtkFollower>::New();
	textActor->SetMapper(textMapper);
	textActor->SetScale(2,2,2);

	vtkSmartPointer<vtkPolyDataMapper> maceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	maceMapper->SetInputConnection(apd->GetOutputPort());
	vtkSmartPointer<vtkLODActor> actor = vtkSmartPointer<vtkLODActor>::New();
	actor->SetMapper(maceMapper);
	actor->VisibilityOn();
	double R= rand()/static_cast<double>(RAND_MAX); 
	double G= rand()/static_cast<double>(RAND_MAX);
	double B= rand()/static_cast<double>(RAND_MAX);
	textActor->GetProperty()->SetColor(R,G,B);
	actor->GetProperty()->SetColor(R,G,B);
	actor->PickableOff();

	this->row.clear();
	if(picker->GetPointId()!=-1 && this->count < 3){ 
		if(SelectedActor){ 
			SelectedActor->SetPosition(picker->GetPickPosition());
			actor->SetPosition(picker->GetPickPosition());
			textActor->SetPosition(picker->GetPickPosition());

			this->ren->AddActor(actor);
			this->ren->AddActor(textActor);
			textActor->SetCamera(this->ren->GetActiveCamera());
			this->count += 1; 
			for(unsigned int i=0;i<3;i++){ 
				this->row.push_back(picked[i]); 
			} 
			this->matrix.push_back(row); 
			
			if(this->win == true){
				sourceLandmark.clear();
				sourceLandmark = this->matrix;
			}else{
				targetLandmark.clear();
				targetLandmark = this->matrix;
			}

			iren->Render(); 
		}
	}
}

void vtkMyCallback::SetSelectionActor(vtkActor* pvtkActorSelection)
{
	SelectedActor = pvtkActorSelection; 
}

void vtkMyCallback::SetRenderer(vtkRenderer* RENDER){
	this->ren = RENDER;
}

void vtkMyCallback::SetWindow(bool window)
{
	this->win = window;
}
