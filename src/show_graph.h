#ifndef SHOW_GRAPH_H
#define SHOW_GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

class Show_Graph : public QWidget {
	Q_OBJECT
public:
	Show_Graph(QWidget *parent = 0);
	~Show_Graph();
	void showGraph(QString filename);
//protected:
//	void paintEvent(QPaintEvent *);
private:
	QVBoxLayout *vBox;
	QScrollArea *scroolArea;
	QLabel *labelImage;
};

#endif // SHOW_GRAPH_H