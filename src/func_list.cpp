#include <QtWidgets>

#include "func_list.h"

Func_List::Func_List(QWidget *parent) : QListWidget(parent){}
QString Func_List::clearComment(QString str)
{
	int k = 0;
	for(int i = 0; i < str.length(); i++)
		if(str[i] == '/')
		{
			if(i + 1 < str.length() && str[++i] == '*')
			{
				k = i - 1;
				while(i < str.length())
				{
					i++;
					if(str[i] == '*')
						if(i + 1 < str.length() && str[++i] == '/')
							break;
				}
				i++;
				if(str[i+1] == '\n')
					i++;
				str.remove(k, i-k);
				i = k;
			}
			else if(i + 1 < str.length() && str[i] == '/')
			{
				k = i - 1;
				while(i < str.length() && str[i] != '\n')
					i++;
				i++;
				str.remove(k, i-k);
				i = k-1;
			}
		}
		return str;
}
QStringList Func_List::createList(QString str)
{
	QStringList strList = clearComment(str).split("\n"), funcList;
	for(int i = 0; i < strList.count(); i++) {
		if(!strList[i].contains("funcdef"))
			strList.removeAt(i--);
	}
	for(int i = 0; i < strList.count(); i++) {
		funcList<<strList[i].left(strList[i].indexOf("<<")).replace(" ", "\0").replace("\t", "\0");
		addItem(funcList[i]);
	}
	return funcList;
}

Func_List::~Func_List() { }
