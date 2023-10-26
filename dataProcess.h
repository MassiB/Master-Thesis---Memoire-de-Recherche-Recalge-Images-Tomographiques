#ifndef DATAPROCESS_H
#define	DATAPROCESS_H

#include "ui_DataProcessGUI.h"
#include "tomoregistration.h"
#include "DataDimension.h"

extern std::vector<double> cubeDimension;
//QDialog class
class DataProcess : public QDialog, public vtkInteractorStyleTrackballCamera, Ui::DataProcess
{    
    Q_OBJECT  
public: 
    DataProcess();
    ~DataProcess();

	void ReadDicomDir(vtkImageData*,int);
    void Visualize(vtkAlgorithmOutput*);

	void Slider(vtkRenderWindowInteractor*,vtkImageData*,vtkContourFilter*);
	void Button(vtkRenderWindowInteractor*,vtkSmartPointer<vtkRenderer>);

	void take(TomoRegistration *a){
		this->in = a;
	}

public slots:
	void OpenFile();
    void Export();
    void ExtractLargestSurface();
	void ExtractSurface();
    void AddData();
    void TrimCells();
    void Clipping();
	void FillHoles();
    void Transform();

private:
	Ui::DataProcess widget;
    vtkSmartPointer<vtkPolyData> data;
	vtkSmartPointer<vtkImageData> dicom;
	TomoRegistration* in;
	int TypeOfData;//0 for obj file, 1 for dicom series
};
//class used for box widget
class myCallback : public vtkCommand
{
public:
  static myCallback *New(){return new myCallback;}

  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
      vtkTransform *t = vtkTransform::New();
      vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
      widget->GetTransform(t);
      widget->GetProp3D()->SetUserTransform(t);
	  double d[6];
	  widget->GetProp3D()->GetBounds(d);
	  cubeDimension.clear();
	  for(unsigned int i=0;i<6;i++){
		  cubeDimension.push_back(widget->GetProp3D()->GetBounds()[i]);
	  }
      t->Delete();
    }
};
//class used for interaction
class MyDataProcessInteractor: public vtkInteractorStyleTrackballCamera
{
public:
    //std::list<int> List;
    static MyDataProcessInteractor* New();
    vtkTypeMacro(MyDataProcessInteractor,vtkInteractorStyleTrackballCamera);

    MyDataProcessInteractor(){
        appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
		flag = 0;
    }

	void SetRENDERER(vtkRenderer* ren){
		this->Renderer = ren;
	}
	
	virtual void OnRightButtonDown(){

		vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	}
	
	/*
    virtual void OnLeftButtonDown(){
		/*
        int* pos = this->GetInteractor()->GetEventPosition();
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);
        picker->Pick(pos[0],pos[1],0,this->GetCurrentRenderer());
        
        if(picker->GetCellId() != -1){
            vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(picker->GetCellId());
             
            vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);
              
            vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
            selection->AddNode(selectionNode);
                
            vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
            extractSelection->SetInput(0,this->Data);
            extractSelection->SetInput(1,selection);
            extractSelection->Update();
            
            // Get the selected cells
            vtkSmartPointer<vtkUnstructuredGrid> selected = vtkSmartPointer<vtkUnstructuredGrid>::New();
            selected->ShallowCopy(extractSelection->GetOutput());
            // Get the unselected cells

            appendFilter->AddInput(selected);
            List.push_back(picker->GetCellId());
            int flag = 1;
            for(std::list<int>::iterator it = List.begin(); it != List.end(); it++){
                if(*it == picker->GetCellId()){
                    flag = 0;
                }
            }
            if(flag == 1){
                List.push_back(picker->GetCellId());
            }
            
            vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            mapper->SetInputConnection(appendFilter->GetOutputPort());
            actor->SetMapper(mapper);
            actor->GetProperty()->EdgeVisibilityOn();
            actor->GetProperty()->SetEdgeColor(1,0,0);
            actor->GetProperty()->SetColor(0.5,0,1);
            actor->GetProperty()->SetLineWidth(3);
               
            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
        }
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
	
    virtual void OnRightButtonDown(){
		/*
        int* pos = this->GetInteractor()->GetEventPosition();
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);
        picker->Pick(pos[0],pos[1],0,this->GetCurrentRenderer());
        if(picker->GetCellId() != -1){
            int i=0;
            for(std::list<int>::iterator it = List.begin(); it != List.end(); it++){
                if(*it == picker->GetCellId()){
                    List.remove(*it);
                    break;
                }
                i++;
            }
            vtkSmartPointer<vtkDataSetCollection> collection = vtkSmartPointer<vtkDataSetCollection>::New();
            collection = appendFilter->GetInputList();
            vtkDataSet* selected = collection->GetItem(i);
            appendFilter->RemoveInput(selected);
            appendFilter->Update();
            collection = appendFilter->GetInputList();
            
            vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            mapper->SetInputConnection(appendFilter->GetOutputPort());
            actor->SetMapper(mapper);
            actor->GetProperty()->EdgeVisibilityOn();
            actor->GetProperty()->SetEdgeColor(1,0,0);
            actor->GetProperty()->SetColor(0.5,0,1);
            actor->GetProperty()->SetLineWidth(3);
             
            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
        }
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }*/

