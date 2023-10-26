#include "PolydataInteractorStyle.h"


void PolydataInteractorStyle::OnKeyPress()
{
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	//get key event
	if(key == "d"){
		cout<<"ok I'm in"<<endl;
		//vtkSmartPointer<vtkTransform> scale = vtkSmartPointer<vtkTransform>::New();
		//scale->Scale(25.4,25.4,25.4);
		//vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		//transformFilter->SetInput(this->polydata);
		//transformFilter->SetTransform(scale);
		//transformFilter->Update();
		vtkSmartPointer<vtkCursor3D> cursor = vtkSmartPointer<vtkCursor3D>::New();
		cursor->SetModelBounds(this->polydata->GetBounds()[0],this->polydata->GetBounds()[1],this->polydata->GetBounds()[2],this->polydata->GetBounds()[3],this->polydata->GetBounds()[4],this->polydata->GetBounds()[5]);
		cursor->AllOn();
		cursor->OutlineOn();
		cursor->Update();


		vtkSmartPointer<vtkPolyDataMapper> map = vtkSmartPointer<vtkPolyDataMapper>::New();
		map->SetInputConnection(cursor->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(map);
		actor->GetProperty()->SetColor(0,1,0);

		this->rend->AddActor(actor);
		this->Interactor->Render();
	}
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}

/**************Encapsulation*****************/
void PolydataInteractorStyle::SetPolydata(vtkPolyData* data)
{
	this->polydata = data;
}

void PolydataInteractorStyle::SetRenderer(vtkRenderer* render)
{
	this->rend = render;
}