#include <QtWidgets>

#include "res_out.h"

Res_Out::Res_Out(QWidget *parent) : QTextEdit(parent)
{
	setReadOnly(true);
}
void Res_Out::showResult(QString resStr)
{
	int indexStart = resStr.indexOf(':') + 1;
	QString str = resStr.right(resStr.length() - indexStart);
	int indexEnd = str.left(str.lastIndexOf('\n')).lastIndexOf('\n');
	str = str.left(indexEnd);
	append(str);
}
Res_Out::~Res_Out() { }
