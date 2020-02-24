#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QDebug>
#include <QHash>
#include "movementanalyzer.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	setlocale(LC_CTYPE, "rus");
	qDebug() << "Русский текст тут есть";

// TODO: Консольное меню
	MovementAnalyzer analyzer;
	// профилирование 2.0
	QTime t1;
	t1.start();
	analyzer.MakeFileAnalyze(":/new/prefix1/log.csv", "./result.txt");
	qDebug() << t1.elapsed() << "ms";



	return a.exec();
}
