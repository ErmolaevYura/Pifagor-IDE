#ifndef PIF_IDE_H
#define PIF_IDE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QCloseEvent>
#include <QProcess>
#include <QDockWidget>
#include <QTextStream>
#include <QFontDialog>
#include <QTextCodec>
#include "ui_pif_ide.h"
#include "text_edit.h"
#include "log_out.h"
#include "res_out.h"
#include "console.h"
#include "arg_list.h"
#include "func_list.h"
#include "set_path.h"
#include "rep_list.h"
#include "show_graph.h"
#include "general.h"

class Pif_IDE : public QMainWindow
{
	Q_OBJECT
public:
	Pif_IDE(QWidget *parent = 0);
	~Pif_IDE();
	bool openFile(const QString &fileName);
protected:
	virtual void closeEvent(QCloseEvent *event);
private slots:
//Ìåíþ->File
	void createFile();
	void openFile();
	bool saveFile();
	void saveAsFile();
	void closeFile();
//Ìåíþ->Edit
	void cut();
	void copy();
	void paste();
//Ìåíþ->Setting
	void fontSetting();
	void pathSetting();
//Ìåíþ->Build
	void build();
	void stopBuild();
	void run();
	void stopRun();
	//void debugRun();
	//void debugStop();
	void jpegRIGFunc();
	void dotToPNGRIG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void outRIG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void jpegCGFunc();
	void dotToPNGCG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void outCG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void stopGraphviz();
	void loadArgFile();
//Ìåíþ->Help
	void about();
	void aboutQt();
//äðóãîå
	void updateMenus();
	void updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void updateWindowMenu();
	void buildReady();
	void cgenReady();
	void resultReady();
	void jpegRIGFuncToRep();
	void dotToPNGRIGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void outRIGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void jpegCGFuncToRep();
	void dotToPNGCGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void outCGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/);
	void openFuncToRep();
	void openFuncToRep(QTreeWidgetItem *item, int column);
    void createRepFile();
	Text_Edit *createText_Edit();
	void switchLayoutDirection();
	void setActiveSubWindow(QWidget *window);
private:
	Ui::Pif_IDEClass ui;
	Log_Out *log_Out;
	Res_Out *res_Out;
	Console *console;
	Arg_List *argList;
	Func_List *funcList;
	Set_Path *setPath;
	Rep_List *repList;
	Show_Graph *showRIGGraph;
	Show_Graph *showCGGraph;
	QMdiArea *mdiArea;
	QSignalMapper *windowMapper;
	QProcess *Process;
	QStringList functionList;
	QStringList repositoryList;
	QTextCodec *decoder;
	QFont *font;
	QProcess *builProcess;
	QList<QProcess*> *cgenListProcess;
	QProcess *runProcess;
	QProcess *rigToDotProcess;
	QProcess *cgToDotProcess;
	QProcess *dotToPNGRIGProcess;
	QProcess *dotToPNGCGProcess;
	QProcess *rigToDotRepProcess;
	QProcess *dotToPNGRIGRepProcess;
	QProcess *cgToDotRepProcess;
	QProcess *dotToPNGCGRepProcess;
	void cgenRun();
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBars();
	void createDockWidget();
	void createFuncList();
	void createRepList();
	void refreshFuncListW();
	void readSettings();
	void writeSettings();
	QString* parsRepFunc(QTreeWidgetItem *item, QString *str);
	Text_Edit* activeText_Edit();
	QMdiSubWindow* findText_Edit(const QString &fileName);
	QMdiSubWindow* findNewText_Edit(const QString &fileName);

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *settingMenu;
	QMenu *buildMenu;
	QMenu *windowMenu;
	QMenu *helpMenu;

	QDockWidget *logOutDock;
	QDockWidget *resOutDock;
	QDockWidget *consoleDock;
	QDockWidget *argDock;
	QDockWidget *funcDock;
	QDockWidget *repDock;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *buildToolBar;

	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *closeAction;
	QAction *exitAction;

	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;

	QAction *fontSetupAction;
	QAction *pathSetupAction;

	QAction *buildAction;
	QAction *stopBuildAction;
	QAction *runAction;
	QAction *stopRunAction;
	QAction *jpegRIGFuncAction;
	QAction *jpegCGFuncAction;
	QAction *stopGraphvizAction;
	//QAction *debugRunAction;
	//QAction *debugStopAction;
	QAction *loadArgFileAction;

	QAction *tileAction;
	QAction *cascadeAction;

	QAction *aboutAction;
	QAction *aboutQtAction;
};

#endif // PIF_IDE_H
