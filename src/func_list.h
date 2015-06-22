#ifndef FUNC_LIST_H
#define FUNC_LIST_H

#include <QListWidget>
#include <QListView>
#include <QString>

class Func_List : public QListWidget
{
	Q_OBJECT
public:
	Func_List(QWidget *parent = 0);
	QStringList createList(QString str);
	~Func_List();
protected:
private:
	QString clearComment(QString str);
};

#endif // FUNC_LIST_H