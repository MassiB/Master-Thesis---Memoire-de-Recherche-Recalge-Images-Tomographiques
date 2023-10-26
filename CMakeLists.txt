cmake_minimum_required(VERSION 2.8)

PROJECT(TomoRegistration)

FIND_PACKAGE(ITK  REQUIRED) 
INCLUDE(${ITK_USE_FILE}) 

FIND_PACKAGE(VTK REQUIRED) 
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(Qt4)
INCLUDE(${QT_USE_FILE}) 


SET(TomoPROJECT_SRCS
main.cpp
DataProcess.cpp
Landmark.cpp
settings.cpp
tomoregistration.cpp
myVtkInteractorStyleImage.cpp
ITKRegistration.cpp
VOI.cpp
MyInteractorStyle.cpp
PolydataInteractorStyle.cpp
TransformData.cpp
DataDimension.cpp
ProgressBarGUI.cpp

tomoregistration.h
dataProcess.h
landmark.h
myVtkInteractorStyleImage.h
settings.h
MarchingCubesValue.h
ITKRegistration.h
VOI.h
MyInteractorStyle.h
PolydataInteractorStyle.h
TransformData.h
DataDimension.h
ProgressBarGUI.h

tomoregistration.ui
SettingsGUI.ui
DataProcessGUI.ui
MarchingCubesValue.ui
TransformGUI.ui
DataDimension.ui
ProgressBarGUI.ui
)



SET(TomoPROJECT_MOC_HDRS tomoregistration.h dataProcess.h landmark.h myVtkInteractorStyleImage.h settings.h MarchingCubesValue.h ITKRegistration.h VOI.h MyInteractorStyle.h PolydataInteractorStyle.h TransformData.h DataDimension.h ProgressBarGUI.h)
SET(TomoPROJECT_UIS tomoregistration.ui SettingsGUI.ui DataProcessGUI.ui MarchingCubesValue.ui TransformGUI.ui DataDimension.ui ProgressBarGUI.ui)
SET(TomoPROJECT_RCS imageregistration.qrc)

QT4_ADD_RESOURCES(TomoPROJECT_RC_SRCS ${TomoPROJECT_RCS})
QT4_WRAP_UI(TomoPROJECT_UI_HDRS ${TomoPROJECT_UIS})
QT4_WRAP_CPP(TomoPROJECT_MOC_SRCS ${TomoPROJECT_MOC_HDRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
  
ADD_EXECUTABLE(TomoRegistration ${TomoPROJECT_SRCS} ${TomoPROJECT_MOC_HDRS} ${TomoPROJECT_MOC_SRCS} ${TomoPROJECT_RC_SRCS} ${TomoPROJECT_UI_HDRS})

TARGET_LINK_LIBRARIES(TomoRegistration ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(TomoRegistration ${VTK_LIBRARIES} QVTK)
TARGET_LINK_LIBRARIES(TomoRegistration ${ITK_LIBRARIES})