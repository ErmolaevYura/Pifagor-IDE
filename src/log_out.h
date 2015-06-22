#ifndef LOG_OUT_H
#define LOG_OUT_H

#include <QTextEdit>

class Log_Out : public QTextEdit
{
	Q_OBJECT
public:
	Log_Out(QWidget *parent = 0);
	~Log_Out();
};

#endif // LOG_OUT_H