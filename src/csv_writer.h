#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <QString>

class CsvWriter
{
	public:

	CsvWriter(QString fileName);
	
	QString insert(unsigned long int x, unsigned long int y, QString str);
	QString get(unsigned long int x, unsigned long int y) const;
	void update();
	QString data();

	private:

	QString _fileName;
	QString _content;
};

#endif
