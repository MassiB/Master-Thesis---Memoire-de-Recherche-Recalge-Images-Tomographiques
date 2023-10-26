#ifndef TOMOREGISTRATION_H
#define TOMOREGISTRATION_H

#include <QtGui/QMainWindow>
#include "ui_tomoregistration.h"
/*******************************VTK header*************************************/
#include "vtkInteractorStyleImage.h"
#include "vtkTextMapper.h"
#include "vtkImageViewer2.h"
#include "vtkImageViewer.h"
#include <vtkRenderer.h>
#include "vtkDataSetMapper.h"
#include "QVTKWidget.h"
#include <vtkPointData.h>
#include <vtkRendererCollection.h>
#include <vtkPlanes.h>
#include "vtkPlane.h"
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkIdFilter.h>
#include "vtkIdList.h"
#include "vtkHardwareSelector.h"
#include "vtkSelection.h"
#include "vtkExtractSelection.h"
#include "vtkPolyDataWriter.h"
#include "vtkGenericDataObjectWriter.h"
#include "vtkOBJReader.h"
#include "vtkOBJExporter.h"
#include "vtkAlgorithmOutput.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkJPEGReader.h"
#include "vtkImageActor.h"
#include "vtkActor2D.h"
#include "vtkImageMapper.h"
#include "vtkImageMapper3D.h"
#include "QVTKWidget.h"
#include "vtkImageLaplacian.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageCast.h"
#include "vtkImageData.h"
#include "vtkAxesActor.h"
#include "vtkAxes.h"
#include "vtkTexture.h"
#include "vtkTextureMapToPlane.h"
#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkAppendPolyData.h"
#include "vtkAppendFilter.h"
#include "vtkImageReader2Factory.h"
#include "vtkImageReader2.h"
#include "vtkImageReader.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#include "vtkLegendScaleActor.h"
#include "vtkOutlineFilter.h"
#include "vtkVideoSource.h"
#include "vtkTextProperty.h"
#include "vtkTextMapper.h"
#include "vtkObjectFactory.h"
#include "vtkOBJReader.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkInteractorStyleTrackballActor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkRectilinearWipeWidget.h"
#include "vtkRectilinearWipeRepresentation.h"
#include "vtkRectangularButtonSource.h"
#include "vtkImageRectilinearWipe.h"
#include "vtkProperty2D.h"
#include "vtkProperty.h"
#include "vtkCommand.h"
#include "vtkImageMapToColors.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkScalarsToColors.h"
#include "vtkLookupTable.h"
#include "vtkImageLuminance.h"
#include "vtkGenericDataObjectReader.h"
#include "vtkVolume16Reader.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkAbstractVolumeMapper.h"
#include "vtkTesting.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkCollection.h"
#include "vtkImageClip.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkHAVSVolumeMapper.h"
#include "vtkThreshold.h"
#include "vtkDataSetTriangleFilter.h"
#include "vtkHAVSVolumeMapper.h"
#include "vtkContourFilter.h"
#include "vtkCursor3D.h"
#include "vtkCutter.h"
#include "vtkMath.h"
#include "vtkJPEGWriter.h"
#include "vtkCornerAnnotation.h"
#include "vtkVersion.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkImageThreshold.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkImageToImageFilter.h"
#include "vtkAbstractVolumeMapper.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkMarchingCubes.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkStructuredPointsReader.h"
#include "vtkSurfaceReconstructionFilter.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkImageStencil.h"
#include "vtkDelaunay3D.h"
#include "vtkDecimatePro.h"
#include "vtkDelaunay2D.h"
#include "vtkDataSet.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkXYPlotActor.h"
#include "vtkCubeAxesActor2D.h"
#include "vtkCubeAxesActor.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkExtractEdges.h"
#include "vtkGradientFilter.h"
#include "vtkTubeFilter.h"
#include "vtkVertexGlyphFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkClipPolyData.h"
#include "vtkClipVolume.h"
#include "vtkStructuredGridReader.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkIdFilter.h"
#include "vtkAreaPicker.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTriangleFilter.h"
#include "vtkTextActor.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkIterativeClosestPointTransform.h"
#include "vtkLandmarkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkMatrix3x3.h"
#include "vtkImagePlaneWidget.h"
#include "vtkPLYWriter.h"
#include "vtkFileOutputWindow.h"
#include "vtkFillHolesFilter.h"
#include "vtkScalarBarActor.h"
#include "vtkScalarBarWidget.h"
#include "vtkButtonWidget.h"
#include "vtkBoxWidget.h"
#include "vtkBoxWidget2.h"
#include "vtkSphereWidget.h"
#include "vtkSphereWidget2.h"
#include "vtkImplicitPlaneWidget.h"
#include "vtkImplicitPlaneWidget2.h"
#include "vtkSplineWidget.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkCellPicker.h"
#include "vtkPoints.h"
#include "vtkLine.h"
#include "vtkCamera.h"
#include "vtkIdTypeArray.h"
#include "vtkInformation.h"
#include "vtkSelection.h"
#include "vtkSelectVisiblePoints.h"
#include "vtkSelectionNode.h"
#include "vtkCellCenters.h"
#include "vtkUnstructuredGrid.h"
#include "vtkImageChangeInformation.h"
#include "vtkImageReslice.h"
#include "vtkImageReslice.h"
#include "vtkFiniteDifferenceGradientEstimator.h"
#include "vtkVolumeTextureMapper3D.h"
#include "vtkOpenGLVolumeTextureMapper3D.h"
#include "vtkVolumeRayCastIsosurfaceFunction.h"
#include "vtkVolumeRayCastMIPFunction.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkVoxelModeller.h"
#include "vtkWindowedSincPolyDataFilter.h"
#include "vtkKdTree.h"
#include "vtkKdTreePointLocator.h"
#include "vtkPolyDataReader.h"
#include "vtkWindow.h"
#include "vtkObject.h"
#include "vtkCallbackCommand.h"
#include "vtkProgrammableFilter.h"
#include "vtkVRMLExporter.h"
#include "vtkDataReader.h"
#include "vtkMaskPoints.h"
#include "vtkInteractorStyleRubberBandZoom.h"
#include "vtkInteractorStyleUnicam.h"
#include "vtkExtractUnstructuredGrid.h"
#include "vtkCastToConcrete.h"
#include "vtkWarpVector.h"
#include "vtkExtractSelectedPolyDataIds.h"
#include "vtkHomogeneousTransform.h"
#include "vtkAbstractTransform.h"
#include "vtkExtractCells.h"
#include "vtkArray.h"
#include "vtkType.h"
#include "vtkDataSetCollection.h"
#include "vtkInteractorStyleFlight.h"
#include "vtkInteractorStyleTerrain.h"
#include "vtkInteractorStyleSwitch.h"
#include "vtkImageMagnitude.h"
#include "vtkImageAccumulate.h"
#include "vtkBarChartActor.h"
#include "vtkIntArray.h"
#include "vtkLegendBoxActor.h"
#include "vtkPolyDataAlgorithm.h"
#include "vtkVoxelContoursToSurfaceFilter.h"
#include "vtkVoxelModeller.h"
#include "vtkDistanceWidget.h"
#include "vtkPointHandleRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkDistanceRepresentation3D.h"
#include "vtkPolygonalSurfacePointPlacer.h"
#include "vtkUnsignedCharArray.h"
#include "QVTKWidget.h"
#include "vtkPointPicker.h"
#include "vtkGlyph3D.h"
#include "vtkCylinderSource.h"
#include "vtkSphereSource.h"
#include "vtkConeSource.h"
#include "vtkLODActor.h"
#include "vtkFollower.h"
#include "vtkVectorText.h"
#include "vtkCubeSource.h"
#include "vtkElevationFilter.h"
#include "vtkImplicitDataSet.h"
#include "vtkImplicitBoolean.h"
#include "vtkSampleFunction.h"
#include "vtkSurfaceReconstructionFilter.h"
#include "vtkProbeFilter.h"
#include "vtkMetaImageReader.h"
#include "vtkPropCollection.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkDiscreteMarchingCubes.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkContourWidget.h"
#include "vtkBoxRepresentation.h"
#include "vtkSliderWidget.h"
#include "vtkSliderRepresentation3D.h"
#include "vtkSliderRepresentation2D.h"
#include "vtkWidgetEvent.h"
#include "vtkExtractVOI.h"
#include "vtkVolumeMapper.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkWin32OpenGLRenderWindow.h"
#include "vtkBalloonRepresentation.h"
#include "vtkBalloonWidget.h"
#include "vtkCompassWidget.h"
#include "vtkCompassRepresentation.h"
#include "vtkCenterOfMass.h"
#include "vtkDoubleArray.h"
#include "vtkTable.h"
#include "vtkContextView.h"
#include "vtkChartXY.h"
#include "vtkPen.h"
#include "vtkPlot.h"
#include "vtkVersion.h"
#include "vtkContextScene.h"
#include "vtkTexturedButtonRepresentation2D.h"
#include "vtkCoordinate.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkCaptionRepresentation.h"
#include "vtkCaptionWidget.h"
#include "vtkCaptionActor2D.h"
#include "vtkAssignAttribute.h"
#include "vtkArrowSource.h"
#include "vtkImageRGBToHSV.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageGradient.h"
#include "vtkImageShiftScale.h"
#include "vtkFieldDataToAttributeDataFilter.h"
#include "vtkImageMathematics.h"
#include "vtkActorCollection.h"
#include "vtkInteractorStyleUser.h"
/********************************Qt header*************************************/
#include "QtGui/QApplication"
#include "iostream"
#include "fstream"
#include "string"
#include <vector>
#include <fstream>
#include "QtGui/QtGui"
#include <QtCore>
#include "QSplashScreen"
#include "QApplication"
#include "QMainWindow.h"
#include "QMenu"
#include "qfile.h"
#include "QVTKWidget.h"
#include "QDesktopServices"
#include "QDialog.h"
#include "QSettings"
#include "QStringList"
#include "QString"
#include "qlist.h"
#include "qmessagebox.h"
#include "qspinbox.h"
#include "qcombobox.h"
#include "qgridlayout.h"
#include "qdialogbuttonbox.h"
#include "qpushbutton.h"
#include "qprogressbar.h"
#include "qcursor.h"
#include "qprogressdialog.h"
#include "qlayout.h"
#include "qtimer.h"
#include "qcheckbox.h"
#include "qtablewidget.h"
#include <sstream>
#include "QuickView.h"

