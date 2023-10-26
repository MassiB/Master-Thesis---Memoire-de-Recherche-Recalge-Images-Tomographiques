#include "tomoregistration.h"
#include <QtGui/QApplication>


int main(int argc, char *argv[])
{
	//TomoRegistration w;
	QApplication a(argc, argv);
	TomoRegistration w;
	a.setStyle("Cleanlooks");
	w.show();
	return a.exec();
}
