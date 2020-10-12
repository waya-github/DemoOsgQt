#include "DemoOsgQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DemoOsgQt w;
	w.show();
	return a.exec();
}
