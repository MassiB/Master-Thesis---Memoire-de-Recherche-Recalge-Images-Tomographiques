#ifndef DATADIMENSION_H
#define	DATADIMENSION_H

#include "tomoregistration.h"
#include "ui_DataDimension.h"

class dataDimension : public QDialog
{
	Q_OBJECT
public:
	dataDimension();
	~dataDimension();

	std::vector<int> DataDimensionVector();
	std::vector<double> VoxelDimensionVector();
	bool Tomography();
	bool dataType();

private:
	Ui::DataDimension widget;
};

#endif