#ifndef REP_LIST_H
#define REP_LIST_H

#include <QTreeWidget>
#include <QListView>
#include <QDir>
#include <QString>

class Rep_List : public QTreeWidget
{
	Q_OBJECT
public:
	Rep_List(QWidget *parent = 0);
	void createList();
	~Rep_List();
private:
	void AddGroup(QTreeWidgetItem *parent, QStringList strList);
	void AddFunc(QTreeWidgetItem *parent, QString str);
	virtual void contextMenuEvent(QContextMenuEvent * e);
public:
	QAction *showAction;
    QAction *newFileAction;
	QAction *showRIGjpg;
	QAction *showCGjpg;
};

#endif // REP_LIST_H
