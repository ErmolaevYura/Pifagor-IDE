#ifndef RES_OUT_H
#define RES_OUT_H

#include <QTextEdit>

class Res_Out : public QTextEdit
{
	Q_OBJECT
public:
	Res_Out(QWidget *parent = 0);
	~Res_Out();
	void showResult(QString resStr);
};

#endif // RES_OUT_H