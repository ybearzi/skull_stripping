#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
	void browseInputT1();
	void browseInputT2();
	void browseLabel();
	void browseOutputT1();
	void browseOutputT2();
	void browseOutputExcel();

	void changedInputT1();
	void changedInputT2();
	void changedLabel();
	void changedOutputT1();
	void changedOutputT2();
	void changedOutputExcel();

	void execute();

private:
    Ui::MainWindow *ui;

	QWidget *_centralWidget;
	QLabel *_inputT1, *_inputT2, *_label, *_outputT1, *_outputT2, *_outputExcel;
	QPushButton *_browseInputT1, *_browseInputT2, *_browseLabel, *_browseOutputT1, *_browseOutputT2, *_browseOutputExcel, *_execute;
	QTextEdit *_editInputT1, *_editInputT2, *_editLabel, *_editOutputT1, *_editOutputT2, *_editOutputExcel;
	QHBoxLayout *_layoutInputT1, *_layoutInputT2, *_layoutLabel, *_layoutOutputT1, *_layoutOutputT2, *_layoutOutputExcel;
	QVBoxLayout *_layout;

	QString _strinputT1, _strinputT2, _strlabel, _stroutputT1, _stroutputT2, _stroutputexcel;

	void createCommon();
    void createWidgets();
	void createSlots();

	void openFile(QString& path);
	void saveFile(QString& path);
	void saveCsvFile(QString& path);

};

#endif // MAINWINDOW_H