#include "ProgressBarGUI.h"
#include "MarchingCubesValue.h"
#include "VOI.h"

using namespace std;

class vtkEventQtSlotConnect;
//vector used for profile-profile and profile-tomo registration
extern std::vector<std::vector<double>> sourceLandmark;
extern std::vector<std::vector<double>> targetLandmark;
//vector used for boxwidget
extern std::vector<std::vector<double>> sourceFiducialLandmark;
extern std::vector<std::vector<double>> targetFiducialLandmark;
//vector used for fiducial markers
extern std::vector<std::vector<double>> sourceCentroid;
extern std::vector<std::vector<double>> targetCentroid;

//main class
class TomoRegistration : public QMainWindow, Ui::TomoRegistrationClass
{
	Q_OBJECT
public:
	TomoRegistration(QWidget *parent = 0, Qt::WFlags flags = 0);
public:
	~TomoRegistration();
public:
	//read image
	void ReadSurface(vtkPolyData*,QVTKWidget*);
    void ReadDicomSeries(vtkImageData*,QVTKWidget*);
	void ReadRawImages(QString,std::vector<int>,std::vector<double>,bool,QVTKWidget*);
	vtkSmartPointer<vtkPolyData> ReadOBJ(QString);
	vtkSmartPointer<vtkImageData> ReadDicomDirectory(QString,bool);
	//Utilities
    void Axes(vtkSmartPointer<vtkRenderer>, vtkRenderWindowInteractor*);
	void Compass(vtkRenderWindowInteractor*);
	vtkSmartPointer<vtkActor2D> ImageInfo(vtkImageData*);
	vtkSmartPointer<vtkActor2D> DataInfo(vtkPolyData*);
    vtkSmartPointer<vtkPolyDataConnectivityFilter> ExtractSurface(vtkImageData*,double);
    vtkSmartPointer<vtkCornerAnnotation> Annotation(int, const char*, int);
	void ChangeResolution(vtkImageData*,vtkImageData*);
	//function used for surface and fiducial landamrk registration
	void LandmarkBased();
	void ICP(vtkPolyData*,vtkPolyData*);
	void FiducialLandmarkRegistration();
	//callback function
	void KeyCallbackFunction(vtkObject* caller,long unsigned int eventId,void* callData);
	void KeyCallbackFunctionEnd(vtkObject* caller,long unsigned int eventId,void* callData);
	void RegistrationProgressFunction(vtkObject* caller,long unsigned int eventId,void* callData);
	void LoadRawFile(vtkObject* caller,long unsigned int eventId,void* callData);
	void LoadObjFile(vtkObject* caller,long unsigned int eventId,void* callData);
	void LoadDicomDirectory(vtkObject* caller,long unsigned int eventId,void* callData);
	//functions related to volume processing
	vtkSmartPointer<vtkVolume> GetVolume(vtkImageData*,int);
	void PaulViola(vtkImageData*,vtkImageData*);
	vtkSmartPointer<vtkPolyData> ImageData2Polydata(vtkImageData*,vtkTransform*,bool);
	void DisplayVolume(vtkImageData*,const char*,QVTKWidget*,int);
	//void Reload(vtkObject*,unsigned long,void*,void*);

