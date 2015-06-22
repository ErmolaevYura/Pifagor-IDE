#ifndef GENERAL_H
#define GENERAL_H

#include <QString>
#include <QSettings>

struct StdValue
{
	QString pathTools;
	QString pathTrans;
	QString pathCgen;
	QString pathInter;
	QString pathRig2dot;
	QString pathCg2dot;
	QString pathDot2jpeg;
	QString pathProject;
	QString pathRepository;
	QString pathTemp;
	StdValue();
	void readSettings();
	void writeSettings();
} static stdValue;

#endif // GENERAL_H