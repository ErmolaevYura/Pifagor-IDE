#include "code_lighter.h"
#include "general.h"

Code_Lighter::Code_Lighter(QObject *parent) : QSyntaxHighlighter(parent)
{
	lightRule rule;
	countArg = 0;
	countRepFunc = 0;
	leftBrackPos = rightBrackPos = 0;
	line = 0;
	identifierFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\b[A-Za-z_]\\b|\\b[A-Za-z_]{1,1}[A-Za-z_0-9]+\\b");
	rule.format = identifierFormat;
	lightRules.append(rule);

	numberFormat.setForeground(Qt::darkRed);
	rule.pattern = QRegExp("(-{0,1}\\b[0-9]+\\b)|(-{0,1}\\b[0-9]+[.][0-9]+\\b)|(-{0,1}\\b[0-9]+[.]{0,1}[0-9]*[eE]{1,1}[-+]{0,1}[0-9]+\\b)");
	rule.format = numberFormat;
	lightRules.append(rule);

	keywordFormat.setForeground(Qt::darkBlue);
	rule.format = keywordFormat;
	QStringList keywordPatterns;
	keywordPatterns << "\\bblock\\b"<<"\\bbreak\\b"<<"\\bbool\\b"
		<<"\\bchar\\b"<<"\\bconst\\b"<<"\\bdup\\b"<<"\\bdatalist\\b"
		<<"\\bdelaylist\\b"<<"\\belse\\b"<<"\\berror\\b"<<"\\bfloat\\b"
		<<"\\bfunc\\b"<<"\\bint\\b"<<"\\bnil\\b"<<"\\bparlist\\b"
		<<"\\bprefunc\\b"<<"\\breturn\\b"<<"\\bsignal\\b"
		<<"\\btype\\b" << "\\btypedef\\b";
	for(int i = 0; i < keywordPatterns.length(); i++)
	{
		rule.pattern = QRegExp(keywordPatterns.at(i));
		lightRules.append(rule);
	}

	valueWordFormat.setForeground(Qt::darkBlue);
	valueWordFormat.setFontItalic(true);
	rule.format = valueWordFormat;
	QStringList valueWordPatterns;
	valueWordPatterns << "\\btrue\\b" << "\\bfalse\\b";
	for(int i = 0; i < valueWordPatterns.length(); i++)
	{
		rule.pattern = QRegExp(valueWordPatterns.at(i));
		lightRules.append(rule);
	}

	symbolFormat.setForeground(Qt::green);
	rule.pattern = QRegExp("\'.*\'");
	rule.format = symbolFormat;
	lightRules.append(rule);

	quotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	lightRules.append(rule);


	singleLineCommentFormat.setForeground(Qt::green);
	singleLineCommentFormat.setFontItalic(true);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	lightRules.append(rule);
	
	multiLineCommentFormat.setForeground(Qt::green);
	multiLineCommentFormat.setFontItalic(true);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = multiLineCommentFormat;
	lightRules.append(rule);


	funcArgFormat.setFontItalic(true);
	funcArgFormat.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\bfuncdef\\b[\\s\\t\\v]+\\b[A-Za-z_]\\b|\\bfuncdef\\b[\\s\\t\\v]+\\b[A-Za-z_]{1,1}[A-Za-z_0-9]+\\b");
	rule.format = funcArgFormat;
	lightRules.append(rule);

	functionFormat.setFontItalic(true);
	functionFormat.setFontWeight(QFont::Bold);
	functionFormat.setForeground(Qt::darkCyan);
	rule.pattern = QRegExp("\\b[A-Za-z_]{1,1}[A-Za-z0-9_.]*\\b[\\s\\t\\v]*[<]{2,2}[\\s\\t\\v]*\\bfuncdef\\b");
	rule.format = functionFormat;
	lightRules.append(rule);
	
	commentStartExpression = QRegExp("[/]{1,1}[*]{1,1}");
	commentEndExpression = QRegExp("[*]{1,1}[/]{1,1}");
	setRepFormat();

	bracketsFormat.setFontWeight(QFont::Bold);
}
void Code_Lighter::bracketsLight(const QTextCursor &textCursor, const QString &str)
{
 	int positionInLine = textCursor.positionInBlock(), positionInText = textCursor.position();
	QStringList strList = str.split("\n");
	if(!positionInText || positionInText ==	str.size())
		return;
	int countBrack = 0, countLatter = 0;
	line = 0;
	for(int i = 0; i < strList.size(); i++)
	{
		if(countLatter + strList.at(i).size() < positionInText)
			{
				countLatter += strList.at(i).size();
				countLatter++;
				line++;
			}
			else
				break;
	}
	for(int i = positionInText - 1; i >= 0; i--)
	{
		if(str.at(i) == '[' || str.at(i) == '(' || str.at(i) == '{')
		{	
			if(countBrack == 0)
			{
				leftBrack = str.at(i);
				leftBrackPos = i;
				break;
			}
			else
				countBrack--;
		}
		else if(str.at(i) == ']' || str.at(i) == ')' || str.at(i) =='}')
			countBrack++;
	}
	if(leftBrack == '[')
		rightBrack = ']';
	else if(leftBrack == '{')
		rightBrack = '}';
	else if(leftBrack == '(')
		rightBrack =')';
	for(int i = leftBrackPos + 1; i < str.size(); i++)
	{	
		if(str.at(i) == rightBrack)
		{
			if(countBrack == 0)
			{
				rightBrackPos = i;
				break;
			}
			else
				countBrack--;
		}
		else if(str.at(i) == ']' || str.at(i) == ')' || str.at(i) == '}')
			countBrack--;
		else if(str.at(i) == '[' || str.at(i) == '(' || str.at(i) == '{')
			countBrack++;
	}
	leftBrackPos -= countLatter;
	rightBrackPos -= countLatter ;
	if(rightBrackPos < 0)
		rightBrackPos = 0;
	if(leftBrackPos < 0)
		leftBrackPos = 0;
}
void Code_Lighter::setRepFormat()
{
	QRegExp *regExp = new QRegExp;
	lightRule rule;
	QDir dir;
	QStringList strList;
	QString bufStr;
	QStringList bufList;
	QFileInfoList list;
	dir.setFilter(QDir::AllDirs | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);
	dir.setPath(stdValue.pathRepository);
	list = dir.entryInfoList();
	for(int i = 0; i < list.count(); i++)
		if(list.at(i).fileName() != "." && list.at(i).fileName() != "..")
			strList.push_back(list.at(i).fileName());
	repFuncFormat.setFontItalic(true);
	repFuncFormat.setFontWeight(QFont::Bold);
	repFuncFormat.setForeground(Qt::blue);
	rule.format = repFuncFormat;
	for(int i = 0; i < strList.size(); i++)
	{
		regExp->setPattern("\\b" + strList.at(i) + "\\b");
		rule.pattern = *regExp;
		lightRules.push_back(rule);
		countRepFunc++;
	}
}
bool Code_Lighter::setArgFormat(const QString text)
{
	QRegExp *regExp = new QRegExp("\\bfuncdef\\b"), *regExp2 = new QRegExp("[A-Za-z_]{1,1}"), *regExp3 = new QRegExp;
	QString argName;
	int indexStart, indexEnd;
	lightRule rule;
	argFormat.setFontItalic(true);
	argFormat.setForeground(Qt::blue);
	rule.format = argFormat;
	bool clear = false;
	indexStart = regExp->indexIn(text);
	while(indexStart >= 0)
	{
		while(countArg && !clear)
		{
			lightRules.remove(6);
			countArg--;
		}
		clear = true;
		indexStart += 7;
		indexStart = indexEnd = regExp2->indexIn(text, indexStart);
		if(indexStart < 0 || indexEnd < 0)
		{
			clear = false;
			return false;
		}
		while(indexEnd < text.length() && (text[indexEnd].isLetter() || text[indexEnd].isDigit() || text[indexEnd] == '_'))
			indexEnd++;
		argName = text.mid(indexStart, indexEnd - indexStart);
		regExp3->setPattern("\\b" + argName + "\\b");
		rule.pattern = *regExp3;
		lightRules.insert(6, rule);
		countArg++;
		indexStart = regExp->indexIn(text, indexEnd);
	}
	if(clear)
		return true;
	return false;
}
void Code_Lighter::highlightBlock(const QString &text)
{
	setArgFormat(text);
	if(leftBrackPos != rightBrackPos && leftBrackPos > 0 && rightBrackPos > 0 && rightBrackPos < text.size())
	{
		if(text[leftBrackPos] == leftBrack)
			setFormat(leftBrackPos, 1, bracketsFormat);
		else if(leftBrackPos + 1 < text.size() && text[leftBrackPos + 1] == leftBrack)
			setFormat(leftBrackPos + 1, 1, bracketsFormat);
		else if(leftBrackPos - 1 > 0 && text[leftBrackPos - 1] == leftBrack)
			setFormat(leftBrackPos - 1, 1, bracketsFormat);
		if(text[rightBrackPos] == rightBrack)
			setFormat(rightBrackPos, 1, bracketsFormat);
		if(rightBrackPos + 1 < text.size() && text[rightBrackPos + 1] == rightBrack)
			setFormat(rightBrackPos + 1, 1, bracketsFormat);
		else if(rightBrackPos - 1 > 0 && text[rightBrackPos - 1] == rightBrack)
			setFormat(rightBrackPos - 1, 1, bracketsFormat);
	}
	QString str;
	for(int i = 0; i < lightRules.length(); i++)
	{
		QRegExp expression(lightRules.at(i).pattern);
		int index = expression.indexIn(text), length;
		while(index >= 0)
		{
			length = expression.matchedLength();
			setFormat(index, length, lightRules.at(i).format);
			index = expression.indexIn(text, index + length);
		}
		str = lightRules.at(i).pattern.pattern();
	}
	setCurrentBlockState(0);
	int startIndex = 0;
	if(previousBlockState() != 1)
		startIndex = commentStartExpression.indexIn(text);
	int endIndex, commentLength;
	while(startIndex >= 0)
	{
		endIndex = commentEndExpression.indexIn(text, startIndex);
		if(endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = startIndex + endIndex + 2;
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
	}
}
Code_Lighter::~Code_Lighter(){}