#pragma once
#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QtCore>
#include <qmessagebox.h>

class CData : public QObject
{
	Q_OBJECT

public:
	CData(QObject *parent = 0);
	~CData();

	int openDatafile(QString fileName_);

	QStringList	getunitList() { return unitList; }
	QStringList getheaderList() { return headerList; }

	QList<QStringList> getdataMatrix() { return dataMatrix; }
	QStringList getTimeStamps() { return timeStamps; }

	QStringList getCollum(int index, QString asUnit = 0);
	//QStringList getCollumRelative(int index, QString asUnit = 0);


private:
	void prozessLine(QString line);
	//QStringList toUnit(QStringList dataList, QString unitFrom, QString unitTo, double multiplier = 1);


	double toTime_t(QString TimeStamp);


	//Attributes
	QString fileName;


	QStringList	unitList;
	QStringList headerList;

	QStringList timeStamps;
	QList<QStringList> dataMatrix;

	int Row;
};

#endif // DATA_H
