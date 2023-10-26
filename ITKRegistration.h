#ifndef ITKREGISTRATION_H
#define	ITKREGISTRATION_H

#include "tomoregistration.h"
#include "qstring.h"
/*****************ITK HEADERS***************/
/*
#include "itkMacro.h"
#include "itkImageRegistrationMethod.h"
#include "itkTranslationTransform.h"
#include "itkAffineTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkMutualInformationImageToImageMetric.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkMutualInformationHistogramImageToImageMetric.h"
#include "itkGradientDescentOptimizer.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkResampleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkExceptionObject.h"
#include "itkExtractImageFilter.h"
#include "itkImageToVTKImageFilter.h"
#include "itkNormalizeImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkCheckerBoardImageFilter.h"
#include "itkCommand.h"
#include "itkCenteredRigid2DTransform.h"
#include "itkVersorRigid3DTransform.h"
#include "itkVersorRigid3DTransformOptimizer.h"
#include "itkCenteredTransformInitializer.h"
#include "itkHistogramToEntropyImageFilter.h"

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkVTKImageToImageFilter.h"
#include "itkImportImageFilter.h"

using namespace std;

class CommandIterationUpdate : public itk::Command
{
public:
	typedef CommandIterationUpdate Self;
	typedef itk::Command Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	itkNewMacro(Self);
protected:
	CommandIterationUpdate(){};
public:
	typedef itk::VersorRigid3DTransformOptimizer OptimizerType;
	typedef const OptimizerType *OptimizerPointer;
	void Execute(itk::Object *caller, const itk::EventObject & event){
		Execute((const itk::Object *)caller, event);
	}
	void Execute(const itk::Object *object, const itk::EventObject & event){
		OptimizerPointer optimizer = dynamic_cast< OptimizerPointer >( object );
		if(!itk::IterationEvent().CheckEvent(&event)){
			return;
		}
		cout<<optimizer->GetCurrentIteration()<<"   ";
		cout<<optimizer->GetValue()<<"   ";
		cout<<optimizer->GetCurrentPosition()<<endl;
	}
};

class ITKRegistration
{
public:
	ITKRegistration();
	~ITKRegistration();

	vtkImageData* VolumeRegistration(const string,const string);

	void VTKimage2ITKimage(vtkSmartPointer<vtkImageData>);

	void GetTomoregistration(TomoRegistration *a){
		this->in = a;
	}

private:
	TomoRegistration* in;
};*/

#endif