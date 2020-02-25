#include "movementanalyzer.h"
#include <QDebug>
MovementAnalyzer::MovementAnalyzer()
{

}



void MovementAnalyzer::MakeFileAnalyze(const QString &sourceCSV, const QString &resultTXT)
{
	m_resultFile = resultTXT == QString("")?m_resultFile:resultTXT;

// открыть файл CSV
// прочесть и положить в удобный вид
	m_csvParser.Parse(sourceCSV, m_csvParsedLines);

	InitializeDataDictionary();			/// Заполняем m_movementDataDic
	AnalyzeMovement(m_movementDataDic);	/// Заполняем структуру с результатами - movementStatistic
	MakeDayReport(m_movementStatistic);	/// вывод в файл resultTxt или default

	qDebug() << "CSV parsed lines count: " << m_csvParsedLines.size();
	qDebug() << "Results saved to " << m_resultFile ;
	m_csvParsedLines.clear();

}

void MovementAnalyzer::MakeDayReport(const reportType& dataToExport)
{
	// TODO: Русский текст выводить нужно
	QFile result(m_resultFile);
	QTextStream out(&result);
	result.open(QIODevice::WriteOnly | QIODevice::Text);
	out << m_reportSeparator << endl;
	auto it = dataToExport.begin();
	while (it != dataToExport.end())
	{
		out << "id: "			<< it.key() << endl;
		out << "drive time: "	<< it.value() << "h" << endl;
		out << "parking time: " << 24 - it.value() << "h" << endl;
		out << m_reportSeparator << endl;
		it++;
	}

	qDebug() << "skipped lines: "<< m_skippedLines.size() << ":";
	while ( !m_skippedLines.isEmpty() )
	{
		qDebug() << m_skippedLines.takeFirst();
	}
	result.close();

}

qreal MovementAnalyzer::SexToDec(quint32 secs)
{
	qreal out = secs/3600.0;
	return out;
}

void MovementAnalyzer::InitializeDataDictionary()
{
	auto itLine = m_csvParsedLines.begin();
	while (itLine != m_csvParsedLines.end())
	{
		QStringList& currentLine = *itLine;			// Никак не проверяем дату. Просто отбрасываем. Надеемся на вх. данные
		QTime time = QTime::fromString(currentLine[TIME].split(" ").last(), "hh:mm:ss");

		// Если строка меньше трех столбцов содержит,или временя-инвалид - в топку.
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
}

void MovementAnalyzer::AnalyzeMovement(movementDataType &dic)
{
	auto keys = dic.uniqueKeys();
	foreach(auto item, keys)
	{
		//кхм...
		//QFuture<void> future = QtConcurrent::run([=](){AnalyzeSingleUnitMovement(item, dic.value(item));});
		AnalyzeSingleUnitMovement(item, dic.value(item));
	}
}

void MovementAnalyzer::AnalyzeSingleUnitMovement(qlonglong id, QMap<QTime, quint32> dic)
{
	qreal movementTime	= 0;
	//qreal parkingTime	= 24;
	QTime lastMarker, currentMarker;
	lastMarker = QTime::fromString("00:00:00");
	bool wasMoving = false;
	bool isMoving = false;
	while (!dic.isEmpty())
	{
		currentMarker	 = dic.firstKey();
		isMoving = dic.value(currentMarker);
// Если ехали, то
// Прибавляем к времени езды либо от прошлого до текущего, либо от прошлого+2минуты(если > timeout прошло)
// иначе - пофиг, просто записываем точку старта.
		movementTime += wasMoving?
					SexToDec( lastMarker.secsTo( lastMarker.secsTo(currentMarker) < m_timeoutSecond?
												currentMarker : lastMarker.addSecs(m_timeoutSecond) ) ):0;
		lastMarker = currentMarker;
		wasMoving = isMoving;
		dic.remove(currentMarker);

	}
//	 Когда словарь закончился обрабатываем последний временной участок (до 23:59:59)
// Если последняя инфа-"ЕДУ", то +2мин(если в этот день умещается) иначе до конца суток
	movementTime += wasMoving?( SexToDec( lastMarker.secsTo( lastMarker.addSecs(m_timeoutSecond) > lastMarker?
									lastMarker.addSecs(m_timeoutSecond) : QTime::fromString(m_endTime) )) ):0;

// Вставить в словарь новое поле с результатами: ID и длительность движения.
	m_movementStatistic.insert(id, movementTime);
}
