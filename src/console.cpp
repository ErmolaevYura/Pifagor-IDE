#include <QtWidgets>

#include "console.h"

Console::Console(QWidget *parent) : QTextEdit(parent)
{
	setAcceptRichText(false);
	Process = new QProcess(this);
	decoder = QTextCodec::codecForName("IBM866");
	encoder = QTextCodec::codecForName("WINDOWS-1251");
	connect(Process, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(terminalReady(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
}
void Console::terminalReady(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	if(Process->state() == QProcess::CrashExit)
		QMessageBox::warning(this,tr("Error"),Process->errorString() + "; " + cmd, QMessageBox::Yes);
	else {
		QString newStr = toPlainText().left(toPlainText().indexOf(cmd));
		append(newStr);
		QByteArray BA = Process->readAll();
		append(decoder->toUnicode(BA));
	}
}
void Console::keyPressEvent(QKeyEvent *e)
{
	if(Process->state() != QProcess::NotRunning)
		Process->kill();
    QString curText = toPlainText();
	QStringList strList = curText.split('\n');
	int line_Count = document()->lineCount();
	switch(e->key())
	{
	case Qt::Key_Enter:
		cmd = strList.at(line_Count-1);
		Process->start(encoder->fromUnicode(cmd));
		break;
	default:
		QTextEdit::keyPressEvent(e);
	}
}
Console::~Console() { }
