#include "settings.h"

settings::settings()
{
	widget.setupUi(this);
	//Read the avalaible settings
	this->ReadSettings();

	//surface registration
	if(widget.radioButtonLandmark->isChecked()){
		widget.groupBoxICP->hide();
	}

	connect(widget.radioButtonLandmark,SIGNAL(clicked()),this,SLOT(landmarkBox()));
	connect(widget.radioButtonICP,SIGNAL(clicked()),this,SLOT(landmarkBox()));
	connect(widget.radioButtonLandmarkICP,SIGNAL(clicked()),this,SLOT(landmarkBox()));
}

settings::~settings()
{

}

void settings::ReadSettings()
{
	/********************* settings for surface registration ***************************/
	QSettings settings("_settings.ini", QSettings::IniFormat);
	settings.beginGroup("Profile/Profile-Profile/PET");
	widget.ProfileTomoRaw->setChecked(settings.value("ProfileTomoRaw").toBool());
	widget.ProfileTomoDicom->setChecked(settings.value("ProfileTomoDicom").toBool());
	widget.radioButtonLandmark->setChecked(settings.value("radioButtonLandmark").toBool());
	widget.radioButtonICP->setChecked(settings.value("radioButtonICP").toBool());
	widget.radioButtonLandmarkICP->setChecked(settings.value("radioButtonLandmarkICP").toBool());
	
	widget.checkBoxMatchingCentroid->setChecked(settings.value("checkBoxMatchingCentroid").toBool());
	widget.spinBoxNumberOfIteration->setValue(settings.value("spinBoxNumberOfIteration").toInt());
	widget.radioButtonRigid->setChecked(settings.value("radioButtonRigid").toBool());
	widget.radioButtonSimilarity->setChecked(settings.value("radioButtonSimilarity").toBool());
	widget.radioButtonAffine->setChecked(settings.value("radioButtonAffine").toBool());
	settings.endGroup();
	/********************** settings for fiducial markers registration*********************/
	settings.beginGroup("Fiducial_Landmark");
	widget.sourceRawFile->setChecked(settings.value("sourceRawFile").toBool());
	widget.targetRawFile->setChecked(settings.value("targetRawFile").toBool());
	widget.FiducialMarkers_ICP->setChecked(settings.value("FiducialMarkers_ICP").toBool());
	widget.FiducialMarkers_landmark->setChecked(settings.value("FiducialMarkers_landmark").toBool());
	widget.FiducialMarkers_Paul_Viola->setChecked(settings.value("FiducialMarkers_Paul_Viola").toBool());

	settings.endGroup();
}

void settings::WriteSettings()
{
	/*********************For surface registration***************************/
	QSettings* settings = new QSettings("_settings.ini",QSettings::IniFormat);
	settings->beginGroup("Profile/Profile-Profile/PET");
	settings->setValue("ProfileTomoRaw",widget.ProfileTomoRaw->isChecked());
	settings->setValue("ProfileTomoDicom",widget.ProfileTomoDicom->isChecked());
	settings->setValue("radioButtonLandmark",widget.radioButtonLandmark->isChecked());
	settings->setValue("radioButtonICP",widget.radioButtonICP->isChecked());
	settings->setValue("radioButtonLandmarkICP",widget.radioButtonLandmarkICP->isChecked());
	settings->setValue("checkBoxMatchingCentroid",widget.checkBoxMatchingCentroid->isChecked());
	settings->setValue("spinBoxNumberOfIteration",widget.spinBoxNumberOfIteration->value());
	settings->setValue("radioButtonRigid",widget.radioButtonRigid->isChecked());
	settings->setValue("radioButtonSimilarity",widget.radioButtonSimilarity->isChecked());
	settings->setValue("radioButtonAffine",widget.radioButtonAffine->isChecked());
    settings->endGroup();
	/****************for fiducial markers registration*********************/
	settings->beginGroup("Fiducial_Landmark");
	settings->setValue("sourceRawFile",widget.sourceRawFile->isChecked());
	settings->setValue("targetRawFile",widget.targetRawFile->isChecked());
	settings->setValue("FiducialMarkers_ICP",widget.FiducialMarkers_ICP->isChecked());
	settings->setValue("FiducialMarkers_landmark",widget.FiducialMarkers_landmark->isChecked());
	settings->setValue("FiducialMarkers_Paul_Viola",widget.FiducialMarkers_Paul_Viola->isChecked());

	settings->endGroup();

}

void settings::landmarkBox()
{
	if(widget.radioButtonLandmark->isChecked()){
		this->widget.groupBoxICP->hide();
	}else{
		this->widget.groupBoxICP->show();
	}
}