	void GetCleanData(vtkPolyData*,vtkImageData*,int,int);

public slots:
	void OpenSource();
	void OpenTarget();
	void ApplyRegistration(); 
	void Save();
    void Reset();
	void Get3D();
	void DisplayDicomSeries();
	void GetSurface();
	void OpenSettings();
	void OpenDataProcess();
	void exit();
	void slot_clicked(vtkObject*,unsigned long,void*,void*);
	void slot_Box(vtkObject*,unsigned long,void*,void*);
	
private:
	Ui::TomoRegistrationClass widget;
protected:
	vtkSmartPointer<vtkPolyData> DATA;//source .obj
    vtkSmartPointer<vtkPolyData> DATA2;//target .obj
    vtkSmartPointer<vtkImageData> DICOM;//target dicom series
	vtkSmartPointer<vtkImageData> DICOMS;//source dicom series
	vtkSmartPointer<vtkImageReader> RawReader;//.raw data
    
	QMessageBox msgBox;
	QProgressBar* progressBar;
	QComboBox* comboBox;

	vtkSmartPointer<vtkEventQtSlotConnect> Connections;//qt-vtk events
};
//Structure used for landamrk-based registration
struct Frame 
{ 
	Frame(double p1[3], double p2[3], double p3[3]){ 
		this->setOrigin(p1); 
		this->setXDirection(p2); 
		this->setZDirection(p3); 
		this->setYDirection(); 
		this->Centroid(p1,p2,p3);

		cout<<"Origin: "<<this->origin[0]<<" "<<this->origin[1]<<" "<<this->origin[2]<<endl; 
		cout<<"X direction: "<<this->XDirection[0]<<" "<<this->XDirection[1]<<" "<<this->XDirection[2]<<endl; 
		cout<<"Y direction: "<<this->YDirection[0]<<" "<<this->YDirection[1]<<" "<<this->YDirection[2]<<endl; 
		cout<<"Z direction: "<<this->ZDirection[0]<<" "<<this->ZDirection[1]<<" "<<this->ZDirection[2]<<endl; 
	} 

