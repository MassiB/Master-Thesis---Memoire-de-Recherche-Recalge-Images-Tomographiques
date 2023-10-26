#include "ProgressBarGUI.h"

ProgressBarGUI::ProgressBarGUI()
{
	this->widget.setupUi(this);
}

ProgressBarGUI::~ProgressBarGUI()
{

}

void ProgressBarGUI::SetProgressBarValue(int value)
{
	this->widget.progressBar->setValue(50);
}