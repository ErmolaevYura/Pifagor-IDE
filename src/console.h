#ifndef CONSOLE_H
#define CONSOLE_H

#include <QTextEdit>
#include <QKeyEvent>
#include <QString>
#include <QProcess>

class Console : public QTextEdit
{
	Q_OBJECT
public:
	Console(QWidget *parent = 0);
	~Console();
private slots:
	void terminalReady(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
protected:
	virtual void keyPressEvent(QKeyEvent *event);
private:
	QString cmd;
	QProcess *Process;
	QTextCodec *decoder, *encoder;
};

#endif // CONSOLE_H