	virtual void OnKeyPress()
	{
		vtkRenderWindowInteractor *rwi = this->Interactor;
		std::string key = rwi->GetKeySym();
		/*Press b to set a box widget and d to set a ruler*/
		vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
		cube->SetBounds(-10,10,-10,10,-10,10);
		vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		cubeMapper->SetInput(cube->GetOutput());
		vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
		cubeActor->SetMapper(cubeMapper);
		cubeActor->GetProperty()->SetColor(0.0,1.0,0.0);
		cubeActor->GetProperty()->SetOpacity(0.5);

		if(key == "b"){
			if(this->flag < 1){
				this->flag +=1;
				this->Renderer->AddActor(cubeActor);
				vtkBoxWidget* boxWidget = vtkBoxWidget::New();
				boxWidget->SetInteractor(this->Interactor);
				boxWidget->SetPlaceFactor(1.25);
				boxWidget->SetProp3D(cubeActor);
				boxWidget->PlaceWidget();
				boxWidget->GetFaceProperty()->SetColor(0.0,0.0,1.0);
				boxWidget->GetFaceProperty()->SetOpacity(0.5);
				boxWidget->GetHandleProperty()->SetColor(1.0,0.3,0.5);
				boxWidget->GetHandleProperty()->SetOpacity(0.5);
				boxWidget->SetHandleSize(0.005);

				myCallback* callback = myCallback::New();
				boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
				boxWidget->On();
			}
		}
		if(key == "d"){
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
		if(key == "l"){
			vtkActorCollection* actorCollection = this->Renderer->GetActors();
			actorCollection->InitTraversal();
			for(vtkIdType i=0; i<actorCollection->GetNumberOfItems(); i++){
				vtkActor* nextActor = actorCollection->GetNextActor();
				std::string className = nextActor->GetClassName();
				std::string wantedClass = "vtkBoxWidget";
				cout<<"next actor: "<<i<<" "<<nextActor->GetClassName()<<endl;

				if(className == wantedClass){
					cout<<"I will delete this class"<<endl;
					cout<<"nextActor: "<< i << "is a polydata"<<endl;
				}
			}
		}
		if(key == "x"){
			this->Renderer->RemoveActor(cubeActor);
		}
		vtkInteractorStyleTrackballCamera::OnKeyPress();
	}

protected:
	int flag;
private:
    vtkPolyData* Data;
    vtkSmartPointer<vtkRenderer> Renderer;
    vtkSmartPointer<vtkAppendFilter> appendFilter;
};

/*class for isoContours*/
class vtkSliderCallback : public vtkCommand
{
public:
  static vtkSliderCallback *New(){return new vtkSliderCallback;}

  virtual void Execute(vtkObject *caller, unsigned long, void*){
	  vtkSliderWidget *sliderWidget = reinterpret_cast<vtkSliderWidget*>(caller);
	  double value = static_cast<vtkSliderRepresentation *>(sliderWidget->GetRepresentation())->GetValue();
	  this->ContourFilter->GenerateValues(1, value, value);
  }
  vtkSliderCallback():ContourFilter(NULL){}
  vtkContourFilter* ContourFilter;
};

#endif