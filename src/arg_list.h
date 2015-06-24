#ifndef ARG_LIST_H
#define ARG_LIST_H

#include <QTextEdit>
#include <QString>
#include <QProcess>
#include "general.h"

class Arg_List : public QTextEdit
{
	Q_OBJECT
public:
	Arg_List(QWidget *parent = 0);
	bool loadFile(const QString &fileName);
	~Arg_List();
public slots:
	void argBuild();
};

#endif // ARG_LIST_H
