#include "csvparser.h"
#include <QtDebug>
CsvParser::CsvParser(const QString &csvFilePath, QChar separator)
{

}

void CsvParser::Parse(const QString &csvFilePath, QList<QStringList> &parsingResult)
{
	m_csvFile.setFileName(csvFilePath);
	if (!m_csvFile.open(QIODevice::ReadOnly))
		return;
	QTextStream stream(&m_csvFile);
	QString csvString = stream.readAll();
	csvString.remove(QChar('"'));
	csvString.remove(QChar('\t'));
	QList<QString> temporary = csvString.split('\n');
	foreach (auto item, temporary)
	{
		if (item == QString(""))
			continue;
		parsingResult.append(item.split(','));
	}
}

