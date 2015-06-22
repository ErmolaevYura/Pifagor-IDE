#include "general.h"

StdValue::StdValue()
{
	pathTools = "./";
	pathTrans = "";
	pathCgen = "";
	pathInter = "";
	pathRig2dot = "";
	pathCg2dot = "";
	pathDot2jpeg = "./dot/dot.exe";
	pathProject = "./project";
	pathRepository = "./MainRepository";
	pathTemp = "./temp";
	readSettings();
}
void StdValue::writeSettings()
{
	QSettings settings("QTProject", "Pifagor IDE 0.1");
	settings.beginGroup("Pifagor IDE Path");
	settings.setValue("tools", pathTools);
	settings.setValue("trans", pathTrans);
	settings.setValue("cgen", pathCgen);
	settings.setValue("inter", pathInter);
	settings.setValue("rig2dot", pathRig2dot);
	settings.setValue("cg2dot", pathCg2dot);
	settings.setValue("dot2jpeg", pathDot2jpeg);
	settings.setValue("project", pathProject);
	settings.setValue("repository", pathRepository);
	settings.setValue("temp", pathTemp);
	settings.endGroup();
}
void StdValue::readSettings()
{
	QSettings settings("QtProject", "Pifagor IDE 0.1");
	settings.beginGroup("Pifagor IDE Path");
	QString str = settings.value("tools").toString();
	if(str != NULL)
		pathTools = str;
	str = settings.value("trans").toString();
	if(str != NULL)
		pathTrans = str;
	str = settings.value("cgen").toString();
	if(str != NULL)
		pathCgen = str;
	str = settings.value("inter").toString();
	if(str != NULL)
		pathInter = str;
	str = settings.value("rig2dot").toString();
	if(str != NULL)
		pathRig2dot = str;
	str = settings.value("cg2dot").toString();
	if(str != NULL)
		pathCg2dot = str;
	str = settings.value("dot2jpeg").toString();
	if(str != NULL)
		pathDot2jpeg = str;
	str = settings.value("project").toString();
	if(str != NULL)
		pathProject = str;
	str = settings.value("repository").toString();
	if(str != NULL)
		pathRepository = str;
	str = settings.value("temp").toString();
	if(str != NULL)
		pathTemp = str;
	settings.endGroup();
}