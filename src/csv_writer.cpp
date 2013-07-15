#include "csv_writer.h"

#include <QChar>
#include <QIODevice>
#include <QFile>
#include <QTextStream>

#include <iostream>

CsvWriter::CsvWriter(QString fileName) : _fileName(fileName)
{	
	_content = QString();
}

QString CsvWriter::insert(unsigned long int x, unsigned long int y, QString str)
{
	QString::iterator it = _content.begin(), it0 = it, it1 = it;
	unsigned long int posit1 = 0, posit0 = 0, posit = 0;
	unsigned long int count = 0;
	QString caseContent = QString();

	// We get the line
	
	while(it!=_content.end() && count!=y)
	{
		if(*it==QChar('\n'))
		{
			++count;
			if(count!=y)
			{
				it0 = it;
				posit0 = posit;
			}
		}
		if(count!=y)
		{
			++posit;
			++it;
		}
	}

	it1 = it;
	posit1 = posit;

	// Are there enough lines?
	
	if(count+1<y)
	{
		while(count+1!=y)
		{
			_content.append(QChar('\n'));
			++count;
		}
		it0 = _content.end()-1;
		posit0 = _content.size()-1;
		it1 = _content.end();
		posit1 = _content.size();
	}

	count = 0;

	// We get the column
	
	if(count!=x)
	{
		it = it0;
		posit = posit0;
		while(it != it1 && count!=x)
		{
			if(*it==QChar(','))
			{
				++count;
				if(count!=x)
				{
					it0=it;
					posit0 = posit;
				}
			}
			if(count!=x)
			{
				++it;
				++posit;
			}
		}
	}
	else
	{
		it = it1;
		posit = posit1;
	}

	it1 = it;
	posit1 = posit;

	// Are there enough columns?
	
	if(count+1<x)
	{
		while(count+1!=x)
		{
			_content.insert(posit1, QChar(','));
			++it1;
			++posit1;
			++count;
		}
	}


	else
	{
		if(posit1-posit0!=1)
		{
			caseContent = _content.section('\n',y-1,y-1).section(',',x-1,x-1);
			_content.remove(it0-_content.begin(),posit1-posit0);
			it1 = it0;
			posit1 = posit0;
		}
	}

	if(it1==_content.end())
		_content.append(str);
	else
		_content.insert(posit1, str);

	return caseContent;
}

QString CsvWriter::data()
{
	return _content;
}

void CsvWriter::update()
{
	QFile file(_fileName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out<<_content;
	file.close();
}
