#include "show_graph.h"

Show_Graph::Show_Graph(QWidget *parent)
{
	vBox = new QVBoxLayout;
	labelImage = new QLabel(this);
	scroolArea = new QScrollArea();
}
void Show_Graph::showGraph(QString filename)
{
	this->setWindowFlags(this->windowFlags() & Qt::WindowStaysOnTopHint);
	QPixmap *pixmap = new QPixmap(filename);
    vBox->removeWidget(scroolArea);
	vBox->addWidget(scroolArea);
	labelImage->setPixmap(*pixmap);
	labelImage->resize(pixmap->size());
	scroolArea->resize(pixmap->size());
	scroolArea->setWidget(labelImage);
	this->setLayout(vBox);
	this->resize(640, 480);
	this->show();
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
}
Show_Graph::~Show_Graph(){}
