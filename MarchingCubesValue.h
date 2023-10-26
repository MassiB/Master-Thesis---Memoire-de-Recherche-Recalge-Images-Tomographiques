#ifndef MARCHINGCUBESVALUE_H
#define	MARCHINGCUBESVALUE_H

#include "ui_MarchingCubesValue.h"

class MarchingCubesValue : public QDialog, Ui::MarchingCubes
{
	Q_OBJECT
public:
	MarchingCubesValue(){widget.setupUi(this);}

	~MarchingCubesValue(){}

	double mcValue(){
		return widget.doubleSpinBox->value();
	}

private:
	Ui::MarchingCubes widget;
};

#endif