#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "strip.h"

#include <iostream>

#include <QFontMetrics>
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	createCommon();
    createWidgets();
	createSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createCommon()
{
	_strinputT1 = QString();
	_strinputT2 = QString();
	_strlabel = QString();
	_stroutputT1 = QString("outputT1.nrrd");
	_stroutputT2 = QString("outputT2.nrrd");
	_stroutputexcel = QString("output.csv");
}

void MainWindow::createWidgets()
{
	// Initialisation

    _centralWidget = new QWidget(this);
	_inputT1 = new QLabel("Input T1 file", this);
	_inputT2 = new QLabel("Input T2 file", this);
	_label = new QLabel("Label file", this);
	_outputT1 = new QLabel("Output T1 file", this);
	_outputT2 = new QLabel("Output T2 file", this);
	_outputExcel = new QLabel("Output Excel file", this);
	_browseInputT1 = new QPushButton("Browse", this);
	_browseInputT2 = new QPushButton("Browse", this);
	_browseLabel = new QPushButton("Browse", this);
	_browseOutputT1 = new QPushButton("Browse", this);
	_browseOutputT2 = new QPushButton("Browse", this);
	_browseOutputExcel = new QPushButton("Browse", this);
	_execute = new QPushButton("Execute", this);
	_editInputT1 = new QTextEdit(this);
	_editInputT2 = new QTextEdit(this);
	_editLabel = new QTextEdit(this);
	_editOutputT1 = new QTextEdit(_stroutputT1, this);
	_editOutputT2 = new QTextEdit(_stroutputT2, this);
	_editOutputExcel = new QTextEdit(_stroutputexcel, this);
	_layoutInputT1 = new QHBoxLayout;
	_layoutInputT2 = new QHBoxLayout;
	_layoutLabel = new QHBoxLayout;
	_layoutOutputT1 = new QHBoxLayout;
	_layoutOutputT2 = new QHBoxLayout;
	_layoutOutputExcel = new QHBoxLayout;
	_layout = new QVBoxLayout;

	// Central Widget

    setCentralWidget(_centralWidget);

	// Text edit maximum size
	
	_editInputT1->setMaximumHeight(2*QFontMetrics(_editInputT1->currentFont()).height());
	_editInputT2->setMaximumHeight(2*QFontMetrics(_editInputT2->currentFont()).height());
	_editLabel->setMaximumHeight(2*QFontMetrics(_editLabel->currentFont()).height());
	_editOutputT1->setMaximumHeight(2*QFontMetrics(_editOutputT1->currentFont()).height());
	_editOutputT2->setMaximumHeight(2*QFontMetrics(_editOutputT2->currentFont()).height());
	_editOutputExcel->setMaximumHeight(2*QFontMetrics(_editOutputExcel->currentFont()).height());

	// Enable tab changing focus in edit text
	
	_editInputT1->setTabChangesFocus(true);
	_editInputT2->setTabChangesFocus(true);
	_editLabel->setTabChangesFocus(true);
	_editOutputT1->setTabChangesFocus(true);
	_editOutputT2->setTabChangesFocus(true);
	_editOutputExcel->setTabChangesFocus(true);

	// Layout setting
	
	_layoutInputT1->addWidget(_inputT1);
	_layoutInputT1->addWidget(_editInputT1);
	_layoutInputT1->addWidget(_browseInputT1);
	_layoutInputT2->addWidget(_inputT2);
	_layoutInputT2->addWidget(_editInputT2);
	_layoutInputT2->addWidget(_browseInputT2);
	_layoutLabel->addWidget(_label);
	_layoutLabel->addWidget(_editLabel);
	_layoutLabel->addWidget(_browseLabel);
	_layoutOutputT1->addWidget(_outputT1);
	_layoutOutputT1->addWidget(_editOutputT1);
	_layoutOutputT1->addWidget(_browseOutputT1);
	_layoutOutputT2->addWidget(_outputT2);
	_layoutOutputT2->addWidget(_editOutputT2);
	_layoutOutputT2->addWidget(_browseOutputT2);
	_layoutOutputExcel->addWidget(_outputExcel);
	_layoutOutputExcel->addWidget(_editOutputExcel);
	_layoutOutputExcel->addWidget(_browseOutputExcel);

	_layout->addLayout(_layoutInputT1);
	_layout->addLayout(_layoutInputT2);
	_layout->addLayout(_layoutLabel);
	_layout->addLayout(_layoutOutputT1);
	_layout->addLayout(_layoutOutputT2);
	_layout->addLayout(_layoutOutputExcel);
	_layout->addWidget(_execute);

	_centralWidget->setLayout(_layout);
}

