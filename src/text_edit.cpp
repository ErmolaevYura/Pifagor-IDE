#include <QtWidgets>

#include "general.h"
#include "text_edit.h"
#include "line_number.h"

Text_Edit::Text_Edit(QWidget *parent) : QTextEdit(parent)
{
	setAcceptRichText(false);
	lineNumArea = new Line_Number(this);
	codeLighter = new Code_Lighter(this);
	codeLighter->setDocument(document());
	setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_AcceptDrops);
    setWindowIcon(QIcon("./images/file.png"));
	connect(this, SIGNAL(contentsChanged()), codeLighter, SLOT(rehighlight()));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(brackLight()));
	connect(document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumAreaWidth(int)));
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumArea(int)));
	connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumArea()));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumArea()));
	updateLineNumAreaWidth(0);
	isUntitled = true;
}
int Text_Edit::lineNumAreaWidth()
{
	int digits = 1;
	int max = qMax(1, document()->blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}
	int space = 15 + fontMetrics().width(font().pointSize()) * digits;
	return space;
}
void Text_Edit::updateLineNumAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(lineNumAreaWidth(), 0, 0, 0);
}
void Text_Edit::updateLineNumArea(QRectF /*rect_f*/)
{
	updateLineNumArea();
}
void Text_Edit::updateLineNumArea(int /*slider_pos*/)
{
	updateLineNumArea();
}
void Text_Edit::updateLineNumArea()
{
	updateLineNumAreaWidth(verticalScrollBar()->sliderPosition());
	QRect rect = contentsRect();
	lineNumArea->update(0, rect.y(), lineNumArea->width(), rect.height());
	updateLineNumAreaWidth(0);
	int dy = verticalScrollBar()->sliderPosition();
	if(dy > -1)
		lineNumArea->scroll(0, dy);
	int firstBlockID = getFirstVisibleBlockId();
	if(firstBlockID == 0 || textCursor().block().blockNumber() == firstBlockID-1)
		verticalScrollBar()->setSliderPosition(dy - document()->documentMargin());
}
void Text_Edit::resizeEvent(QResizeEvent *e)
{
	QTextEdit::resizeEvent(e);
	QRect curRect = contentsRect();
	lineNumArea->setGeometry(QRect(curRect.left(), curRect.top(), lineNumAreaWidth(), curRect.height()));
}
int Text_Edit::getFirstVisibleBlockId()
{
	QTextCursor curs = QTextCursor(document());
	curs.movePosition(QTextCursor::Start);
	for(int i=0; i < document()->blockCount(); ++i)
	{
		QTextBlock block = curs.block();
		QRect r1 = viewport()->geometry();
		QRect r2 = document()->documentLayout()->blockBoundingRect(block).translated(
			this->viewport()->geometry().x(), viewport()->geometry().y() - (
			this->verticalScrollBar()->sliderPosition()
			) ).toRect();
		if(r1.contains(r2, true)) { return i; }
		curs.movePosition(QTextCursor::NextBlock);
	}
	return 0;
}
void Text_Edit::brackLight()
{
	QTextCursor cursor = textCursor();
	codeLighter->bracketsLight(cursor, document()->toPlainText());
}
void Text_Edit::lineNumAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumArea);
	painter.fillRect(event->rect(), Qt::lightGray);
	int blockNumber = getFirstVisibleBlockId();
	QTextBlock block = document()->findBlock(blockNumber);
	QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
	int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;
	int top = viewport()->geometry().top();
	int additional_margin;
	if(blockNumber == 0)
		additional_margin = (int)document()->documentMargin() - 1 - this->verticalScrollBar()->sliderPosition();
	else
		additional_margin = (int)document()->documentLayout()->blockBoundingRect(prev_block)
		.translated(0, translate_y).intersected(viewport()->geometry()).height();
	top += additional_margin;
	int bottom = top + (int)document()->documentLayout()->blockBoundingRect(block).height();
	while(block.isValid() && top <= event->rect().bottom()) 
	{
		if(block.isVisible() && bottom >= event->rect().top()) 
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen((this->textCursor().blockNumber() == blockNumber) ? Qt::darkGreen : Qt::darkGray);

			painter.drawText(-5, top, lineNumArea->width(), fontMetrics().height(), Qt::AlignRight, number);
		}
		block = block.next();
		top = bottom;
		bottom = top + (int) document()->documentLayout()->blockBoundingRect(block).height();
		++blockNumber;
	}
}
void Text_Edit::newFile()
{
	static int sequenceNumber = 1;
	isUntitled = true;
	curFile = tr("project%1.pif").arg(sequenceNumber++);
	setWindowTitle(curFile + "[*]");
	connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
}
bool Text_Edit::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if(!QFile::exists(fileName))
		return false;
	if(!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Pifagor"), 
			tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return false;
	}
	QTextStream ifst(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QString buf = ifst.readAll();
	setPlainText(buf);
	QApplication::restoreOverrideCursor();
	setCurrentFile(fileName);
	connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
	return true;
}
bool Text_Edit::loadFile(const QString &fileName, QString funcName)
{
    curFile = tr("new") + funcName + tr(".pif");
    setWindowTitle(curFile + "[*]");
    QFile file(fileName);
    if(!QFile::exists(fileName))
        return false;
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Pifagor"),
            tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream ifst(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(ifst.readAll());
    QApplication::restoreOverrideCursor();
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;
}
bool Text_Edit::save()
{
	if(isUntitled) {
		return saveAs();
	} else {
		return saveFile(curFile);
	}
}
bool Text_Edit::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), stdValue.pathProject + "/" + curFile);
	if(fileName.isEmpty())
		return false;
	return saveFile(fileName);
}
bool Text_Edit::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if(!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Pifagor"),
			tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return false;
	}
	QTextStream ofst(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	ofst<<toPlainText();
	QApplication::restoreOverrideCursor();
	setCurrentFile(fileName);
	return true;
}
QString Text_Edit::userFriendlyCurrentFile()
{
	return strippedName(curFile);
}

QString Text_Edit::getCurrentFile() 
{
	return curFile; 
}
void Text_Edit::closeEvent(QCloseEvent *event)
{
	if(maybeSave()) {
		event->accept();
	} else {
		event->ignore();
	}
}
void Text_Edit::documentWasModified()
{
	setWindowModified(document()->isModified());
}
Code_Lighter* Text_Edit::getCodeLighter()
{
	return codeLighter;
}
bool Text_Edit::maybeSave()
{
	if(document()->isModified()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Pifagor"), tr("'%1' has been modified.\n"
			"Do you want to save your changes?").arg(userFriendlyCurrentFile()),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}
void Text_Edit::setCurrentFile(const QString &fileName)
{
	curFile = QFileInfo(fileName).canonicalFilePath();
	isUntitled = false;
	document()->setModified(false);
	setWindowModified(false);
	setWindowTitle(userFriendlyCurrentFile() + "[*]");
}
QString Text_Edit::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}
Text_Edit::~Text_Edit() { }
