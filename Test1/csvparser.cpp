#include "csvparser.h"
#include <QtDebug>
CsvParser::CsvParser(QChar separator)	:
	m_separator(separator)
{

}

void CsvParser::Parse(const QString &csvFilePath, QList<QStringList> &parsingResult)
{
	m_csvFile.setFileName(csvFilePath);
	if (!m_csvFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "Couldn't open the file to read";
		return;
	}
	QTextStream stream(&m_csvFile);
	QString csvString = stream.readAll();
	m_csvFile.close();
	csvString.remove(QChar('"'));
	csvString.remove(QChar('\t'));
	csvString.remove(QChar('\r'));
	QList<QString> temporary = csvString.split('\n');
	foreach (auto item, temporary)
	{
		if (item == QString(""))
			continue;
		parsingResult.append(item.split(m_separator));
	}

}

