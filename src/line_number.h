#ifndef LINE_NUMBER_H
#define LINE_NUMBER_H

#include <QWidget>

#include "text_edit.h"

class Line_Number : public QWidget
{
public:
	Line_Number(Text_Edit *editor);
	QSize sizeHint() const;
	~Line_Number();
protected:
	void paintEvent(QPaintEvent *event);
private:
	Text_Edit *textEdit;
};

#endif // LINE_NUMBER_H