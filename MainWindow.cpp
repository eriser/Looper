#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QtMultimedia/QMediaPlayer>
#include <QFile>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow( QWidget *parent ) :
	QMainWindow( parent ),
	ui( new Ui::MainWindow ) {

	ui->setupUi(this);

	connect( ui->pushButton, SIGNAL( clicked() ), this, SLOT( whatevz() ) );
	connect( ui->pushButton_2, SIGNAL( clicked() ), this, SLOT( recording_stop() ) );

	connect( this, SIGNAL( sig_recording_stop() ), &this->track_rec, SLOT( stop() ) );

	connect( &this->track_rec, SIGNAL( sig_finished( QString ) ),
			 this, SLOT( slot_recording_finished( QString ) ) );

}

void MainWindow::whatevz() {

	QString fpath;
	fpath = QDir::currentPath();
	fpath += "/test.raw";

	QMediaPlayer *player = new QMediaPlayer(this);
	player->setVolume(50);
	player->setMedia(QUrl::fromLocalFile("C:/ChatIn.wav"));
	// player->play();

	this->track_rec.set_dest_file( fpath );
	this->track_rec.start();

}

void MainWindow::recording_stop() {

	emit sig_recording_stop();

}

void MainWindow::slot_recording_finished( QString file_path ) {

	// QAudio output ...
	std::cout << "Recording ended (" << file_path.toStdString() << ")." << std::endl;

	// Parse file ?)

	f.setFileName( file_path );
	f.open( QIODevice::ReadOnly );

	QAudioFormat format;
	// Set up the format, eg.
	format.setSampleRate( 44100 );
	format.setChannelCount( 1 );
	format.setSampleSize( 16 );
	format.setCodec( "audio/pcm" );
	format.setByteOrder( QAudioFormat::LittleEndian );
	format.setSampleType( QAudioFormat::UnSignedInt );

	QAudioDeviceInfo info( QAudioDeviceInfo::defaultOutputDevice() );
	if( !info.isFormatSupported(format) ) {
		std::cout << "Raw audio format not supported by backend, cannot play audio." << std::endl;
		return;
	}

	audio = new QAudioOutput( format, this );

	audio->start( &f );

}

MainWindow::~MainWindow() {

	delete ui;

}
