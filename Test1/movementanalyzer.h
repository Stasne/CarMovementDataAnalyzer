#ifndef MOVEMENTANALYZER_H
#define MOVEMENTANALYZER_H
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QMultiMap>
#include "csvparser.h"


class MovementAnalyzer
{
	enum COLUMNS{TIME, ID, SPEED, COLUMNS_COUNT};
	CsvParser	m_csvParser;
	QList<QStringList>	m_csvParsedLines;					// CSV parsing result
	QMap <qlonglong, QMap<QTime, uint>> m_movementDataDic;	// storing data

	QList<QStringList>	m_skippedLines;						// Количество неучтенных строк (ввиду несоответствия формата)
public:
	MovementAnalyzer();
	void makeReport(const QString &sourceCSV, const QString &resultTXT);
};

#endif // MOVEMENTANALYZER_H