	double origin[3]; 
	double XDirection[3]; 
	double YDirection[3]; 
	double ZDirection[3];
	double centroid[3];
	//Create polydata for landmarks
	void CreatePolydata(vtkPolyData* polydata){
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		points->InsertNextPoint(this->origin);
		double x[3];
		vtkMath::Add(this->origin,this->XDirection,x);
		points->InsertNextPoint(x);
		double y[3];
		vtkMath::Add(this->origin,this->YDirection,y);
		points->InsertNextPoint(y);
		double z[3];
		vtkMath::Add(this->origin,this->ZDirection,z);
		points->InsertNextPoint(z);
		polydata->SetPoints(points);
		
		vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
		vertexGlyphFilter->AddInput(polydata);
		vertexGlyphFilter->Update();

		polydata->ShallowCopy(vertexGlyphFilter->GetOutput());
	}
        
	void setOrigin(double o[3]){ 
		this->origin[0] = o[0]; 
		this->origin[1] = o[1]; 
		this->origin[2] = o[2]; 
	} 
	void setXDirection(double direction[3]){ 
		double a[3]; 
		vtkMath::Subtract(direction,this->origin,a); 
		vtkMath::Normalize(a); 
		this->XDirection[0] = a[0]; 
		this->XDirection[1] = a[1]; 
		this->XDirection[2] = a[2];
	} 
	void setYDirection(){
		double a[3];
		vtkMath::Cross(this->ZDirection,this->XDirection,a);
		vtkMath::Normalize(a);
		this->YDirection[0] = a[0];
		this->YDirection[1] = a[1];
		this->YDirection[2] = a[2];
	} 
	void setZDirection(double direction[3]){ 
		double a[3]; 
		vtkMath::Subtract(direction,this->origin,a); 
		vtkMath::Normalize(a);
		double b[3];
		vtkMath::Cross(this->XDirection,a,b); 
		vtkMath::Normalize(b);
		this->ZDirection[0] = b[0];
		this->ZDirection[1] = b[1];
		this->ZDirection[2] = b[2]; 
	}
	void Centroid(double point1[3],double point2[3],double point3[3]){
		double a[3];
		vtkMath::Add(point1,point2,a);
		vtkMath::Add(point3,a,a);
		vtkMath::MultiplyScalar(a,0.33333333333);
		this->centroid[0] = a[0];
		this->centroid[1] = a[1];
		this->centroid[2] = a[2];
	}
};