void MainWindow::createSlots()
{
	// Connecting buttons

	connect(_browseInputT1, SIGNAL(clicked()), SLOT(browseInputT1()));
	connect(_browseInputT2, SIGNAL(clicked()), SLOT(browseInputT2()));
	connect(_browseLabel, SIGNAL(clicked()), SLOT(browseLabel()));
	connect(_browseOutputT1, SIGNAL(clicked()), SLOT(browseOutputT1()));
	connect(_browseOutputT2, SIGNAL(clicked()), SLOT(browseOutputT2()));
	connect(_browseOutputExcel, SIGNAL(clicked()), SLOT(browseOutputExcel()));
	connect(_execute, SIGNAL(clicked()), SLOT(execute()));

	// Connecting text changes
	
	connect(_editInputT1, SIGNAL(textChanged()), SLOT(changedInputT1()));
	connect(_editInputT2, SIGNAL(textChanged()), SLOT(changedInputT2()));
	connect(_editLabel, SIGNAL(textChanged()), SLOT(changedLabel()));
	connect(_editOutputT1, SIGNAL(textChanged()), SLOT(changedOutputT1()));
	connect(_editOutputT2, SIGNAL(textChanged()), SLOT(changedOutputT2()));
	connect(_editOutputExcel, SIGNAL(textChanged()), SLOT(changedOutputExcel()));
}

void MainWindow::browseInputT1()
{
	openFile(_strinputT1);
	_editInputT1->setText(_strinputT1);
}

void MainWindow::browseInputT2()
{
	openFile(_strinputT2);
	_editInputT2->setText(_strinputT2);
}

void MainWindow::browseLabel()
{
	openFile(_strlabel);
	_editLabel->setText(_strlabel);
}

void MainWindow::browseOutputT1()
{
	saveFile(_stroutputT1);
	_editOutputT1->setText(_stroutputT1);
}

void MainWindow::browseOutputT2()
{
	saveFile(_stroutputT2);
	_editOutputT2->setText(_stroutputT2);
}

void MainWindow::browseOutputExcel()
{
	saveFile(_stroutputexcel);
	_editOutputExcel->setText(_stroutputexcel);
}

void MainWindow::openFile(QString& path)
{
	path = QFileDialog::getOpenFileName(this, tr("Choose a file"), path, tr("nrrd files (*nrrd);;gipl.gz files (*gipl.gz)"));
}

void MainWindow::saveFile(QString& path)
{
	path = QFileDialog::getSaveFileName(this, tr("Create a file"), path, tr("nrrd files (*nrrd);;gipl.gz files (*gipl.gz)"));
}

void MainWindow::saveCsvFile(QString& path)
{
	path = QFileDialog::getSaveFileName(this, tr("Create a file"), path, tr("csv files (*csv)"));
}

void MainWindow::execute()
{
	// Check if files acually exist

	if(QFile(_strinputT1).exists() && QFile(_strinputT2).exists() && QFile(_strlabel).exists() 
			&& (QString(".nrrd")==_strinputT1.rightRef(5)
				|| QString(".gipl.gz")==_strinputT1.rightRef(8))
			&& (QString(".nrrd")==_strinputT2.rightRef(5)
				|| QString(".gipl.gz")==_strinputT2.rightRef(8))
			&& (QString(".nrrd")==_strlabel.rightRef(5)
				|| QString(".gipl.gz")==_strlabel.rightRef(8))
			&& (QString(".nrrd")==_stroutputT1.rightRef(5)
				|| QString(".gipl.gz")==_stroutputT1.rightRef(8))
			&& (QString(".nrrd")==_stroutputT2.rightRef(5)
				|| QString(".gipl.gz")==_stroutputT2.rightRef(8))
			&& QString(".csv")==_stroutputexcel.rightRef(4))
		strip(_strinputT1.toStdString().c_str(), _strinputT2.toStdString().c_str(), _strlabel.toStdString().c_str(), _stroutputT1.toStdString().c_str(), _stroutputT2.toStdString().c_str(), _stroutputexcel.toStdString().c_str());
}

void MainWindow::changedInputT1()
{
	_strinputT1 = _editInputT1->toPlainText();
}

void MainWindow::changedInputT2()
{
	_strinputT2 = _editInputT2->toPlainText();
}

void MainWindow::changedLabel()
{
	_strlabel = _editLabel->toPlainText();
}

void MainWindow::changedOutputT1()
{
	_stroutputT1 = _editOutputT1->toPlainText();
}

void MainWindow::changedOutputT2()
{
	_stroutputT2 = _editOutputT2->toPlainText();
}

void MainWindow::changedOutputExcel()
{
	_stroutputexcel = _editOutputExcel->toPlainText();
}
