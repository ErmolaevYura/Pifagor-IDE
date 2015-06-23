#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H

#include <QTextEdit>

#include "code_lighter.h"
class Line_Number;

class Text_Edit : public QTextEdit
{
	Q_OBJECT
public:
	Text_Edit(QWidget *parent = 0);
	~Text_Edit();
	void newFile();
	bool loadFile(const QString &fileName);
    bool loadFile(const QString &fileName,  QString funcName);
	bool save();
	bool saveAs();
	bool saveFile(const QString &fileName);
	QString userFriendlyCurrentFile();
	QString getCurrentFile();
	Code_Lighter* getCodeLighter();
	int getFirstVisibleBlockId();
	int lineNumAreaWidth();
	void lineNumAreaPaintEvent(QPaintEvent *event);
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void resizeEvent(QResizeEvent *event);
private slots:
	void updateLineNumAreaWidth(int newBlockCount);
	void updateLineNumArea(QRectF);
	void updateLineNumArea(int);
	void updateLineNumArea();
	void documentWasModified();
	void brackLight();
private:
	bool maybeSave();
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	QString curFile;
	bool isUntitled;
	QWidget *lineNumArea;
	Code_Lighter *codeLighter;
};

#endif // TEXT_EDIT_H