struct Frame2 
{ 
	Frame2(double p1[3], double p2[3], double p3[3]){ 
		this->setOrigin(p1); 
		this->setXDirection(p2); 
		this->setYDirection(p3); 
		this->setZDirection();
		this->Centroid(p1,p2,p3);
	} 

	double origin[3]; 
	double XDirection[3]; 
	double YDirection[3]; 
	double ZDirection[3];
	double centroid[3];
	//Create polydata for landmarks
	void CreatePolydata(vtkPolyData* polydata){
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		points->InsertNextPoint(this->origin);
		double x[3];
		vtkMath::Add(this->origin,this->XDirection,x);
		points->InsertNextPoint(x);
		double y[3];
		vtkMath::Add(this->origin,this->YDirection,y);
		points->InsertNextPoint(y);
		double z[3];
		vtkMath::Add(this->origin,this->ZDirection,z);
		points->InsertNextPoint(z);
		polydata->SetPoints(points);
		
		vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
		vertexGlyphFilter->AddInput(polydata);
		vertexGlyphFilter->Update();

		polydata->ShallowCopy(vertexGlyphFilter->GetOutput());
	}
        
	void setOrigin(double o[3]){ 
		this->origin[0] = o[0]; 
		this->origin[1] = o[1]; 
		this->origin[2] = o[2]; 
	} 
	void setXDirection(double direction[3]){ 
		double a[3]; 
		vtkMath::Subtract(direction,this->origin,a); 
		vtkMath::Normalize(a); 
		this->XDirection[0] = a[0]; 
		this->XDirection[1] = a[1]; 
		this->XDirection[2] = a[2];
	} 
	void setYDirection(double direction[3]){
		/*double a[3]; 
		vtkMath::Subtract(direction,this->origin,a); 
		vtkMath::Normalize(a);
		this->YDirection[0] = a[0];
		this->YDirection[1] = a[1];
		this->YDirection[2] = a[2];*/
		double a[3];
		double b[3];
		vtkMath::Subtract(direction,this->origin,a);
		vtkMath::Subtract(direction,this->origin,b);
		double y[3];
		double dot = vtkMath::Dot(a,this->XDirection);
		vtkMath::MultiplyScalar(a,dot);
		vtkMath::Subtract(b,a,y);
		vtkMath::Normalize(y);
		this->YDirection[0] = y[0];
		this->YDirection[1] = y[1];
		this->YDirection[2] = y[2];
	} 
	void setZDirection(){ 
		double a[3]; 
		vtkMath::Cross(this->XDirection,this->YDirection,a); 
		vtkMath::Normalize(a);
		this->ZDirection[0] = a[0];
		this->ZDirection[1] = a[1];
		this->ZDirection[2] = a[2]; 
	}
	void Centroid(double point1[3],double point2[3],double point3[3]){
		double a[3];
		vtkMath::Add(point1,point2,a);
		vtkMath::Add(point3,a,a);
		vtkMath::MultiplyScalar(a,0.33333333333);
		this->centroid[0] = a[0];
		this->centroid[1] = a[1];
		this->centroid[2] = a[2];
	}
};

#endif // TOMOREGISTRATION_H
