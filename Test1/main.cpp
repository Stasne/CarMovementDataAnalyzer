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

	MovementAnalyzer analyzer;
	analyzer.makeReport(":/new/prefix1/log.csv", "./result.txt");


	return a.exec();
}
