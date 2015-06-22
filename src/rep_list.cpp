#include <QtWidgets>

#include "rep_list.h"
#include "general.h"

Rep_List::Rep_List(QWidget *parent) : QTreeWidget(parent)
{
	setColumnCount(1);
	setHeaderLabel("Repository");

	showAction = new QAction(QIcon("./images/file.png"), tr("Open File"), this);
	showAction->setStatusTip(tr("Open original file"));

    newFileAction = new QAction(QIcon("./images/new.png"), tr("Open as new file"), this);
    newFileAction->setStatusTip(tr("Open as new file"));

	showRIGjpg = new QAction(QIcon("./images/graf.png"), tr("Open RIG"), this);
	showRIGjpg->setStatusTip(tr("Open RIG.jpg"));
	
	showCGjpg = new QAction(QIcon("./images/graf.png"), tr("Open CG"), this);
	showCGjpg->setStatusTip(tr("Open CG.jpg"));
}
void Rep_List::createList()
{
	clear();
	QDir dir;
	dir.setFilter(QDir::AllDirs | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);
	dir.setPath(stdValue.pathRepository);
	QFileInfoList list = dir.entryInfoList();
	QStringList strList;
    QString bufStr;
    QStringList bufList;
	for(int i = 0; i < list.count(); i++)
		if(list.at(i).fileName() != "." && list.at(i).fileName() != "..")
			strList.push_back(list.at(i).fileName());
	for (int i = 0; i < strList.count(); i++)
	{
		if(strList[i].contains(".")) {
			bufList = strList[i].split(".");
			AddGroup(NULL, bufList);
		}
		else {
			bufStr = strList[i];
			AddFunc(NULL, bufStr);
		}
	}
}
void Rep_List::AddGroup(QTreeWidgetItem *parent, QStringList strList)
{
	QString strBuf = strList[0];
	QList<QTreeWidgetItem *> found = findItems(strBuf, Qt::MatchRecursive);
    QTreeWidgetItem *item;
	bool exist = false;
	if(found.count() == 0) {
		if(parent == NULL)
			addTopLevelItem(item = new QTreeWidgetItem());
		else 
			item = new QTreeWidgetItem(parent);
		item->setText(0, strBuf);
	}
	else
	{
		for(int i = 0; i < found.size(); i++)
		{
			if((parent == NULL && strBuf == found[i]->text(0)) || (parent == found[i]->parent()))
			{
				item = found[i];
				exist = true;
				break;
			}
		}
		if(!exist)
		{
			item = new QTreeWidgetItem(parent);
			item->setText(0, strBuf);
		}
	}
	strList.pop_front();
	if(strList.count() > 1)
		AddGroup(item, strList);
	else
		AddFunc(item, strList[0]);
}
void Rep_List::AddFunc(QTreeWidgetItem *parent, QString str)
{
	QTreeWidgetItem *item;
	if(parent == NULL)
		addTopLevelItem(item = new QTreeWidgetItem());
	else
		item = new QTreeWidgetItem(parent);
	item->setText(0, str);
}
void Rep_List::contextMenuEvent(QContextMenuEvent * e)
{
	QMenu* menu = new QMenu(this);
	menu->addAction(showAction);
    menu->addAction(newFileAction);
	menu->addSeparator();
	menu->addAction(showRIGjpg);
	menu->addAction(showCGjpg);
	menu->exec(e->globalPos());
};
Rep_List::~Rep_List() { }
