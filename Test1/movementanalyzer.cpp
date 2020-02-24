#include "movementanalyzer.h"
#include <QMap>
#include <QTextCodec>
#include <QPair>
MovementAnalyzer::MovementAnalyzer()
{

}



void MovementAnalyzer::MakeFileAnalyze(const QString &sourceCSV, const QString &resultTXT)
{
	m_resultFile = resultTXT;

// открыть файл CSV
// прочесть и положить в удобный вид
	m_csvParser.Parse(sourceCSV, m_csvParsedLines);

	InitializeDataDictionary();	// Заполняем m_movementDataDic
	AnalyzeMovement(m_movementDataDic);
	MakeReport(m_movementStatistic);



	qDebug() << "CSV parsed lines count: " << m_csvParsedLines.size();
	qDebug() << "Lines skipped (total): "<< m_skippedLines.size();
// посчитать времена
	// concurrentRun?
// вывести в файл TXT


}

void MovementAnalyzer::MakeReport(const reportType& dataToExport)
{
	// TODO: Русский текст выводить нужно
	QFile result(m_resultFile);
	QTextStream out(&result);
	result.open(QIODevice::WriteOnly | QIODevice::Text);
	out << m_reportSeparator << endl;
	auto it = dataToExport.begin();
	while (it != dataToExport.end())
	{
		out << "id: " << it.key()<< endl;
		out << "riding time: "	<< it.value() << "h" << endl;
		out << "standby time: " << 24 - it.value() << "h" << endl;
		out << m_reportSeparator << endl;
		it++;
	}

	qDebug()<< "skipped lines: "<<m_skippedLines.size() << ":";
	while (!m_skippedLines.isEmpty())
	{
		qDebug() << m_skippedLines.takeFirst();
	}
	result.close();

}

qreal MovementAnalyzer::SexToDec(int secs)
{
	qreal out = secs/3600.0;
	return out;
}

void MovementAnalyzer::InitializeDataDictionary()
{
	auto itLine = m_csvParsedLines.begin();
	while (itLine != m_csvParsedLines.end())
	{
		QStringList& currentLine = *itLine;
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
//	TODO: упростить монстра
	while (!dic.isEmpty())
	{
		currentMarker	 = dic.firstKey();
		isMoving = dic.value(currentMarker);
		if (wasMoving)		// ЕСЛИ ЕХАЛИ
		{
			if (isMoving)	// и продолжаем
			{
				if (lastMarker.secsTo(currentMarker) > m_timeoutSecond) // если прошло больше 2х минут
				{

					dic.insert(lastMarker.addSecs(m_timeoutSecond), 0);
				}
				else	// прошло меньше 2х минут, едем дальше
				{
					dic.remove(currentMarker);
				}
			}
			else			// и остановились
			{
				movementTime += SexToDec(lastMarker.secsTo(currentMarker));
				wasMoving = false;
				dic.remove(currentMarker);
			}
		}
		else				// ЕСЛИ СТОЯЛИ
		{
			if (isMoving)	// и сейчас поехали
			{
				lastMarker = currentMarker;
				wasMoving = true;
			}
			dic.remove(currentMarker); // удаляем учтенное поле
		}
	}
	// Когда словарь закончился обрабатываем последний временной участок (до 23:59:59)
	if (wasMoving)
	{
		if (lastMarker.addSecs(m_timeoutSecond) > lastMarker)	// если через <2 мин полночь.. ) сбросится время.
		{
			movementTime += SexToDec(lastMarker.secsTo(lastMarker.addSecs(120)));
		}
		else
			movementTime += SexToDec(lastMarker.secsTo(QTime::fromString(m_endTime)));

	}
	// Вставить в словарь новое поле с результатами: ID и длительность движения.
	m_movementStatistic.insert(id, movementTime);
}
