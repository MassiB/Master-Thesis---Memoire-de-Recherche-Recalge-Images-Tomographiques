#ifndef VOI_H
#define	VOI_H

#include "tomoregistration.h"

class VOI : public vtkInteractorStyleTrackballCamera
{
public:
	static VOI *New(){return new VOI;} 

	VOI();
	~VOI();

	void SetRENDER(vtkRenderer*);
	// compute centroid
	void Centroid(vtkSmartPointer<vtkImageData>,std::vector<double>,vtkRenderer*,int);
	void Centroid2(vtkSmartPointer<vtkImageData>,std::vector<double>,vtkRenderer*,int);
	// extract volume of interest
	void ExtractVolume(vtkSmartPointer<vtkImageData>,std::vector<std::vector<double>>);
	void SetCurrentWindow(bool);

	vtkSmartPointer<vtkCornerAnnotation> annotation(int,bool,int);
	void axes(vtkSmartPointer<vtkRenderer> ren, vtkRenderWindowInteractor *iren);

private:
	vtkSmartPointer<vtkRenderer> rend;
	bool window;
};

#endif