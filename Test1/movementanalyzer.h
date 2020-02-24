#ifndef MOVEMENTANALYZER_H
#define MOVEMENTANALYZER_H
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QMultiMap>
#include <QtConcurrent/QtConcurrent>
#include "csvparser.h"
#include "csvreader.h"


class MovementAnalyzer
{
<<<<<<< Updated upstream
	enum COLUMNS{TIME, ID, SPEED, COLUMNS_COUNT};
	CsvReader	m_csvReader;
	CsvParser	m_csvParser;
	QList<QStringList>	m_csvParsedLines;					// CSV parsing result
	QMap <qlonglong, QMap<QTime, uint>> m_movementDataDic;	// storing data

	QList<QStringList>	m_skippedLines;						// Количество неучтенных строк (ввиду несоответствия формата)
=======
	typedef QMap <qulonglong, QMap<QTime, uint>> movementDataType;
	typedef QMap <qulonglong, qreal> reportType;
>>>>>>> Stashed changes
public:
	MovementAnalyzer();
	void MakeFileAnalyze(const QString &sourceCSV, const QString &resultTXT);
private:
	void MakeReport(const reportType&);

	qreal SexToDec(int);			// секунды в десятые часа
	void InitializeDataDictionary();
	void AnalyzeMovement(movementDataType&);
	void AnalyzeSingleUnitMovement(qlonglong, QMap<QTime, quint32>);
	enum COLUMNS{TIME, ID, SPEED, COLUMNS_COUNT};
	// Вывод
	QString					m_resultFile;
	const QString			m_reportSeparator = "----";
	reportType				m_movementStatistic;
	QList<QStringList>		m_skippedLines;				// Количество неучтенных строк (ввиду несоответствия формата)

	// Ввод+работа
	CsvParser				m_csvParser;
	QList<QStringList>		m_csvParsedLines;			// CSV parsing result
	movementDataType		m_movementDataDic;			// storing data
	QString					m_endTime = "23:59:59.999";
	quint8					m_timeoutSecond = 120;


};

#endif // MOVEMENTANALYZER_H
