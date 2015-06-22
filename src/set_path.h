#ifndef SET_PATH_H
#define SET_PATH_H

#include <QDialog>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

class Set_Path : public QDialog {
	Q_OBJECT
private:
	QLineEdit *path_tools;
	QLineEdit *path_trans;
	QLineEdit *path_cgen;
	QLineEdit *path_inter;
	QLineEdit *path_rig2dot;
	QLineEdit *path_cg2dot;
	QLineEdit *path_dot2jpeg;
	QLineEdit *path_project;
	QLineEdit *path_temp;
	QLineEdit *path_repository;
private slots:
	void setPathTools();
	void setPathTrans();
	void setPathCgen();
	void setPathInter();
	void setPathRig2dot();
	void setPathCg2dot();
	void setPathDot2jpeg();
	void setPathProject();
	void setPathTemp();
	void setPathRepository();
public:
	Set_Path(QWidget* pwgt = 0);
	QString getPathTools() const;
	QString getPathTrans() const;
	QString getPathCgen() const;
	QString getPathInter() const;
	QString getPathRig2dot() const;
	QString getPathCg2dot() const;
	QString getPathDot2jpeg() const;
	QString getPathProject () const;
	QString getPathTemp() const;
	QString getPathRepository() const;
	~Set_Path();
};

#endif // SET_PATH_H