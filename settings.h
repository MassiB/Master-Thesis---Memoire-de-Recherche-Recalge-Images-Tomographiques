#ifndef SETTINGS_H
#define	SETTINGS_H

#include "ui_SettingsGUI.h"
#include "tomoregistration.h"
#include "qsettings.h"
#include "DataDimension.h"

class settings : public QDialog, Ui::Settings
{
	Q_OBJECT;
public:
	settings();
	~settings();
	void ReadSettings();
public slots:
	void landmarkBox();
	void WriteSettings();

private:
	Ui::Settings widget;
};

#endif