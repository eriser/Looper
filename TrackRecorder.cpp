#include "TrackRecorder.hpp"
#include <QDir>
#include <iostream>

TrackRecorder::TrackRecorder() : auto_start(false), auto_stop(false) {

	this->rec = new QAudioRecorder( this );

}

TrackRecorder::~TrackRecorder() {

	delete this->rec;

}

void TrackRecorder::start() {

	std::cout << "Recording started" << std::endl;

	QAudioEncoderSettings settings;
	settings.setCodec( "audio/pcm" );
	settings.setSampleRate( 44100 );
	settings.setBitRate( 64000 );
	settings.setQuality( QMultimedia::EncodingQuality( QMultimedia::NormalQuality ) );
	settings.setEncodingMode( QMultimedia::ConstantQualityEncoding );

	std::cout << "dev" << std::endl;
	foreach( const QString &device, this->rec->audioInputs() ) {
		std::cout << device.toStdString() << std::endl;
	}

	std::cout << "codec" << std::endl;
	foreach( const QString &codecName, this->rec->supportedAudioCodecs() ) {
		std::cout << codecName.toStdString() << std::endl;
	}

	std::cout << "container" << std::endl;
	foreach( const QString &containerName, this->rec->supportedContainers() ) {
		std::cout << containerName.toStdString() << std::endl;
	}

	std::cout << "sample" << std::endl;
	foreach( int sampleRate, this->rec->supportedAudioSampleRates() ) {
		std::cout << sampleRate << std::endl;
	}

	QString container = "audio/x-wav";
	this->rec->setEncodingSettings( settings, QVideoEncoderSettings(), container );
	this->rec->setAudioInput( "Integrated Microphone Array (ID" );

	this->rec->setOutputLocation( QUrl::fromLocalFile( this->dest_file_path ) );

	connect( this->rec, SIGNAL( durationChanged( qint64 ) ), this, SLOT( update_progress( qint64 ) ) );
	connect( this->rec, SIGNAL( stateChanged( QMediaRecorder::State ) ),
			this, SLOT( rec_state_changed( QMediaRecorder::State ) ) );
	connect( this->rec, SIGNAL( error( QMediaRecorder::Error ) ),
			this, SLOT( rec_error() ) );

	this->rec->record();

}

void TrackRecorder::stop() {

	// Stop recording
	std::cout << "recording stopped" << std::endl;

	if( this->auto_start ) {
		// cut start until sound
	}

	if( this->auto_stop ) {
		// cut end until sound
	}

	this->rec->stop();

	QMediaPlayer *player = new QMediaPlayer(this);
	player->setVolume(100);

	player->setMedia( QUrl::fromLocalFile( this->dest_file_path ) );
	player->play();

	emit finished( this->dest_file_path );

}

void TrackRecorder::update_progress( qint64 progress ) {

	std::cout << "Progress: " << progress << std::endl;

}

void TrackRecorder::rec_state_changed( QMediaRecorder::State new_state ) {

	std::cout << "rec state changed" << std::endl;
	switch( new_state ) {
		case QMediaRecorder::RecordingState:
			std::cout << "state: recording" << std::endl;
			break;
		case QMediaRecorder::PausedState:
			std::cout << "state: paused" << std::endl;
			break;
		case QMediaRecorder::StoppedState:
			std::cout << "state: stopped" << std::endl;
			break;
	}

}

void TrackRecorder::rec_error() {

	std::cout << "ERROR: " << this->rec->errorString().toStdString() << std::endl;

}


void TrackRecorder::set_dest_file( QString path ) {

	this->dest_file_path = path;

}

void TrackRecorder::set_max_silence_time( int max_silence_sec ) {

}

void TrackRecorder::set_auto_start( bool yesno ) {

}

void TrackRecorder::set_auto_stop( bool yesno ) {

}
