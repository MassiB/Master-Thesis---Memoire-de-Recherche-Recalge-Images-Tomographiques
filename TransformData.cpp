#include "TransformData.h"

TransformData::TransformData()
{
	widget.setupUi(this);
	/**fill the QTableWidget with identity matrix**/
	for(unsigned int row=0;row<4;row++){
		for(unsigned int column=0;column<4;column++){
			QTableWidgetItem* newItem = new QTableWidgetItem();
			if(row == column){
				newItem->setText(QString::number(1));
			}else{
				newItem->setText(QString::number(0));
			}
			this->widget.matrixTable->setItem(row,column,newItem);
		}
	}
}

TransformData::~TransformData()
{
}

double TransformData::ScalingValue()
{
	return widget.scalingValue->value();
}

vtkSmartPointer<vtkMatrix4x4> TransformData::MatrixOfTransformation()
{
	vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();

	for(unsigned int i=0;i<4;i++){
		for(unsigned int j=0;j<4;j++){
			QTableWidgetItem *item = this->widget.matrixTable->item(i,j);
			matrix->SetElement(i,j,item->text().toDouble());
		}
	}
	
	return matrix;
}