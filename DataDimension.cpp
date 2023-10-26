#include "DataDimension.h"

dataDimension::dataDimension()
{
	this->widget.setupUi(this);
}

dataDimension::~dataDimension()
{

}

std::vector<int> dataDimension::DataDimensionVector()
{
	std::vector<int> vecDataExtend;
	vecDataExtend.push_back(this->widget.DataX->value());
	vecDataExtend.push_back(this->widget.DataY->value());
	vecDataExtend.push_back(this->widget.DataZ->value());

	return vecDataExtend;
}

std::vector<double> dataDimension::VoxelDimensionVector()
{
	std::vector<double> Voxel;
	Voxel.push_back(this->widget.VoxelSize_x->value());
	Voxel.push_back(this->widget.VoxelSize_y->value());
	Voxel.push_back(this->widget.VoxelSize_z->value());

	return Voxel;
}

bool dataDimension::Tomography()
{
	if(this->widget.MRI->isChecked()){
		return true;
	}else{
		return false;
	}
}

bool dataDimension::dataType()
{
	if(this->widget.Short->isChecked()){
		return true;
	}else{
		return false;
	}
}
