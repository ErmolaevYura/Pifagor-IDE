#include "pif_ide.h"

Pif_IDE::Pif_IDE(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	log_Out = new Log_Out;
	res_Out = new Res_Out;
	console = new Console;
	argList = new Arg_List;
	funcList = new Func_List;
	showRIGGraph = new Show_Graph;
	showCGGraph = new Show_Graph;
	setPath = new Set_Path;
	repList = new Rep_List;
	mdiArea = new QMdiArea;
	builProcess = new QProcess(this);
	cgenListProcess = new QList<QProcess*>;
	runProcess = new QProcess(this);
	rigToDotProcess = new QProcess(this);
	dotToPNGRIGProcess = new QProcess(this);
	cgToDotProcess = new QProcess(this);
	dotToPNGCGProcess = new QProcess(this);
	rigToDotRepProcess = new QProcess(this);
	dotToPNGRIGRepProcess = new QProcess(this);
	cgToDotRepProcess = new QProcess(this);
	dotToPNGCGRepProcess = new QProcess(this);
	setWindowIcon(QIcon("./images/icon.png"));
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
	connect(funcList, SIGNAL(itemSelectionChanged()), this, SLOT(updateMenus()));
	connect(repList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(openFuncToRep(QTreeWidgetItem*, int)));
	connect(repList->showAction, SIGNAL(triggered()), this, SLOT(openFuncToRep()));
    connect(repList->newFileAction, SIGNAL(triggered()), this, SLOT(createRepFile()));
	connect(repList->showRIGjpg, SIGNAL(triggered()), this, SLOT(jpegRIGFuncToRep()));
	connect(repList->showCGjpg, SIGNAL(triggered()), this, SLOT(jpegCGFuncToRep()));
	////
	connect(builProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(buildReady()));
	connect(builProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	///
	////
	connect(runProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(resultReady()));
	connect(runProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	////
	connect(rigToDotProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(dotToPNGRIG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	connect(rigToDotProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	//
	connect(dotToPNGRIGProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(outRIG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	connect(dotToPNGRIGProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	///
	connect(cgToDotProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(dotToPNGCG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	connect(cgToDotProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	//
	connect(dotToPNGCGProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(outCG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	connect(dotToPNGCGProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	////
	connect(rigToDotRepProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(dotToPNGRIGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	connect(rigToDotRepProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	//
	connect(dotToPNGRIGRepProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(outRIGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	connect(dotToPNGRIGRepProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	///
	connect(cgToDotRepProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(dotToPNGCGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	connect(cgToDotRepProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	//
	connect(dotToPNGCGRepProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(outCGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	connect(dotToPNGCGRepProcess, SIGNAL(finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)), this, SLOT(updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)));
	////
	windowMapper = new QSignalMapper(this);
	connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
	decoder = QTextCodec::codecForName("IBM866");
	createActions();
	createMenus();
	createToolBars();
	createStatusBars();
	createDockWidget();
	createRepList();
	updateMenus();
	readSettings();
	setWindowTitle(tr("Pifagor IDE 0.1"));
}
void Pif_IDE::closeEvent(QCloseEvent *event)
{
	mdiArea->closeAllSubWindows();
	if(mdiArea->currentSubWindow()) {
		event->ignore();
	} else {
		stdValue.writeSettings();
		writeSettings();
		event->accept();
	}
}
void Pif_IDE::createFile()
{
	Text_Edit *textedit = createText_Edit();
	textedit->newFile();
	QMdiSubWindow *file = findNewText_Edit(textedit->getCurrentFile());
	mdiArea->setActiveSubWindow(file);
	textedit->show();
}
void Pif_IDE::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open File", stdValue.pathProject, "*.pfg *.txt");
	if(!fileName.isEmpty()) {
		QMdiSubWindow *existing = findText_Edit(fileName);
		if(existing) {
			mdiArea->setActiveSubWindow(existing);
			return;
		}
		if(openFile(fileName))
			statusBar()->showMessage(tr("File loaded"), 2000);
	}
}
bool Pif_IDE::openFile(const QString &fileName)
{
	Text_Edit *textedit = createText_Edit();
	const bool succeeded = textedit->loadFile(fileName);
	if(succeeded)
	{
		QMdiSubWindow *file = findNewText_Edit(textedit->getCurrentFile());
		mdiArea->setActiveSubWindow(file);
		textedit->show();
	}
	else
		textedit->close();
	return succeeded;
}
void Pif_IDE::createRepFile()
{
    if(repList->currentItem() == NULL)
    {
        QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
        return;
	}
	Text_Edit *textedit = createText_Edit();
	textedit->newFile();
    QString functionName;
	parsRepFunc(repList->currentItem(), &functionName);
    QString fileName = stdValue.pathRepository + "/" + functionName + "/001/orig.txt";
	textedit->loadFile(fileName, functionName);
	mdiArea->setActiveSubWindow(mdiArea->currentSubWindow());
	textedit->show();
}
bool Pif_IDE::saveFile()
{
	if(activeText_Edit() && activeText_Edit()->save())
	{
		statusBar()->showMessage(tr("File Saved"), 2000);
		return true;
	}
	return false;
}
void Pif_IDE::saveAsFile()
{
	if(activeText_Edit() && activeText_Edit()->saveAs())
		statusBar()->showMessage(tr("File Saved"), 2000);
}
void Pif_IDE::cut()
{
	if(activeText_Edit())
		activeText_Edit()->cut();
}
void Pif_IDE::copy()
{
	if(activeText_Edit())
		activeText_Edit()->copy();
}
void Pif_IDE::paste()
{
	if(activeText_Edit())
		activeText_Edit()->paste();
}
void Pif_IDE::fontSetting()
{
    bool bÎê;
	QFont fnt = QFontDialog::getFont(&bÎê); 
	if(!bÎê) { 
		return;
	}
	if(fnt.pointSize() > 24)
		fnt.setPointSize(24);
	foreach (QMdiSubWindow *window, mdiArea->subWindowList())	{
		Text_Edit *testedit = qobject_cast<Text_Edit *>(window->widget());
		testedit->setFont(fnt);
	}
}
void Pif_IDE::about()
{
	QMessageBox::about(this, tr("About Pifagor IDE 1.0"), 
		tr("This is IDE for Pifagor"));
}
void Pif_IDE::aboutQt()
{
	QMessageBox::about(this, tr("About Qt"), 
		tr("In that project was used Qt 5.2.1."));
}
void Pif_IDE::updateMenus()
{
	updateMenus(0, QProcess::ExitStatus::NormalExit);
}
void Pif_IDE::updateMenus(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	bool hasTest_Edit = (activeText_Edit() != 0);
	saveAction->setEnabled(hasTest_Edit);
	saveAsAction->setEnabled(hasTest_Edit);
	closeAction->setEnabled(hasTest_Edit);
	pasteAction->setEnabled(hasTest_Edit);
	bool hasChoosFunc = funcList->currentItem() != NULL;
	buildAction->setEnabled(hasTest_Edit && builProcess->state() == QProcess::NotRunning);
	stopBuildAction->setEnabled(builProcess->state() != QProcess::NotRunning || cgenListProcess->size() != 0);
	runAction->setEnabled(hasChoosFunc && hasTest_Edit && runProcess->state() == QProcess::NotRunning);
	stopRunAction->setEnabled(runProcess->state() != QProcess::NotRunning);
	stopGraphvizAction->setEnabled(rigToDotProcess->state() != QProcess::NotRunning || dotToPNGRIGProcess->state() != QProcess::NotRunning 
		|| cgToDotProcess->state() != QProcess::NotRunning || dotToPNGCGProcess->state() != QProcess::NotRunning 
		|| rigToDotRepProcess->state() != QProcess::NotRunning || dotToPNGRIGRepProcess->state() != QProcess::NotRunning
		|| cgToDotRepProcess->state() != QProcess::NotRunning || dotToPNGCGRepProcess->state() != QProcess::NotRunning);
	//debugRunAction->setEnabled(hasChoosFunc && hasTest_Edit);
	//debugStopAction->setEnabled(hasChoosFunc && hasTest_Edit);
	jpegRIGFuncAction->setEnabled(hasChoosFunc && hasTest_Edit && rigToDotProcess->state() == QProcess::NotRunning && dotToPNGRIGProcess->state() == QProcess::NotRunning);
	jpegCGFuncAction->setEnabled(hasChoosFunc && hasTest_Edit && cgToDotProcess->state() == QProcess::NotRunning && dotToPNGCGProcess->state() == QProcess::NotRunning);
	bool hasSelection = (activeText_Edit() && activeText_Edit()->textCursor().hasSelection());
	cutAction->setEnabled(hasSelection);
	copyAction->setEnabled(hasSelection);
}
void Pif_IDE::updateWindowMenu()
{
	windowMenu->clear();
	windowMenu->addAction(tileAction);
	windowMenu->addAction(cascadeAction);
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	for (int i = 0; i < windows.size(); ++i) {
		Text_Edit *textedit = qobject_cast<Text_Edit *>(windows.at(i)->widget());
		QString text;
		if (i < 9) {
			text = tr("&%1 %2").arg(i + 1).arg(textedit->userFriendlyCurrentFile());
		} else {
			text = tr("%1 %2").arg(i + 1).arg(textedit->userFriendlyCurrentFile());
		}
		QAction *action  = windowMenu->addAction(text);
		action->setCheckable(true);
		action->setChecked(textedit == activeText_Edit());
		connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
		windowMapper->setMapping(action, windows.at(i));
	}
}
QString* Pif_IDE::parsRepFunc(QTreeWidgetItem *item, QString *str)
{
	if(item->parent() != NULL)
		*str = *parsRepFunc(item->parent(), str) + "." + item->text(0);
	else 
		*str += item->text(0);
	return str;
}
void Pif_IDE::openFuncToRep(QTreeWidgetItem *item, int column)
{
	QString str, fileName;
	parsRepFunc(item, &str);
	fileName = stdValue.pathRepository + "/" + str + "/001/orig.txt";
	QFileInfo FI(fileName);
	if(!FI.exists())
		return;
	if(openFile(fileName))
		statusBar()->showMessage(tr("File loaded"), 2000);
}
void Pif_IDE::openFuncToRep()
{
	openFuncToRep(repList->currentItem(), 0);
}
Text_Edit* Pif_IDE::createText_Edit()
{
	Text_Edit *textedit = new Text_Edit;
	mdiArea->addSubWindow(textedit);
	connect(textedit, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
	connect(textedit, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));
	return textedit;
}
void Pif_IDE::createActions()
{
	newAction = new QAction(QIcon("./images/new.png"), tr("&New"), this);
	newAction->setShortcuts(QKeySequence::New);
	newAction->setStatusTip(tr("Create a new file"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(createFile()));

	openAction = new QAction(QIcon("./images/open.png"), tr("&Open..."), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

	saveAction = new QAction(QIcon("./images/save.png"), tr("&Save"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save the document to disk"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	saveAsAction->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));
	
	closeAction = new QAction(tr("&Close"), this);
	closeAction->setShortcuts(QKeySequence::Close);
	closeAction->setStatusTip(tr("Close the active window"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(closeFile()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcuts(QKeySequence::Quit);
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
	//////////////////////////////////////////////////////////////////////////////
	cutAction = new QAction(QIcon("./images/cut.png"), tr("Cu&t"), this);
	cutAction->setShortcuts(QKeySequence::Cut);
	cutAction->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
	connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

	copyAction = new QAction(QIcon("./images/copy.png"), tr("&Copy"), this);
	copyAction->setShortcuts(QKeySequence::Copy);
	copyAction->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

	pasteAction = new QAction(QIcon("./images/paste.png"), tr("&Paste"), this);
	pasteAction->setShortcuts(QKeySequence::Paste);
	pasteAction->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
	connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
	//////////////////////////////////////////////////////////////////////////////////////////////
	fontSetupAction = new QAction(QIcon("./images/font.png"), tr("Font Setup"), this);
	fontSetupAction->setStatusTip(tr("Choose other font for listing file"));
	connect(fontSetupAction, SIGNAL(triggered()), this, SLOT(fontSetting()));

	pathSetupAction = new QAction(tr("Set Path"), this);
	pathSetupAction->setStatusTip(tr("Choos path for tools"));
	connect(pathSetupAction, SIGNAL(triggered()), this, SLOT(pathSetting()));
	/////////////////////////////////////////////////////////////////////////////////////
	buildAction = new QAction(QIcon("./images/build.png"), tr("Build"), this);
	buildAction->setShortcut(Qt::Key_F5);
	buildAction->setStatusTip(tr("Translation this programm"));
	connect(buildAction, SIGNAL(triggered()), this, SLOT(build()));
	connect(buildAction, SIGNAL(triggered()), argList, SLOT(argBuild()));

	stopBuildAction = new QAction(QIcon("./images/stopBuild.png"), tr("Stop Build"), this);
	stopBuildAction->setStatusTip(tr("Break to build this programm"));
	connect(stopBuildAction, SIGNAL(triggered()), this, SLOT(stopBuild()));
	
	runAction = new QAction(QIcon("./images/run.png"), tr("Run"), this);
	runAction->setShortcut(Qt::Key_F6);
	runAction->setStatusTip(tr("Start this programm"));
	connect(runAction, SIGNAL(triggered()), this, SLOT(run()));
	
	stopRunAction = new QAction(QIcon("./images/stopRun.png"), tr("Stop"), this);
	stopRunAction->setStatusTip(tr("Break to start this programm"));
	connect(stopRunAction, SIGNAL(triggered()), this, SLOT(stopRun()));
	
	/*
	debugRunAction = new QAction(QIcon("./images/debugrun.png"), tr("Debug"), this);
	debugRunAction->setStatusTip(tr("Debugging this programm"));
	connect(debugRunAction, SIGNAL(triggered()), this, SLOT(debugRun()));

	debugStopAction = new QAction(QIcon("./images/debugstop.png"), tr("Break to debug"), this);
	debugStopAction->setStatusTip(tr("Break to debugging this programm"));
	connect(debugStopAction, SIGNAL(triggered()), this, SLOT(debugStop()));
	*/
	loadArgFileAction = new QAction(QIcon("./images/loadarg.png"), tr("Load Arguments"), this);
	loadArgFileAction->setStatusTip(tr("Load file with arguments"));
	connect(loadArgFileAction, SIGNAL(triggered()), this, SLOT(loadArgFile()));

	jpegRIGFuncAction = new QAction(QIcon("./images/graph.png"), tr("Show RIG"), this);
	jpegRIGFuncAction->setStatusTip(tr("Show RIG"));
	connect(jpegRIGFuncAction, SIGNAL(triggered()), this, SLOT(jpegRIGFunc()));

	jpegCGFuncAction = new QAction(QIcon("./images/graph.png"), tr("Show CG"), this);
	jpegCGFuncAction->setStatusTip(tr("Show CG"));
	connect(jpegCGFuncAction, SIGNAL(triggered()), this, SLOT(jpegCGFunc()));

	stopGraphvizAction = new QAction(QIcon("./images/stopGraphiz.png"), tr("Stop Graphiz"), this);
	stopGraphvizAction->setStatusTip(tr("Break to generate Graphviz"));
	connect(stopGraphvizAction, SIGNAL(triggered()), this, SLOT(stopGraphviz()));
	////////////////////////////////////////////////////////////////////////
	tileAction = new QAction(tr("&Tile"), this);
	tileAction->setStatusTip(tr("Tile the windows"));
	connect(tileAction, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
	
	cascadeAction = new QAction(tr("&Cascade"), this);
	cascadeAction->setStatusTip(tr("Cascade the windows"));
	connect(cascadeAction, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
	//////////////////////////////////////////////////////////////////////////////////
	aboutAction = new QAction(QIcon("./images/info.png"), tr("&About"), this);
	aboutAction->setStatusTip(tr("Show the application's About box"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(tr("About &Qt"), this);
	aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}
void Pif_IDE::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addAction(closeAction);
	fileMenu->addSeparator();
	QAction *action = fileMenu->addAction(tr("Switch layout direction"));
	connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);

	settingMenu = menuBar()->addMenu(tr("&Setting"));
	settingMenu->addAction(fontSetupAction);
	settingMenu->addAction(pathSetupAction);

	buildMenu = menuBar()->addMenu(tr("&Build"));
	buildMenu->addAction(buildAction);
	buildMenu->addAction(stopBuildAction);
	buildMenu->addSeparator();
	buildMenu->addAction(runAction);
	buildMenu->addAction(stopRunAction);
	//buildMenu->addSeparator();
	//buildMenu->addAction(debugRunAction);
	//buildMenu->addAction(debugStopAction);
	buildMenu->addSeparator();
	buildMenu->addAction(jpegRIGFuncAction);
	buildMenu->addAction(jpegCGFuncAction);
	buildMenu->addAction(stopGraphvizAction);
	buildMenu->addSeparator();
	buildMenu->addAction(loadArgFileAction);

	windowMenu = menuBar()->addMenu(tr("&Windows"));
	windowMenu->addAction(tileAction);
	windowMenu->addAction(cascadeAction);

 	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}
void Pif_IDE::createToolBars()
{
	fileToolBar = ui.mainToolBar;
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(cutAction);
	editToolBar->addAction(copyAction);
	editToolBar->addAction(pasteAction);

	buildToolBar = addToolBar(tr("Build"));
	buildToolBar->addAction(buildAction);
	buildToolBar->addAction(stopBuildAction);
	buildToolBar->addSeparator();
	buildToolBar->addAction(runAction);
	buildToolBar->addAction(stopRunAction);
	//buildToolBar->addSeparator();
	//buildToolBar->addAction(debugRunAction);
	//buildToolBar->addAction(debugStopAction);
	buildToolBar->addSeparator();
	buildToolBar->addAction(jpegRIGFuncAction);
	buildToolBar->addAction(jpegCGFuncAction);
	buildToolBar->addAction(stopGraphvizAction);
}
void Pif_IDE::createStatusBars()
{
	statusBar()->showMessage(tr("ready"));
}
void Pif_IDE::createDockWidget()
{
	setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks);
	
	logOutDock = new QDockWidget(tr("Log Out"));
	logOutDock->setObjectName("logOutDock");
	logOutDock->setWidget(log_Out);
	addDockWidget(Qt::BottomDockWidgetArea, logOutDock);

	resOutDock = new QDockWidget(tr("Result Out"));
	resOutDock->setObjectName("resOutDock");
	resOutDock->setWidget(res_Out);
	addDockWidget(Qt::BottomDockWidgetArea, resOutDock);

	consoleDock = new QDockWidget(tr("Console"));
	consoleDock->setObjectName("consoleDock");
	consoleDock->setWidget(console);
	addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

	argDock = new QDockWidget(tr("Argument"));
	argDock->setObjectName("argDock");
	argDock->setWidget(argList);
	addDockWidget(Qt::BottomDockWidgetArea, argDock);

	funcDock = new QDockWidget(tr("Function"));
	funcDock->setObjectName("funcDock");
	funcDock->setWidget(funcList);
	addDockWidget(Qt::RightDockWidgetArea, funcDock);

	repDock = new QDockWidget(tr("Repository"));
	repDock->setObjectName("repDock");
	repDock->setWidget(repList);
	addDockWidget(Qt::RightDockWidgetArea, repDock);
}
void Pif_IDE::writeSettings()
{
	QSettings settings("QTProject", "Pifagor IDE 0.1");
	settings.beginGroup("Pifagor IDE");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.endGroup();
	settings.sync();
}
void Pif_IDE::readSettings()
{
	QSettings settings("QtProject", "Pifagor IDE 0.1");
    settings.beginGroup("Pifagor IDE");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	settings.endGroup();
	move(pos);
	resize(size);
}
Text_Edit *Pif_IDE::activeText_Edit()
{
	if(QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
		return qobject_cast<Text_Edit *>(activeSubWindow->widget());
	else if(QMdiSubWindow *activeSubWindow = mdiArea->currentSubWindow())
		return qobject_cast<Text_Edit *>(activeSubWindow->widget());
	return NULL;
}
QMdiSubWindow *Pif_IDE::findText_Edit(const QString &fileName)
{
	QString cannonicalFilePath = QFileInfo(fileName).canonicalFilePath();
	foreach (QMdiSubWindow *window, mdiArea->subWindowList())	{
		Text_Edit *testedit = qobject_cast<Text_Edit *>(window->widget());
		if(testedit->getCurrentFile() == cannonicalFilePath)
			return window;
	}
	return 0;
}
QMdiSubWindow *Pif_IDE::findNewText_Edit(const QString &fileName)
{
	foreach (QMdiSubWindow *window, mdiArea->subWindowList())	{
		Text_Edit *testedit = qobject_cast<Text_Edit *>(window->widget());
		if(testedit->getCurrentFile() == fileName)
			return window;
	}
	return 0;
}
void Pif_IDE::switchLayoutDirection()
{
	if(layoutDirection() == Qt::LeftToRight)
		qApp->setLayoutDirection(Qt::RightToLeft);
	else
		qApp->setLayoutDirection(Qt::LeftToRight);
}
void Pif_IDE::setActiveSubWindow(QWidget *window)
{
	if(!window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}
void Pif_IDE::closeFile()
{
	QMessageBox msgClose;
	msgClose.setText("Close");
	msgClose.setInformativeText("Do you want to save this file?");
	msgClose.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);
	msgClose.setDefaultButton(QMessageBox::Save);
	int ret = msgClose.exec();
	if(ret == QMessageBox::Save) {
		emit saveFile();
		mdiArea->closeActiveSubWindow();
	} else if(ret == QMessageBox::Discard) {
		mdiArea->closeActiveSubWindow();
	}
}
void Pif_IDE::pathSetting()
{
	if(setPath->exec() == QDialog::Accepted) 
	{
		stdValue.pathProject = setPath->getPathProject();
		stdValue.pathTrans = setPath->getPathTrans();
		stdValue.pathCgen = setPath->getPathCgen();
		stdValue.pathInter = setPath->getPathInter();
		stdValue.pathRig2dot = setPath->getPathRig2dot();
		stdValue.pathCg2dot = setPath->getPathCg2dot();
		stdValue.pathDot2jpeg = setPath->getPathDot2jpeg();
		stdValue.pathRepository = setPath->getPathRepository();
		stdValue.pathTemp = setPath->getPathTemp();
		stdValue.pathTools = setPath->getPathTools();
	}
}
void Pif_IDE::loadArgFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Argument file", "./", "*.txt *.pfg *.arg");
	if(!fileName.isEmpty()) {
		argList->loadFile(fileName);
		statusBar()->showMessage(tr("File arguments"), 2000);
	}
}
void Pif_IDE::buildReady()
{
	QByteArray BA = builProcess->readAll();
	log_Out->append(decoder->toUnicode(BA));
	QString fileName = "./err.log", errStr;
	QFile err(fileName);
	if(err.open(QFile::ReadOnly | QFile::Text)) 
	{
		QTextStream ifst(&err);
		errStr = ifst.readAll();
		log_Out->append(errStr);
		res_Out->setPlainText(errStr);
	}
	if(errStr == NULL)
		cgenRun();
}
void Pif_IDE::cgenReady()
{
	disconnect(cgenListProcess->at(0), SIGNAL(readyReadStandardOutput()), this, SLOT(cgenReady()));
	QByteArray BA = cgenListProcess->at(0)->readAll();
	log_Out->append(decoder->toUnicode(BA));
	cgenListProcess->removeAt(0);
	if(cgenListProcess->size() == 0)
	{
		createRepList();
		updateMenus();
	}
	else
	{
		connect(cgenListProcess->at(0), SIGNAL(readyReadStandardOutput()), this, SLOT(cgenReady()));
		cgenListProcess->at(0)->start();
	}
}
void Pif_IDE::resultReady()
{
	QByteArray BA = runProcess->readAll();
	res_Out->clear();
	log_Out->append(decoder->toUnicode(BA));
	res_Out->showResult(decoder->toUnicode(BA));
}
void Pif_IDE::createFuncList()
{
	funcList->clear();
	functionList = funcList->createList(activeText_Edit()->toPlainText());
}
void Pif_IDE::refreshFuncListW()
{
	funcList->addItems(functionList);
}
void Pif_IDE::createRepList()
{
	repList->createList();
}
void Pif_IDE::build()
{
    if(!saveFile())
		return;
	if(builProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous build had not been completed"), QMessageBox::Yes);
		return;
	}
	if(mdiArea->currentSubWindow() == NULL)
	{
		QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
		return;
	}
    createFuncList();
    funcList->setCurrentItem(funcList->item(0));
    if(funcList->currentItem() == NULL)
    {
        funcList->clear();
        QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
        return;
    }
	QString fileName = activeText_Edit()->getCurrentFile(), cmd;
	if(stdValue.pathTrans == NULL)
		cmd = "\"" + stdValue.pathTools + "/trans2.exe\" -t \"" + fileName + "\" err.log dbg.log";
	else
		cmd = "\"" + stdValue.pathTrans + "\" - t \"" + fileName + "\" err.log dbg.log";
	QFileInfo FI(fileName);
	if(!FI.exists())
	{
        QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName,QMessageBox::Yes);
		return;
	}
	builProcess->start(cmd);
	updateMenus();
}
void Pif_IDE::cgenRun()
{
 	QProcess *cgenProcess;
	QString fileName, functionName, cmd;
	for(int i = 0; i < functionList.count(); i++)
	{
		functionName = functionList.at(i);
		fileName = stdValue.pathRepository + "/" + functionName + "/001/";
		if(stdValue.pathCgen == NULL)
			cmd = "\"" + stdValue.pathTools + "/cgen2.exe\" -f \"" + fileName + "rig.txt\" \"" + fileName +"cg.txt\"";
		else
			cmd = "\"" + stdValue.pathCgen + "\" -t \"" + fileName + "rig.txt\" \"" + fileName + "cg.txt\"";
		cgenProcess = new QProcess;
		cgenListProcess->push_back(cgenProcess);
		cgenListProcess->at(i)->setProgram(cmd);
	}
	connect(cgenListProcess->at(0), SIGNAL(readyReadStandardOutput()), this, SLOT(cgenReady()));
	cgenListProcess->at(0)->start(cmd);
	updateMenus();
}
void Pif_IDE::run()
{
	if(runProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous program has not is finished"), QMessageBox::Yes);
		return;
	}
	argList->argBuild();
	QFileInfo FI("./arg.rig");
	if(!FI.exists())
	{
		QMessageBox::warning(this,tr("Error"),"File not exist! arg.rig",QMessageBox::Yes);
		return;
	}
	if(funcList->currentItem() == NULL)
	{
		QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
		return;
	}
	QString functionName = funcList->currentItem()->text(), cmd;
	if(stdValue.pathInter == NULL)
		cmd = "\"" + stdValue.pathTools + "/inter2.exe\" " + functionName;
	else
		cmd = "\"" + stdValue.pathInter +  "\" " +functionName;
	runProcess->start(cmd);
	updateMenus();
}
void Pif_IDE::stopBuild()
{
	if(builProcess->state() != QProcess::NotRunning)
		builProcess->kill();
	while(cgenListProcess->size() != 0)
	{
		if(cgenListProcess->at(0)->state() != QProcess::NotRunning)
			cgenListProcess->at(0)->kill();
		cgenListProcess->removeAt(0);
	}
	updateMenus();
}
void Pif_IDE::stopRun()
{
	if(runProcess->state() != QProcess::NotRunning)
		runProcess->kill();
	updateMenus();
}
void Pif_IDE::stopGraphviz()
{
	if(rigToDotProcess->state() != QProcess::NotRunning)
		rigToDotProcess->kill();
	if(dotToPNGRIGProcess->state() != QProcess::NotRunning)
		dotToPNGRIGProcess->kill();
	if(cgToDotProcess->state() != QProcess::NotRunning)
		cgToDotProcess->kill();
	if(dotToPNGCGProcess->state() != QProcess::NotRunning)
		dotToPNGCGProcess->kill();
	if(rigToDotRepProcess->state() != QProcess::NotRunning)
		rigToDotRepProcess->kill();
	if(dotToPNGRIGRepProcess->state() != QProcess::NotRunning)
		dotToPNGRIGRepProcess->kill();
	if(cgToDotRepProcess->state() != QProcess::NotRunning)
		cgToDotRepProcess->kill();
	if(dotToPNGCGRepProcess->state() != QProcess::NotRunning)
		dotToPNGCGRepProcess->kill();
	updateMenus();
}
/*
void Pif_IDE::debugRun()
{

}
void Pif_IDE::debugStop()
{

}
*/
void Pif_IDE::jpegRIGFunc()
{
	if(rigToDotProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous RIGToDot has not is finished"), QMessageBox::Yes);
		return;
	}
	if(funcList->currentItem() == NULL)
	{
		QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
		return;
	}
	QString functionName = funcList->currentItem()->text(), cmd, fileName;
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	if(stdValue.pathRig2dot != NULL)
		cmd = "\"" + stdValue.pathRig2dot + "\" \"" + fileName + "rig.txt\"";
	else
		cmd = "\"" + stdValue.pathTools + "/rig2dot.exe\" \"" + fileName + "rig.txt\"";
	QFileInfo FI(fileName + "rig.txt");
	if(!FI.exists())
	{
        QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName + "rig.txt",QMessageBox::Yes);
		return;
	}
	rigToDotProcess->start(cmd);
	updateMenus();
}
void Pif_IDE::dotToPNGRIG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	if(dotToPNGRIGProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous DotToPNG has not is finished"), QMessageBox::Yes);
		return;
	}
	if(funcList->currentItem() == NULL)
	{
		QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
		return;
	}
	QString functionName = funcList->currentItem()->text(), cmd, fileName;
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	QFileInfo FIdot(fileName + "rig.txt.dot");
	if(!FIdot.exists())
	{
		QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName + "rig.txt.dot",QMessageBox::Yes);
		return;
	}
	cmd = "\"" + stdValue.pathDot2jpeg + "\" -Tpng \"" + fileName + "rig.txt.dot\"";
	dotToPNGRIGProcess->setStandardOutputFile(fileName + "rig.png");
	dotToPNGRIGProcess->start(cmd);
}
void Pif_IDE::outRIG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	QString functionName = funcList->currentItem()->text(), cmd, fileName;
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	showRIGGraph->setWindowTitle(functionName);
	showRIGGraph->showGraph(fileName + "rig.png");
	updateMenus();
}
void Pif_IDE::jpegCGFunc()
{
	if(cgToDotProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous CGToDot has not is finished"), QMessageBox::Yes);
		return;
	}
	if(funcList->currentItem() == NULL)
	{
		QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
		return;
	}
	QString functionName = funcList->currentItem()->text(), cmd, fileName;
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	if(stdValue.pathRig2dot != NULL)
		cmd = "\"" + stdValue.pathRig2dot + "\" \"" + fileName + "cg.txt\"";
	else
		cmd = "\"" + stdValue.pathTools + "/cg2dot.exe\" \"" + fileName + "cg.txt\"";
	QFileInfo FI(fileName + "cg.txt");
	if(!FI.exists())
	{
        QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName + "cg.txt",QMessageBox::Yes);
		return;
	}
	cgToDotProcess->start(cmd);
	updateMenus();
}
void Pif_IDE::dotToPNGCG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	if(dotToPNGCGProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous RIGtoDot has not is finished"), QMessageBox::Yes);
		return;
	}
	if(funcList->currentItem() == NULL)
	{
		QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
		return;
	}
	QString functionName = funcList->currentItem()->text(), cmd, fileName;
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	QFileInfo FIdot(fileName + "cg.txt.dot");
	if(!FIdot.exists())
	{
		QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName + "cg.txt.dot",QMessageBox::Yes);
		return;
	}
	cmd = "\"" + stdValue.pathDot2jpeg + "\" -Tpng \"" + fileName + "cg.txt.dot\"";
	dotToPNGCGProcess->setStandardOutputFile(fileName+ "cg.png");
	dotToPNGCGProcess->start(cmd);
}
void Pif_IDE::outCG(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	QString functionName = funcList->currentItem()->text(), cmd, fileName;
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	showCGGraph->setWindowTitle(functionName);
	showCGGraph->showGraph(fileName + "cg.png");
	updateMenus();
}
void Pif_IDE::jpegRIGFuncToRep()
{
	if(rigToDotRepProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous RIGtoDot has not is finished"), QMessageBox::Yes);
		return;
	}
	if(repList->currentItem() == NULL)
	{
		QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
		return;
	}
	QVector<QString> longFuncName;
	QTreeWidgetItem* curTreeItem = repList->currentItem();
	QString functionName, cmd, fileName;
	while(curTreeItem != NULL)
	{
		longFuncName<<curTreeItem->text(0);
		curTreeItem = curTreeItem->parent();
	}
	for(int i = longFuncName.size() - 1; i >= 0; i--)
	{
		functionName += longFuncName.at(i);
		if(i != 0)
			functionName += ".";
	}
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	if(stdValue.pathRig2dot != NULL)
		cmd = "\"" + stdValue.pathRig2dot + "\" \"" + fileName + "rig.txt\"";
	else
		cmd = "\"" + stdValue.pathTools + "/rig2dot.exe\" \"" + fileName + "rig.txt\"";
	QFileInfo FI(fileName + "rig.txt");
	if(!FI.exists())
	{
        QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName + "rig.txt",QMessageBox::Yes);
		return;
	}
	rigToDotRepProcess->start(cmd);
}
void Pif_IDE::dotToPNGRIGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	if(dotToPNGRIGRepProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous RIGGraphiz has not is finished"), QMessageBox::Yes);
		return;
	}
	QVector<QString> longFuncName;
	QTreeWidgetItem* curTreeItem = repList->currentItem();
	QString functionName, cmd, fileName;
	while(curTreeItem != NULL)
	{
		longFuncName<<curTreeItem->text(0);
		curTreeItem = curTreeItem->parent();
	}
	for(int i = longFuncName.size() - 1; i >= 0; i--)
	{
		functionName += longFuncName.at(i);
		if(i != 0)
			functionName += ".";
	}
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	QFileInfo FIdot(fileName + "rig.txt.dot");
	if(!FIdot.exists())
	{
		QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName + "rig.txt.dot",QMessageBox::Yes);
		return;
	}
	cmd = "\"" + stdValue.pathDot2jpeg + "\" -Tpng \"" + fileName + "rig.txt.dot\"";
	dotToPNGRIGRepProcess->setStandardOutputFile(fileName + "rig.png");
	dotToPNGRIGRepProcess->start(cmd);
}
void Pif_IDE::outRIGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	QVector<QString> longFuncName;
	QTreeWidgetItem* curTreeItem = repList->currentItem();
	QString functionName, cmd, fileName;
	while(curTreeItem != NULL)
	{
		longFuncName<<curTreeItem->text(0);
		curTreeItem = curTreeItem->parent();
	}
	for(int i = longFuncName.size() - 1; i >= 0; i--)
	{
		functionName += longFuncName.at(i);
		if(i != 0)
			functionName += ".";
	}
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	showRIGGraph->setWindowTitle(functionName);
	showRIGGraph->showGraph(fileName + "rig.png");
}
void Pif_IDE::jpegCGFuncToRep()
{
	if(cgToDotRepProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous CGToDot has not is finished"), QMessageBox::Yes);
		return;
	}
	if(repList->currentItem() == NULL)
	{
		QMessageBox::warning(this,tr("Error"), tr("Don't choose function"), QMessageBox::Yes);
		return;
	}
	QVector<QString> longFuncName;
	QTreeWidgetItem* curTreeItem = repList->currentItem();
	QString functionName, cmd, fileName;
	while(curTreeItem != NULL)
	{
		longFuncName<<curTreeItem->text(0);
		curTreeItem = curTreeItem->parent();
	}
	for(int i = longFuncName.size() - 1; i >= 0; i--)
	{
		functionName += longFuncName.at(i);
		if(i != 0)
			functionName += ".";
	}
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	if(stdValue.pathRig2dot != NULL)
		cmd = "\"" + stdValue.pathRig2dot + "\" \"" + fileName + "cg.txt\"";
	else
		cmd = "\"" + stdValue.pathTools + "/cg2dot.exe\" \"" + fileName + "cg.txt\"";
	QFileInfo FI(fileName + "cg.txt");
	if(!FI.exists())
	{
        QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName + "cg.txt",QMessageBox::Yes);
		return;
	}
	cgToDotRepProcess->start(cmd);
}
void Pif_IDE::dotToPNGCGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	if(dotToPNGCGRepProcess->state() != QProcess::NotRunning)
	{
		QMessageBox::warning(this,tr("Error"), tr("The previous CGGraphiz has not is finished"), QMessageBox::Yes);
		return;
	}
	QVector<QString> longFuncName;
	QTreeWidgetItem* curTreeItem = repList->currentItem();
	QString functionName, cmd, fileName;
	while(curTreeItem != NULL)
	{
		longFuncName<<curTreeItem->text(0);
		curTreeItem = curTreeItem->parent();
	}
	for(int i = longFuncName.size() - 1; i >= 0; i--)
	{
		functionName += longFuncName.at(i);
		if(i != 0)
			functionName += ".";
	}
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	cmd = "\"" + stdValue.pathDot2jpeg + "\" -Tpng \"" + fileName + "cg.txt.dot\"";
	QFileInfo FIdot(fileName + "cg.txt.dot");
	if(!FIdot.exists())
	{
		QMessageBox::warning(this,tr("Error"),"File not exist! " + fileName + "cg.txt.dot",QMessageBox::Yes);
		return;
	}
	dotToPNGCGRepProcess->setStandardOutputFile(fileName + "cg.png");
	dotToPNGCGRepProcess->start(cmd);
}
void Pif_IDE::outCGRep(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	QVector<QString> longFuncName;
	QTreeWidgetItem* curTreeItem = repList->currentItem();
	QString functionName, cmd, fileName;
	while(curTreeItem != NULL)
	{
		longFuncName<<curTreeItem->text(0);
		curTreeItem = curTreeItem->parent();
	}
	for(int i = longFuncName.size() - 1; i >= 0; i--)
	{
		functionName += longFuncName.at(i);
		if(i != 0)
			functionName += ".";
	}
	fileName = stdValue.pathRepository + "/" + functionName + "/001/";
	showCGGraph->setWindowTitle(functionName);
	showCGGraph->showGraph(fileName + "cg.png");
}
Pif_IDE::~Pif_IDE() {}
