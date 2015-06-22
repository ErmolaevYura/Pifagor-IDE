#include <QtWidgets>

#include "arg_list.h"
#include "general.h"

Arg_List::Arg_List(QWidget *parent) : QTextEdit(parent) 
{
	setAcceptRichText(false);
}
void Arg_List::argBuild()
{
	if(toPlainText() == "")
		return;
	QString argument = toPlainText();
	QString fileName = stdValue.pathTemp + "/arg.txt", cmd;
	QFile file(fileName);
	if (file.open(QFile::WriteOnly | QFile::Truncate)) {
		QTextStream out(&file);
		out<<argument;
	}
	file.close();
	if(stdValue.pathTrans == NULL)
		cmd = "\"" + stdValue.pathTools + "/trans2.exe\" -c \"" + fileName + "\" err.log dbg.log";
	else
		cmd = "\"" + stdValue.pathTrans + "/trans2.exe\" -c \"" + fileName + "\" err.log dbg.log";
	QFileInfo FI(fileName);
	if(!FI.exists())
	{
        QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName,QMessageBox::Yes);
		return;
	}
	QProcess *Process = new QProcess(this);
	Process->start(cmd);
	if(!Process->waitForFinished() )
        QMessageBox::warning(this,tr("Error"),Process->errorString() + "; " + cmd,QMessageBox::Yes);
}
bool Arg_List::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Pifagor"), 
			tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return false;
	}
	QTextStream ifst(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	setPlainText(ifst.readAll());
	QApplication::restoreOverrideCursor();
	return true;
}
Arg_List::~Arg_List() { }
