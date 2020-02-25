#ifndef CSVPARSER_H
#define CSVPARSER_H
#include <QString>
#include <QFile>
#include <QTextStream>

// "Парсер" под разделители отличные от "-", ":", """.
// принимает QString& - путь к файлу для парсинга. и ссылку на QList<QStringList> куда и кладет результат.
class CsvParser
{
public:
	CsvParser(QChar separator = QChar(','));

	void Parse(const QString& csvFilePath, QList<QStringList>&);
private:
	QChar m_separator;
	QFile m_csvFile;
	void inline setFile(const QString& csvFilePath) {m_csvFile.setFileName(csvFilePath);}


};

#endif // CSVPARSER_H
