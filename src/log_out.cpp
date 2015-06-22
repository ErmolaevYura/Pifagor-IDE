#include <QtWidgets>

#include "log_out.h"

Log_Out::Log_Out(QWidget *parent) : QTextEdit(parent)
{
	setReadOnly(true);
}

Log_Out::~Log_Out() { }
