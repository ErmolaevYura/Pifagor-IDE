#include "line_number.h"

Line_Number::Line_Number(Text_Edit *editor) : QWidget(editor)
{
	textEdit = editor;
}
QSize Line_Number::sizeHint() const
{
	return QSize(textEdit->lineNumAreaWidth(), 0);
}
void Line_Number::paintEvent(QPaintEvent *event) 
{
	textEdit->lineNumAreaPaintEvent(event);
}
Line_Number::~Line_Number(){}