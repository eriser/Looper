#include "TrackRecorder.hpp"
#include <QDir>
#include <iostream>
#include <cstdio>

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
	format.setChannelCount( 1 );
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

	this->f.close();

	if( this->auto_start ) {
		// cut start until sound
	}

	if( this->auto_stop ) {
		// cut end until sound
	}

	this->audio->stop();
	// this->rec->stop();

	FILE *raw_audio_file_in;
	FILE *raw_audio_file_out;

	QString p;
	p = this->dest_file_path;
	p += "xxx";

	raw_audio_file_in = fopen( this->dest_file_path.toStdString().c_str(), "rb" );
	raw_audio_file_out = fopen( p.toStdString().c_str(), "wb" );

	// one second average sound
	// 44100
	#define AVG_WINDOW_MAX_SIZE 10000 // 100000

	unsigned short int avg_window[AVG_WINDOW_MAX_SIZE];
	int avg_window_index = 0;
	unsigned short int avg_sample = 0;

	unsigned short int audio_sample = 0;
	bool start_detected_flag = false;
	while( !feof( raw_audio_file_in ) ) {

		fread( &audio_sample, sizeof(unsigned short int), 1, raw_audio_file_in );
		// std::cout << "audio sample: " << audio_sample << std::endl;

		if( !start_detected_flag ) {

			if( avg_window_index == AVG_WINDOW_MAX_SIZE ) {
				// Check if average: silence/sound
				std::cout << "Avg sample: " << avg_sample << std::endl;
				if( avg_sample > 20000 ) {
					// Detected sound
					fwrite( avg_window, sizeof(unsigned short int), avg_window_index, raw_audio_file_out );
					start_detected_flag = true;
				}
				avg_sample = 0;
				avg_window_index = 0;
			} else {
				// std::cout << "Computing avg; sample: " << audio_sample << "Index: " <<
				//			 avg_window_index << std::endl;
				avg_window[avg_window_index++] = audio_sample;
				if( audio_sample < 65200 ) {
					this->cmov_avg( &avg_sample, audio_sample, avg_window_index );
				}
			}

		} else {

			fwrite( &audio_sample, sizeof(unsigned short int), 1, raw_audio_file_out );

		}

	}

	fclose( raw_audio_file_in );
	fclose( raw_audio_file_out );

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

void TrackRecorder::cmov_avg( unsigned short int *out_avg, unsigned short int new_sample, int sample_count ) {

	*out_avg = ( new_sample + sample_count * (*out_avg) ) / ( sample_count + 1 );

}

bool TrackRecorder::sound_reached( unsigned short int *buffer, int buffer_size ) {



}
