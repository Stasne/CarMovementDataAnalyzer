#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <iostream>
#include <QTextStream>
#include "movementanalyzer.h"

QTextStream cin(stdin);
QTextStream cout(stdout);

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	setlocale(LC_CTYPE, "rus");
	MovementAnalyzer analyzer;
	quint32 cmd = 2;
	QString srcPath = "./log.csv";
	QString resultPath = "./result.txt";
	QTextStream in;

	while (cmd != 0)
	{
		switch (cmd)
		{
		case 0:
			a.exit();
		break;

		default:
		case 2:	 // MENU
			cout << "0 - Nothing" << endl;
			cout << "1 - Enter new file" << endl;
			cin >> cmd;
			if (cmd > 1) cmd = 2;

		break;
		case 1:	// choose file dialog
			cout << "Full path to log.csv (X:/../log.csv)" << endl;
			do
			{

				srcPath = cin.readLine();
			}while (!QFile::exists(srcPath));
			cout << "Result path (X:/../result.txt)" << endl;
			resultPath = cin.readLine();
			cmd = 3;

		break;
		case 3:		//pureWork
			// профилирование 2.0
			QTime t1;
			t1.start();
			analyzer.MakeFileAnalyze(srcPath, resultPath);
			qDebug() << t1.elapsed() << "ms";
			cmd = 2;
		break;
		}

	}


	return a.exec();
}
