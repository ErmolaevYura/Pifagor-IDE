#include <QApplication>

#include "pif_ide.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	Pif_IDE w;
	w.show();
	return app.exec();
}
