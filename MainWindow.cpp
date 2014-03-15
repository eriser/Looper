#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QtMultimedia/QMediaPlayer>
#include <iostream>
#include <QMessageBox>

MainWindow::MainWindow( QWidget *parent ) :
	QMainWindow( parent ),
	ui( new Ui::MainWindow ) {

	ui->setupUi(this);

	connect( ui->pushButton, SIGNAL( clicked() ), this, SLOT( whatevz() ) );
	connect( ui->pushButton_2, SIGNAL( clicked() ), this, SLOT( recording_stop() ) );

	connect( this, SIGNAL( sig_recording_stop() ), &this->track_rec, SLOT( stop() ) );

}

void MainWindow::whatevz() {

	QString fpath;
	fpath = QDir::currentPath();
	fpath += "/test.wav";

	this->track_rec.set_dest_file( fpath );
	this->track_rec.start();

	QMediaPlayer *player = new QMediaPlayer(this);
	player->setVolume(50);

	player->setMedia(QUrl::fromLocalFile("C:/ChatIn.wav"));
	player->play();

}

void MainWindow::recording_stop() {

	emit sig_recording_stop();

}

MainWindow::~MainWindow() {

	delete ui;

}
