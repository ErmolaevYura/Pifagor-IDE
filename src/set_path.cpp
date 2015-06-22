#include <QtGui>

#include "set_path.h"
#include "general.h"

Set_Path::Set_Path(QWidget *pwgt) 
	: QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{   
	path_tools = new QLineEdit;
	path_trans = new QLineEdit;
	path_cgen = new QLineEdit;
	path_inter = new QLineEdit;
	path_rig2dot = new QLineEdit;
	path_cg2dot = new QLineEdit;
	path_dot2jpeg = new QLineEdit;
	path_project = new QLineEdit;
	path_temp = new QLineEdit;
	path_repository = new QLineEdit;

	path_tools->setText(stdValue.pathTools);
	path_trans->setText(stdValue.pathTrans);
	path_cgen->setText(stdValue.pathCgen);
	path_inter->setText(stdValue.pathInter);
	path_rig2dot->setText(stdValue.pathRig2dot);
	path_cg2dot->setText(stdValue.pathCg2dot);
	path_dot2jpeg->setText(stdValue.pathDot2jpeg);
	path_project->setText(stdValue.pathProject);
	path_repository->setText(stdValue.pathRepository);
	path_temp->setText(stdValue.pathTemp);

	QPushButton *bpath_tools = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_trans = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_cgen = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_inter = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_rig2dot = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_cg2dot = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_dot2jpeg = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_project = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_temp = new QPushButton(QIcon("./images/open.png"), "");
	QPushButton *bpath_repository = new QPushButton(QIcon("./images/open.png"), "");
	connect(bpath_tools, SIGNAL(clicked()), SLOT(setPathTools()));
	connect(bpath_trans, SIGNAL(clicked()), SLOT(setPathTrans()));
	connect(bpath_cgen, SIGNAL(clicked()), SLOT(setPathCgen()));
	connect(bpath_inter, SIGNAL(clicked()), SLOT(setPathInter()));
	connect(bpath_rig2dot, SIGNAL(clicked()), SLOT(setPathRig2dot()));
	connect(bpath_cg2dot, SIGNAL(clicked()), SLOT(setPathCg2dot()));
	connect(bpath_dot2jpeg, SIGNAL(clicked()), SLOT(setPathDot2jpeg()));
	connect(bpath_project, SIGNAL(clicked()), SLOT(setPathProject()));
	connect(bpath_temp, SIGNAL(clicked()), SLOT(setPathTemp()));
	connect(bpath_repository, SIGNAL(clicked()), SLOT(setPathRepository()));

	QLabel* lbpath_tools = new QLabel("Path Tools");
	QLabel* lbpath_trans = new QLabel("Path Translator");
	QLabel* lbpath_cgen = new QLabel("Path Generator control graph");
	QLabel* lbpath_inter = new QLabel("Path Interpreter");
	QLabel* lbpath_rig2dot = new QLabel("Path .rig to .dot utility");
	QLabel* lbpath_cg2dot = new QLabel("Path .cg to .dot utility");
	QLabel* lbpath_dot2jpeg = new QLabel("Path Graphviz utility(dot.exe)");
	QLabel* lbpath_project = new QLabel("Path Project");
	QLabel* lbpath_temp = new QLabel("Path Temp");
	QLabel* lbpath_repository = new QLabel("Path Repository");
	
	lbpath_tools->setBuddy(path_tools);
	lbpath_trans->setBuddy(path_trans);
	lbpath_cgen->setBuddy(path_cgen);
	lbpath_inter->setBuddy(path_inter);
	lbpath_rig2dot->setBuddy(path_rig2dot);
	lbpath_cg2dot->setBuddy(path_cg2dot);
	lbpath_dot2jpeg->setBuddy(path_dot2jpeg);
	lbpath_project->setBuddy(path_project);
	lbpath_temp->setBuddy(path_temp);
	lbpath_repository->setBuddy(path_repository);

	QPushButton* pcmdOk     = new QPushButton("&Ok");
	QPushButton* pcmdCancel = new QPushButton("&Cancel");

	connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
	connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

	QGridLayout* topLayout = new QGridLayout;
	topLayout->addWidget(lbpath_tools, 0, 0);
	topLayout->addWidget(lbpath_trans, 1, 0);
	topLayout->addWidget(lbpath_cgen, 2, 0);
	topLayout->addWidget(lbpath_inter, 3, 0);
	topLayout->addWidget(lbpath_rig2dot, 4, 0);
	topLayout->addWidget(lbpath_cg2dot, 5, 0);
	topLayout->addWidget(lbpath_dot2jpeg, 6, 0);
	topLayout->addWidget(lbpath_project,7, 0);
	topLayout->addWidget(lbpath_temp, 8, 0);
	topLayout->addWidget(lbpath_repository, 9, 0);
	topLayout->addWidget(path_tools, 0, 1);
	topLayout->addWidget(path_trans, 1, 1);
	topLayout->addWidget(path_cgen, 2, 1);
	topLayout->addWidget(path_inter, 3, 1);
	topLayout->addWidget(path_rig2dot, 4, 1);
	topLayout->addWidget(path_cg2dot, 5, 1);
	topLayout->addWidget(path_dot2jpeg, 6, 1);
	topLayout->addWidget(path_project, 7, 1);
	topLayout->addWidget(path_temp, 8, 1);
	topLayout->addWidget(path_repository, 9, 1);
	topLayout->addWidget(bpath_tools, 0, 2);
	topLayout->addWidget(bpath_trans, 1, 2);
	topLayout->addWidget(bpath_cgen, 2, 2);
	topLayout->addWidget(bpath_inter, 3, 2);
	topLayout->addWidget(bpath_rig2dot, 4, 2);
	topLayout->addWidget(bpath_cg2dot, 5, 2);
	topLayout->addWidget(bpath_dot2jpeg, 6, 2);
	topLayout->addWidget(bpath_project, 7, 2);
	topLayout->addWidget(bpath_temp, 8, 2);
	topLayout->addWidget(bpath_repository, 9, 2);
	topLayout->addWidget(pcmdOk, 10,0);
	topLayout->addWidget(pcmdCancel, 10, 2);
	setLayout(topLayout);
}
QString Set_Path::getPathTools() const
{
	return path_tools->text();
}
QString Set_Path::getPathTrans() const
{
	return path_trans->text();
}
QString Set_Path::getPathCgen() const
{
	return path_cgen->text();
}
QString Set_Path::getPathInter() const
{
	return path_inter->text();
}
QString Set_Path::getPathRig2dot() const
{
	return path_rig2dot->text();
}
QString Set_Path::getPathCg2dot() const
{
	return path_cg2dot->text();
}
QString Set_Path::getPathDot2jpeg() const
{
	return path_dot2jpeg->text();
}
QString Set_Path::getPathProject() const
{
	return path_project->text();
}
QString Set_Path::getPathTemp() const
{
	return path_temp->text();
}
QString Set_Path::getPathRepository() const
{
	return path_repository->text();
}
void Set_Path::setPathTools()
{
	QString path = QFileDialog::getExistingDirectory(this, "Path Tools", path_tools->text());
	if(path != NULL)
	{
		path_tools->setText(path);
		stdValue.pathTools = path;
	}
}
void Set_Path::setPathTrans()
{
	QString path = QFileDialog::getOpenFileName(this, "Path Translator", path_tools->text(), ".exe");
	if(path != NULL)
	{
		path_trans->setText(path);
		stdValue.pathTrans = path;
	}
}
void Set_Path::setPathCgen()
{
	QString path = QFileDialog::getOpenFileName(this, "Path Generator control graph", path_tools->text(), ".exe");
	if(path != NULL)
	{
		path_cgen->setText(path);
		stdValue.pathCgen = path;
	}
}
void Set_Path::setPathInter()
{
	QString path = QFileDialog::getOpenFileName(this, "Path Interpreter", path_tools->text(), ".exe");
	if(path != NULL)
	{
		path_inter->setText(path);
		stdValue.pathInter = path;
	}
}
void Set_Path::setPathRig2dot()
{
	QString path = QFileDialog::getOpenFileName(this, "Path .rig to .dot utility", path_tools->text(), ".exe");
	if(path != NULL)
	{
		path_rig2dot->setText(path);
		stdValue.pathRig2dot = path;
	}
}
void Set_Path::setPathCg2dot()
{
	QString path = QFileDialog::getOpenFileName(this, "Path .cg to .dot utility", path_tools->text(), ".exe");
	if(path != NULL)
	{
		path_cg2dot->setText(path);
		stdValue.pathCg2dot = path;
	}
}
void Set_Path::setPathDot2jpeg()
{
	QString path = QFileDialog::getOpenFileName(this, "Path Path Graphviz utility(dot.exe)", path_tools->text(), ".exe");
	if(path != NULL)
	{
		path_dot2jpeg->setText(path);
		stdValue.pathDot2jpeg = path;
	}
}
void Set_Path::setPathProject()
{
	QString path = QFileDialog::getExistingDirectory(this, "Path Project", path_project->text());
	if(path != NULL)
	{
		path_project->setText(path);
		stdValue.pathProject = path;
	}
}
void Set_Path::setPathTemp()
{
	QString path = QFileDialog::getExistingDirectory(this, "Path Temp", path_temp->text());
	if(path != NULL)
	{
		path_temp->setText(path);
		stdValue.pathTemp = path;
	}
}
void Set_Path::setPathRepository()
{
	QString path = QFileDialog::getExistingDirectory(this, "Path Repository", path_repository->text());
	if(path != NULL)
	{
		path_repository->setText(path);
		stdValue.pathRepository = path;
	}
}
Set_Path::~Set_Path(){}