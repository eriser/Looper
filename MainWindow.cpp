#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QtMultimedia/QMediaPlayer>
#include <QFile>
#include <QMessageBox>
#include <iostream>
#include <cstdio>

MainWindow::MainWindow( QWidget *parent ) :
	QMainWindow( parent ),
	ui( new Ui::MainWindow ) {

	ui->setupUi(this);

	// http://www.qtcentre.org/threads/37856-QtableWidget-With-Pushutton-get-row-error + qsignalmapper
	// ui->tableWidgetTrack->insertRow( 0 );
	// ui->tableWidgetTrack->setCellWidget( 0, 0, new QPushButton( "cokolwiek", this ) );

	connect( ui->pushButton, SIGNAL( clicked() ), this, SLOT( whatevz() ) );
	connect( ui->pushButton_2, SIGNAL( clicked() ), this, SLOT( recording_stop() ) );

	connect( this, SIGNAL( sig_recording_stop() ), &this->track_rec, SLOT( stop() ) );

	connect( &this->track_rec, SIGNAL( sig_finished( QString, quint64, quint64 ) ),
			 this, SLOT( slot_recording_finished( QString, quint64, quint64 ) ) );

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
	this->track_rec.set_auto_start( true );
	this->track_rec.set_auto_stop( true );
	this->track_rec.start();

}

void MainWindow::recording_stop() {

	emit sig_recording_stop();

}

void MainWindow::slot_recording_finished( QString file_path, quint64 start_offset, quint64 end_offset ) {

	// QAudio output ...
	std::cout << "Recording ended (" << file_path.toStdString() << ")." << std::endl;
	std::cout << "Start: " << start_offset << " End: " << end_offset << std::endl;

	FILE *tmp = tmpfile();
	FILE *fp = fopen( file_path.toStdString().c_str(), "rb" );

	fseek( fp, start_offset, SEEK_CUR );
	unsigned short int sample;
	while( !feof( fp ) ) {
		fread( &sample, sizeof( unsigned short int ), 1, fp );
		fwrite( &sample, sizeof( unsigned short int ), 1, tmp );
		if( ftell( fp ) == end_offset ) {
			break;
		}
	}

	fclose( fp );
	remove( file_path.toStdString().c_str() );
	rewind( tmp );

	fp = fopen( file_path.toStdString().c_str(), "wb" );
	while( !feof( tmp ) ) {
		fread( &sample, sizeof( unsigned short int ), 1, tmp );
		fwrite( &sample, sizeof( unsigned short int ), 1, fp );
	}
	fclose( fp );
	fclose( tmp );

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

	connect( audio, SIGNAL( stateChanged(QAudio::State) ),
			 this, SLOT( slot_handle_audio_state(QAudio::State) ) );

	audio->start( &f );

}

void MainWindow::slot_handle_audio_state( QAudio::State new_state ) {

	std::cout << "state changed " << (int)new_state << std::endl;

	if( new_state == QAudio::IdleState ) {

		this->f.close();
		f.open( QIODevice::ReadOnly );

		this->audio->start( &this->f );
	}

}
\
MainWindow::~MainWindow() {

	delete ui;

}
