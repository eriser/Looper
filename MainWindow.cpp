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

	// MainWindow GUI to MainWindow signals
	connect( ui->pushButtonStartRecording, SIGNAL( clicked() ),
			 this, SLOT( slot_recording_start_clicked() ) );
	connect( ui->pushButtonStopRecording, SIGNAL( clicked() ),
			 this, SLOT( slot_recording_stop_clicked() ) );

	// MainWindow to TrackRecorder signals
	connect( this, SIGNAL( sig_recording_stop() ),
			 &this->track_rec, SLOT( slot_recording_stop() ) );

	// TrackRecorder to MainWindow signals
	connect( &this->track_rec, SIGNAL( sig_finished( TrackRecorder::TmpFilePtr, quint64, quint64 ) ),
			 this, SLOT( slot_recording_finished( TrackRecorder::TmpFilePtr, quint64, quint64 ) ) );

	// TrackRecorder to TrackRecorderStatus signals
	connect( &this->track_rec, SIGNAL( sig_recording_status(int) ),
			 &this->track_rec_status, SLOT( slot_recording_status(int) ) );

	// TrackRecorderStatus to MainWindow signals
	connect( &this->track_rec_status, SIGNAL( sig_recording_stop() ),
			 this, SLOT( slot_recording_stop_clicked() ) );

}

void MainWindow::slot_recording_start_clicked() {

	/*
	QString fpath;
	fpath = QDir::currentPath();
	fpath += "/test.raw";

	QMediaPlayer *player = new QMediaPlayer(this);
	player->setVolume(50);
	player->setMedia(QUrl::fromLocalFile("C:/ChatIn.wav"));
	player->play();
	*/

	this->track_rec.set_auto_start( true );
	this->track_rec.set_auto_stop( true );
	this->track_rec.slot_recording_start();

	this->track_rec_status.show();

}

void MainWindow::slot_recording_stop_clicked() {

	emit sig_recording_stop();
	this->track_rec_status.hide();

}

void MainWindow::slot_recording_finished( TrackRecorder::TmpFilePtr tmp_file, quint64 start_offset, quint64 end_offset ) {

	// Show TrackSettings window, set file to edit

	// QAudio output ...
	std::cout << "Recording ended (" << tmp_file->fileName().toStdString() << ")." << std::endl;
	std::cout << "Start: " << start_offset << " End: " << end_offset << std::endl;

	FILE *tmp = tmpfile();
	FILE *fp = fopen( tmp_file->fileName().toStdString().c_str(), "rb" );

	fseek( fp, start_offset, SEEK_CUR );
	unsigned short int sample;
	while( !feof( fp ) ) {

		fread( &sample, sizeof( unsigned short int ), 1, fp );
		fwrite( &sample, sizeof( unsigned short int ), 1, tmp );

		if( ftell(fp) != -1 && (unsigned int)ftell( fp ) == end_offset ) {
			break;
		}

	}

	fclose( fp );
	remove( tmp_file->fileName().toStdString().c_str() );
	rewind( tmp );

	fp = fopen( tmp_file->fileName().toStdString().c_str(), "wb" );
	while( !feof( tmp ) ) {
		fread( &sample, sizeof( unsigned short int ), 1, tmp );
		fwrite( &sample, sizeof( unsigned short int ), 1, fp );
	}
	fclose( fp );
	fclose( tmp );

	f.setFileName( tmp_file->fileName() );
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
