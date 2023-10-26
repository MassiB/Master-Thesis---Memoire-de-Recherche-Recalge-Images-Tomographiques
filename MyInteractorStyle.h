#ifndef MYINTERACTORSTYLE_H
#define MYINTERACTORSTYLE_H

#include "tomoregistration.h"

//Create a box widget
class myBoxCallback : public vtkCommand
{
	/*class used to control box widget*/
public:
	myBoxCallback(){flag=0;}
	static myBoxCallback *New(){return new myBoxCallback;}

	void SetCurrentBox(int);
	void SetCurrentWindow(bool a);
	void SetRender(vtkRenderer*);
	virtual void Execute(vtkObject *caller, unsigned long, void*);

protected:
	bool win;
	vtkRenderer* ren;
	std::vector<double> vec;
	int flag;
};
//class used to interact myBoxCallback
class MyInteractorStyle : public vtkInteractorStyleTrackballCamera
{
	/*class used to add ruler and box widget*/
public:
	MyInteractorStyle(){this->count=0; this->voi = VOI::New();}
public:
	static MyInteractorStyle* New();
	vtkTypeMacro(MyInteractorStyle, vtkInteractorStyleTrackballCamera);
	/*encapsulation*/
	void SetRenderer(vtkRenderer*);
	void CurrentWindow(bool);
	void SetImageData(vtkImageData*);
	/*key press event*/
	virtual void OnKeyPress();

protected:
	int count; // used to set the current number of boxWidget.
	bool window; // used to set the current window (source or target).
private:
	vtkRenderer* ren; // current renderer.
	vtkSmartPointer<vtkImageData> imageData;// current volume.
	VOI* voi;//class to extract volume of interest.
};

#endif