#include "movementanalyzer.h"
#include <QMap>
#define HAS_COMMAS(...) __VA_ARGS__

MovementAnalyzer::MovementAnalyzer()
{

}


void MovementAnalyzer::makeReport(const QString &sourceCSV, const QString &resultTXT)
{
	QFile result(resultTXT);

	QMap <qlonglong, QString> tst;
	QTextStream out(&result);
	result.open(QIODevice::WriteOnly | QIODevice::Text);
// открыть файл CSV
// прочесть и положить в удобный вид
	m_csvParser.Parse(sourceCSV, m_csvParsedLines);

	auto itLine = m_csvParsedLines.begin();
	while (itLine != m_csvParsedLines.end())
	{
		QStringList& currentLine = *itLine;
		QTime time = QTime::fromString(currentLine[TIME].split(" ").last(), "hh:mm:ss");

		// Если строка меньше трех столбцов содержит, то зачем это всё...
		// времени нет - смысла нет.
		if (currentLine.size() == COLUMNS_COUNT && time.isValid())
		{
			m_movementDataDic[currentLine[ID].toULongLong()].insert(time, currentLine[SPEED].toUInt());
		}
		else
		{
			m_skippedLines << currentLine;
		}

		++itLine;
	}

//	foreach (auto currenKey, m_movementDataDic.keys())
//	{
//		qDebug() << m_movementDataDic[currenKey];
//	}

	qDebug() << "CSV parsed lines count: " << m_csvParsedLines.size();
	qDebug() << "Lines skipped (total): "<< m_skippedLines.size();
// закрыть файл
// посчитать времена
	// concurrentRun?
// вывести в файл TXT
	out << "dsdsd";

}
