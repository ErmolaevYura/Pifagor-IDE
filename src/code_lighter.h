#ifndef CODE_LIGHTER_H
#define CODE_LIGHTER_H

#include <QSyntaxHighlighter>
#include <QDir>
#include <QTextEdit>

class Code_Lighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	Code_Lighter(QObject *parent = 0);
	~Code_Lighter();
	void highlightBlock(const QString &text);
	bool setArgFormat(const QString text);
	void bracketsLight(const QTextCursor &textCursor, const QString &str);
public slots:
private:
	struct lightRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	int countArg, countRepFunc,
		leftBrackPos, rightBrackPos, line;
	QChar leftBrack, rightBrack;
	QVector<lightRule> lightRules;
	////
	QRegExp commentStartExpression;
	QRegExp commentEndExpression;
	////
	QTextCharFormat identifierFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat symbolFormat;
	QTextCharFormat numberFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat keywordFormat;
	QTextCharFormat valueWordFormat;
	QTextCharFormat functionFormat;
	QTextCharFormat funcArgFormat;
	QTextCharFormat bracketsFormat;
	QTextCharFormat repFuncFormat;
	QTextCharFormat argFormat;
	//////
	void setRepFormat();
};

#endif // CODE_LIGHTER_H