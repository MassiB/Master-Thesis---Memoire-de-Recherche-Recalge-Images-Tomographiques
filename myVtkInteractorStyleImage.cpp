#include "myVtkInteractorStyleImage.h"

void myVtkInteractorStyleImage::SetImageViewer(vtkImageViewer2* imageViewer){
	    this->ImageViewer = imageViewer;
        MinSlice = imageViewer->GetSliceMin();
        MaxSlice = imageViewer->GetSliceMax();
        Slice = MinSlice;
}

void myVtkInteractorStyleImage::SetStatusMapper(vtkTextMapper* statusMapper){
	this->StatusMapper = statusMapper;
}
