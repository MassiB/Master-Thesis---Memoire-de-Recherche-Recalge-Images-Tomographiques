#include "ITKRegistration.h"

//ITKRegistration::ITKRegistration()
//{
//
//}
//
//ITKRegistration::~ITKRegistration()
//{
//
//}
//
//void ITKRegistration::VTKimage2ITKimage(vtkSmartPointer<vtkImageData> imageData)
//{
//	/*for the source*/
//	int* dims = imageData->GetDimensions();
//	double dataSpacing[3];
//	imageData->GetSpacing(dataSpacing);
//	/*display the info*/
//	std::cout<<"Dims: "<<" x: "<<dims[0]<<" y: "<<dims[1]<<" z: "<<dims[2]<<std::endl;
//	std::cout<<"Number of points: "<<imageData->GetNumberOfPoints()<<std::endl;
//	std::cout<<"Number of cells: "<<imageData->GetNumberOfCells()<<std::endl;
//	
//	/*Retrieve the entries from the image data*/
//	for (int z = 0; z < dims[2]; z++){
//		for (int y = 0; y < dims[1]; y++){
//			for (int x = 0; x < dims[0]; x++){
//				double* pixel = static_cast<double*>(imageData->GetScalarPointer(x,y,z));
//			}
//		}
//	}
//
//	typedef unsigned char   PixelType;
//	const unsigned int Dimension = 3;
//	typedef itk::Image<PixelType,Dimension> ImageType;
//	typedef itk::ImportImageFilter<PixelType,Dimension> ImportFilterType;
//
//	ImportFilterType::Pointer importFilter = ImportFilterType::New();
// 
//	ImportFilterType::SizeType  size;
//	size[0]  = dims[0];  // size along X
//	size[1]  = dims[1];  // size along Y
//	size[2]  = dims[2];  // size along Z
// 
//	ImportFilterType::IndexType start;
//	start.Fill(0);
// 
//	ImportFilterType::RegionType region;
//	region.SetIndex(start);
//	region.SetSize(size);
// 
//	importFilter->SetRegion( region );
// 
//	double origin[Dimension];
//	origin[0] = 0.0;    // X coordinate
//	origin[1] = 0.0;    // Y coordinate
//	origin[2] = 0.0;    // Z coordinate
//
//	importFilter->SetOrigin( origin );
// 
//	double spacing[Dimension];
//	spacing[0] = dataSpacing[0];    // along X direction
//	spacing[1] = dataSpacing[1];    // along Y direction
//	spacing[2] = dataSpacing[2];    // along Z direction
// 
//	importFilter->SetSpacing(spacing);
//
//	const unsigned int numberOfPixels =  size[0] * size[1] * size[2];
//	PixelType* localBuffer = new PixelType[numberOfPixels];
//
//
//	const double radius = 80.0;
// 
//	const double radius2 = radius * radius;
//	PixelType * it = localBuffer;
// 
//	for(unsigned int z=0; z < size[2]; z++){
//		const double dz = static_cast<double>( z ) - static_cast<double>(size[2])/2.0;
//		for(unsigned int y=0; y < size[1]; y++){
//			const double dy = static_cast<double>( y ) - static_cast<double>(size[1])/2.0;
//			for(unsigned int x=0; x < size[0]; x++){
//				const double dx = static_cast<double>( x ) - static_cast<double>(size[0])/2.0;
//				const double d2 = dx*dx + dy*dy + dz*dz;
//				double* pix = static_cast<double*>(imageData->GetScalarPointer(x,y,z));
//				*it++ = ( d2 < radius2 ) ? pix[0] : 0;
//			}
//		}
//	}
// 
//	const bool importImageFilterWillOwnTheBuffer = true;
//	importFilter->SetImportPointer(localBuffer,numberOfPixels,importImageFilterWillOwnTheBuffer);
//
//
//	typedef itk::ImageFileWriter<ImageType> WriterType;
//	WriterType::Pointer writer = WriterType::New();
// 
//	writer->SetFileName("test.png");
//	writer->SetInput(importFilter->GetOutput());
//	writer->Update();
//}
//
//vtkImageData* ITKRegistration::VolumeRegistration(const string fixedFile,const string movingFile)
//{
//	/*se VTK's 'imageData->GetScalarPointer'demonstrated here: 
//	http://www.vtk.org/Wiki/VTK/Examples/Cxx/ImageData/IterateImageData) 
//	as the input.*/
//
//
//
//	const unsigned int Dimension = 3;
//	typedef float PixelType;
//	typedef itk::Image<PixelType,Dimension> FixedImageType;
//	typedef itk::Image<PixelType,Dimension> MovingImageType;
//	typedef itk::ImageSeriesReader<FixedImageType> ReaderType;
//	ReaderType::Pointer fixedImageReader = ReaderType::New();
//	ReaderType::Pointer movingImageReader = ReaderType::New();
//	/**********read Dicom files*********/
//
//	typedef itk::GDCMImageIO ImageIOType;
//    ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
//    fixedImageReader->SetImageIO(gdcmImageIO);
//    fixedImageReader->Update();
//    movingImageReader->SetImageIO(gdcmImageIO);
//    movingImageReader->Update();
//	/****compose tridimensional images***/
//
//	typedef itk::GDCMSeriesFileNames NamesGeneratorType;
//	NamesGeneratorType::Pointer fixednameGenerator = NamesGeneratorType::New();
//	NamesGeneratorType::Pointer movingnameGenerator = NamesGeneratorType::New();
//	fixednameGenerator->SetUseSeriesDetails(true);
//	fixednameGenerator->AddSeriesRestriction("0008|0021");
//	movingnameGenerator->SetUseSeriesDetails(true);
//	movingnameGenerator->AddSeriesRestriction("0008|0021");
//	fixednameGenerator->SetDirectory(fixedFile);
//	movingnameGenerator->SetDirectory(movingFile);
//
//	typedef std::vector<std::string> SeriesIdContainer;
//	const SeriesIdContainer &FseriesUID = fixednameGenerator->GetSeriesUIDs();
//	const SeriesIdContainer &MseriesUID = movingnameGenerator->GetSeriesUIDs();
//	std::string FseriesIdentifier;
//	FseriesIdentifier = FseriesUID.begin()->c_str();
//	std::string MseriesIdentifier;
//	MseriesIdentifier = MseriesUID.begin()->c_str();
//
//	typedef std::vector<std::string> FileNamesContainer;
//    FileNamesContainer fixedfileNames;
//	FileNamesContainer movingfileNames;
//
//    fixedfileNames = fixednameGenerator->GetFileNames(FseriesIdentifier);
//	movingfileNames = movingnameGenerator->GetFileNames(MseriesIdentifier);
//
//	fixedImageReader->SetFileNames(fixedfileNames);
//	movingImageReader->SetFileNames(movingfileNames);
//
//	/***************Volume Registration*****************/
//
//	typedef itk::VersorRigid3DTransform<double> TransformType;
//	typedef itk::VersorRigid3DTransformOptimizer OptimizerType;
//	typedef itk::MeanSquaresImageToImageMetric<FixedImageType,MovingImageType> MetricType;
//	typedef itk::LinearInterpolateImageFunction<MovingImageType,double> InterpolatorType;
//	typedef itk::ImageRegistrationMethod<FixedImageType,MovingImageType> RegistrationType;
//
//	TransformType::Pointer transform = TransformType::New();
//	OptimizerType::Pointer optimizer = OptimizerType::New();
//	MetricType::Pointer metric = MetricType::New();
//	InterpolatorType::Pointer interpolator = InterpolatorType::New();
//	RegistrationType::Pointer registration = RegistrationType::New();
//
//	registration->SetTransform(transform);
//	registration->SetOptimizer(optimizer);
//	registration->SetMetric(metric);
//	registration->SetInterpolator(interpolator);
//
//	registration->SetFixedImageRegion(fixedImageReader->GetOutput()->GetBufferedRegion());
//	typedef itk::CenteredTransformInitializer<TransformType,FixedImageType,MovingImageType>TransformInitializerType;
//	TransformInitializerType::Pointer initializer = TransformInitializerType::New();
//	initializer->SetTransform(transform);
//	initializer->SetFixedImage(fixedImageReader->GetOutput());
//	initializer->SetMovingImage(movingImageReader->GetOutput());
//	initializer->MomentsOn();
//	initializer->InitializeTransform();
//
//	typedef TransformType::VersorType VersorType;
//	typedef VersorType::VectorType VectorType;
//	VersorType rotation;
//	VectorType axis;
//	axis[0]=0.0;
//	axis[1]=0.0;
//	axis[2]=1.0;
//	const double angle = 0;
//	rotation.Set(axis,angle);
//	transform->SetRotation(rotation);
//
//	registration->SetInitialTransformParameters(transform->GetParameters());
//	typedef OptimizerType::ScalesType OptimizerScalesType;
//	OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());
//	const double translationScale = 1.0/1000.0;
//	optimizerScales[0] = 1.0;
//	optimizerScales[1] = 1.0;
//	optimizerScales[2] = 1.0;
//	optimizerScales[3] = translationScale;
//	optimizerScales[4] = translationScale;
//	optimizerScales[5] = translationScale;
//	optimizer->SetScales(optimizerScales);
//	optimizer->SetMaximumStepLength(0.2000);
//	optimizer->SetMinimumStepLength(0.0001);
//	optimizer->SetNumberOfIterations(200);
//	/***********CallBack Function*************/
//
//	CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
//	optimizer->AddObserver(itk::IterationEvent(), observer);
//
//	registration->Update();
//	std::cout<<"Optimizer stop condition: "<<registration->GetOptimizer()->GetStopConditionDescription()<<std::endl;
//
//	OptimizerType::ParametersType finalParameters = registration->GetLastTransformParameters();
//	const double versorX = finalParameters[0];
//	const double versorY  = finalParameters[1];
//	const double versorZ = finalParameters[2];
//	const double finalTranslationX = finalParameters[3];
//	const double finalTranslationY = finalParameters[4];
//	const double finalTranslationZ = finalParameters[5];
//	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
//	const double bestValue = optimizer->GetValue();
//
//	cout<<"Registration done!"<<endl;
//	cout<<"Result = "<<endl;
//	cout<<"Versor X = "<<versorX<<endl;
//	cout<<"Versor Y = "<<versorY<<endl;
//	cout<<"Versor Z = "<<versorZ<<endl;
//	cout<<"Translation X = "<<finalTranslationX<<endl;
//	cout<<"Translation Y = "<<finalTranslationY<<endl;
//	cout<<"Translation Z = "<<finalTranslationZ<<endl;
//	cout<<"Iterations = "<<numberOfIterations<<endl;
//	cout<<"Metric value = "<<bestValue<<endl;
//
//	transform->SetParameters(finalParameters);
//	TransformType::MatrixType matrix = transform->GetMatrix();
//	TransformType::OffsetType offset = transform->GetOffset();
//	cout<<"Matrix = "<<endl<<matrix<<endl;
//	cout<<"Offset = "<<endl<<offset<<endl;
//
//	typedef itk::ResampleImageFilter<MovingImageType,FixedImageType > ResampleFilterType;
//	TransformType::Pointer finalTransform = TransformType::New();
//	finalTransform->SetCenter(transform->GetCenter());
//	finalTransform->SetParameters(finalParameters);
//	finalTransform->SetFixedParameters(transform->GetFixedParameters());
//	  
//	ResampleFilterType::Pointer resampler = ResampleFilterType::New();
//	resampler->SetTransform(finalTransform);
//	resampler->SetInput(movingImageReader->GetOutput());
//	 
//	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
//	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
//	resampler->SetOutputOrigin(fixedImage->GetOrigin());
//	resampler->SetOutputSpacing(fixedImage->GetSpacing());
//	resampler->SetOutputDirection(fixedImage->GetDirection());
//	resampler->SetDefaultPixelValue(100);
//
//	typedef itk::Image<float,Dimension> DifferenceImageType;
//	typedef itk::SubtractImageFilter<FixedImageType,FixedImageType,DifferenceImageType> DifferenceFilterType;
//	DifferenceFilterType::Pointer difference =  DifferenceFilterType::New();
//	difference->SetInput1(fixedImageReader->GetOutput());
//	difference->SetInput2(resampler->GetOutput());
//	resampler->SetDefaultPixelValue(1);
//
//	typedef float OutputPixelType;
//	typedef itk::Image<OutputPixelType,Dimension> OutputImageType;
//	typedef itk::RescaleIntensityImageFilter<DifferenceImageType,OutputImageType> RescaleType;
//	RescaleType::Pointer intensityRescaler = RescaleType::New();
//	intensityRescaler->SetInput(difference->GetOutput());
//	intensityRescaler->SetOutputMinimum(0);
//	intensityRescaler->SetOutputMaximum(255);
//
//	typedef itk::ImageToVTKImageFilter<OutputImageType>ConnectorType;
//	ConnectorType::Pointer connector = ConnectorType::New();
//	connector->SetInput(intensityRescaler->GetOutput());
//
//	double *range = connector->GetOutput()->GetScalarRange();
//	vtkLookupTable *bwLut = vtkLookupTable::New();
//    bwLut->SetTableRange(range);
//    bwLut->SetNumberOfColors(256);
//	bwLut->SetSaturationRange(0,1);
//    bwLut->SetHueRange(0.66667,0);
//    bwLut->SetValueRange(0,0.66667);
//	bwLut->SetScaleToLog10();
//    bwLut->Build();
//
//	vtkSmartPointer<vtkImageMapToColors> Reader = vtkSmartPointer<vtkImageMapToColors>::New();
//	Reader->SetInput(connector->GetOutput());
//	Reader->SetOutputFormatToRGBA();
//	Reader->SetLookupTable(bwLut);
//	Reader->Update();
//
//	return Reader->GetOutput();
//}
