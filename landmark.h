#ifndef LANDMARK_H
#define LANDMARK_H
#include "tomoregistration.h"

//class used to manage position and number of landmarks
class vtkMyCallback : public vtkCommand 
{ 
public:

	vtkMyCallback();

	static vtkMyCallback *New(){return new vtkMyCallback;} 

	void SetSelectionActor(vtkActor*);

	void SetRenderer(vtkRenderer*);

	void SetWindow(bool);

	virtual void Execute(vtkObject *caller, unsigned long, void*);

private:
	int count;
	bool win;
	vector<double> row; 
	vector<vector<double>> matrix; 
	vtkActor* SelectedActor;
	vtkRenderer* ren;
};

#endif