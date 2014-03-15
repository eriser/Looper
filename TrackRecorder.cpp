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

	/*QAudioEncoderSettings settings;
	settings.setCodec( "audio/pcm" );
	settings.setSampleRate( 44100 );
	settings.setBitRate( 64000 );
	settings.setQuality( QMultimedia::EncodingQuality( QMultimedia::NormalQuality ) );
	settings.setEncodingMode( QMultimedia::ConstantQualityEncoding );
	settings.setChannelCount( 1 );*/

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

	/*
	QString container = "audio/x-raw";
	this->rec->setEncodingSettings( settings, QVideoEncoderSettings(), container );
	this->rec->setAudioInput( "Integrated Microphone Array (ID" );

	this->rec->setOutputLocation( QUrl::fromLocalFile( this->dest_file_path ) );
	*/

	/*
	connect( this->rec, SIGNAL( durationChanged( qint64 ) ), this, SLOT( update_progress( qint64 ) ) );
	connect( this->rec, SIGNAL( stateChanged( QMediaRecorder::State ) ),
			this, SLOT( rec_state_changed( QMediaRecorder::State ) ) );
	connect( this->rec, SIGNAL( error( QMediaRecorder::Error ) ),
			this, SLOT( rec_error() ) );

	this->rec->record();
	*/

	f.setFileName( this->dest_file_path );
	f.open( QIODevice::WriteOnly | QIODevice::Truncate );

	QAudioFormat format;
	// Set up the desired format, for example:
	format.setSampleRate( 44100 );
	format.setChannelCount( 2 );
	format.setSampleSize( 16 );
	format.setCodec("audio/pcm");
	format.setByteOrder( QAudioFormat::LittleEndian );
	format.setSampleType( QAudioFormat::UnSignedInt );

	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if (!info.isFormatSupported(format)) {
		std::cout << "Default format not supported, trying to use the nearest." << std::endl;
		format = info.nearestFormat(format);
	}

	audio = new QAudioInput( format, this );
	// connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

	audio->start( &f );

}

void TrackRecorder::stop() {

	std::cout << "recording stopped" << std::endl;

	if( this->auto_start ) {
		// cut start until sound
	}

	if( this->auto_stop ) {
		// cut end until sound
	}

	this->audio->stop();
	// this->rec->stop();

	QFile newfile;
	QString p;
	p = this->dest_file_path;
	p += "xxx";
	newfile.setFileName( p );\
	newfile.open( QIODevice::WriteOnly | QIODevice::Truncate );

	this->f.close();
	this->f.open( QIODevice::ReadOnly );

	QByteArray qb_arr = this->f.readAll();

	for( QByteArray::iterator it = qb_arr.begin(); it != qb_arr.end(); ++it ) {
		static int two_byte_read = 0;
		static quint32 word = 0;
		if( two_byte_read == 0 ) {
			word = *it;
			two_byte_read = 1;
		} else {
			word |= (*it) << 8;
			std::cout << "Word: " << word << std::endl;
			/*if( word >= 20 ) {
				std::cout << "wieksze" << std::endl;
			} else {
				std::cout << "mniejsze" << std::endl;
			}*/
			word = 0;
			two_byte_read = 0;
		}
	}

	emit sig_finished( p ); // this->dest_file_path );

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
