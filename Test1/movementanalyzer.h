#ifndef MOVEMENTANALYZER_H
#define MOVEMENTANALYZER_H
#include <QTime>
#include <QMap>
#include "csvparser.h"


class MovementAnalyzer
{
	typedef QMap <qulonglong, QMap<QTime, uint>> movementDataType;
	typedef QMap <qulonglong, qreal> reportType;
public:
	MovementAnalyzer();
	void MakeFileAnalyze(const QString &sourceCSV, const QString &resultTXT);
private:
	void MakeDayReport(const reportType&);

	qreal SexToDec(quint32);			/// секунды в десятые часа
	void InitializeDataDictionary();
	void AnalyzeMovement(movementDataType&);
	void AnalyzeSingleUnitMovement(qlonglong, QMap<QTime, quint32>);
	enum COLUMNS{TIME, ID, SPEED, COLUMNS_COUNT};
	// Вывод
	QString					m_resultFile = "./result.txt";
	const QString			m_reportSeparator = "----";
	reportType				m_movementStatistic;		/// Храним время движения каждого ID
	QList<QStringList>		m_skippedLines;				/// Количество неучтенных строк (ввиду несоответствия формата)

	// Ввод+работа
	CsvParser				m_csvParser;
	QList<QStringList>		m_csvParsedLines;			/// CSV parsing result
	movementDataType		m_movementDataDic;			/// storing data
	QString					m_endTime = "23:59:59.999";
	quint8					m_timeoutSecond = 120;		/// Таймаут по которому считается, что тачка остановилась


};

#endif // MOVEMENTANALYZER_H
