#ifndef POLYDATAINTERACTORSTYLE_H
#define POLYDATAINTERACTORSTYLE_H

#include "tomoregistration.h"

class PolydataInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static PolydataInteractorStyle* New();
	vtkTypeMacro(PolydataInteractorStyle , vtkInteractorStyleTrackballCamera);

public:
	virtual void OnKeyPress();
	void SetRenderer(vtkRenderer*);
	void SetPolydata(vtkPolyData*);
private:
	vtkSmartPointer<vtkRenderer> rend;
	vtkSmartPointer<vtkPolyData> polydata;
};

#endif