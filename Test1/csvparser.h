#ifndef CSVPARSER_H
#define CSVPARSER_H
#include <QString>
#include <QFile>
#include <QTextStream>

class CsvParser
{
public:
	CsvParser(const QString& csvFilePath = QString(""), QChar separator = QChar(','));

	void Parse(const QString& csvFilePath, QList<QStringList>&);
private:
	QChar m_separator;
	QFile m_csvFile;
	void inline setFile(const QString& csvFilePath) {m_csvFile.setFileName(csvFilePath);}


};

#endif // CSVPARSER_H
