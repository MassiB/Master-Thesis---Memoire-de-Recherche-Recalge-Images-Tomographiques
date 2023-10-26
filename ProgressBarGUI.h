#ifndef PROGRESSBARGUI_H
#define PROGRESSBARGUI_H

#include "ui_ProgressBarGUI.h"

class ProgressBarGUI : public QDialog
{
public:
	ProgressBarGUI();
	~ProgressBarGUI();

	void SetProgressBarValue(int);

private:
	Ui::ProgressBarDialog widget;
};

#endif