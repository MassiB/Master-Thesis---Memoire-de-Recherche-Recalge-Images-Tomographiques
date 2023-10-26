#ifndef MYVTKINTERACTORSTYLEIMAGE_H
#define	MYVTKINTERACTORSTYLEIMAGE_H
#include "tomoregistration.h"
#include <sstream>

class StatusMessage
{  
public:
    static std::string Format(int slice, int maxSlice){
        std::stringstream tmp;
        tmp<<"Slice Number: "<< slice+1 <<"/"<<maxSlice +1;
        return tmp.str();
    }
};

class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
    myVtkInteractorStyleImage(): ImageViewer(NULL),StatusMapper(NULL){};  
public:
    static myVtkInteractorStyleImage *New();
    vtkTypeMacro(myVtkInteractorStyleImage,vtkInteractorStyleImage);
protected:
    vtkImageViewer2* ImageViewer;
    vtkTextMapper* StatusMapper; 
	int Slice;
    int MinSlice;
    int MaxSlice;
public:
    void SetImageViewer(vtkImageViewer2* );
    void SetStatusMapper(vtkTextMapper* );
protected:
    void MoveSliceForward(){
        if (Slice < MaxSlice){
            Slice += 1;
            this->ImageViewer->SetSlice(Slice);   
            std::string msg = StatusMessage::Format(Slice, MaxSlice);
            StatusMapper->SetInput(msg.c_str());
            ImageViewer->Render();
        }
    }
    void MoveSliceBackward(){
        if (Slice > MinSlice){
            Slice -= 1;      
            this->ImageViewer->SetSlice(Slice); ;   
            std::string msg = StatusMessage::Format(Slice, MaxSlice);
            StatusMapper->SetInput(msg.c_str());
            ImageViewer->Render();
        }
    }
    virtual void OnKeyDown(){
        std::string key = this->GetInteractor()->GetKeySym();
        if(key.compare("Up") == 0){
            MoveSliceForward();
        }
        else if(key.compare("Down") == 0){
            MoveSliceBackward();
        }
    }
};


#endif