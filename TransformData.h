#ifndef TRANSFORMDATA_H
#define	TRANSFORMDATA_H

#include "ui_TransformGUI.h"
#include "tomoregistration.h"

class TransformData: public QDialog
{
	Q_OBJECT
public:
	TransformData();
    ~TransformData();

	double ScalingValue();
	vtkSmartPointer<vtkMatrix4x4> MatrixOfTransformation();

private:
	Ui::TransformData widget;
};


#endif