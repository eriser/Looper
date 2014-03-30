#include "TrackRecorder.hpp"
#include <QDir>
#include <iostream>
#include <cstdio>

TrackRecorder::TrackRecorder() :
	auto_start(false),
	auto_stop(false),
	track_length_timer(this),
	rec( new QAudioRecorder(this) ),
	track_length_sec(0) {

}

TrackRecorder::~TrackRecorder() {

	delete this->rec;

}

void TrackRecorder::slot_recording_start() {

	std::cout << "Recording started." << std::endl;

	this->track_file = new QTemporaryFile();
	this->track_file->open();

	QAudioFormat format;
	format.setSampleRate( 44100 );
	format.setChannelCount( 1 );
	format.setSampleSize( 16 );
	format.setCodec( "audio/pcm" );
	format.setByteOrder( QAudioFormat::LittleEndian );
	format.setSampleType( QAudioFormat::UnSignedInt );

	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if ( !info.isFormatSupported( format ) ) {
		std::cout << "Default format not supported, trying to use the nearest." << std::endl;
		format = info.nearestFormat( format );
	}

	audio = new QAudioInput( format, this );

	audio->start( this->track_file );

	connect( &this->track_length_timer, SIGNAL( timeout() ), this, SLOT( slot_update_progress() ) );
	this->track_length_timer.start( 1000 );

}

void TrackRecorder::slot_recording_stop() {

	std::cout << "Recording stopped." << std::endl;
	this->track_length_timer.stop();

	this->track_file->close();
	this->audio->stop();

	FILE *raw_audio_file_in = fopen( this->track_file->fileName().toStdString().c_str(), "rb" );

	quint64 audio_start_offset = 0;
	quint64 audio_end_offset = 0;

	// Get size of file.
	fseek( raw_audio_file_in, 0, SEEK_END );
	audio_end_offset = ftell( raw_audio_file_in );
	rewind( raw_audio_file_in );

	// Must be 10 000 - matched to threshold in this->sound_reached()
	#define AVG_WINDOW_MAX_SIZE 10000

	unsigned short int samples_buf[AVG_WINDOW_MAX_SIZE];

	if( this->auto_start ) {

		while( !feof( raw_audio_file_in ) ) {

			fread( samples_buf, sizeof(unsigned short int), AVG_WINDOW_MAX_SIZE, raw_audio_file_in );

			if( this->sound_reached( samples_buf, AVG_WINDOW_MAX_SIZE ) ) {
				audio_start_offset = ftell( raw_audio_file_in ) - sizeof(unsigned short int) * AVG_WINDOW_MAX_SIZE;
				break;
			}

		}

	}

	if( this->auto_stop ) {

		// SEEK_END portability ?
		fseek( raw_audio_file_in, 0, SEEK_END );
		std::cout << "End pos: " << ftell( raw_audio_file_in ) << std::endl;
		fseek( raw_audio_file_in, -AVG_WINDOW_MAX_SIZE * sizeof(unsigned short int), SEEK_CUR );

		while( ftell( raw_audio_file_in ) != 0 ) {

			if( ferror( raw_audio_file_in ) != 0 ) {
				std::cout << "File error ! " << std::endl;
				break;
			}

			fread( samples_buf, sizeof(unsigned short int), AVG_WINDOW_MAX_SIZE, raw_audio_file_in );

			if( this->sound_reached( samples_buf, AVG_WINDOW_MAX_SIZE ) ) {
				audio_end_offset = ftell( raw_audio_file_in );
				audio_end_offset -= /*2 **/ AVG_WINDOW_MAX_SIZE * ( sizeof(unsigned short int) );
				break;
			}

			// -2: fread moves forward
			fseek( raw_audio_file_in, (-2) * AVG_WINDOW_MAX_SIZE * ( sizeof(unsigned short int) ), SEEK_CUR );

		}

	}

	fclose( raw_audio_file_in );

	emit sig_finished( TrackRecorder::TmpFilePtr(this->track_file), audio_start_offset, audio_end_offset ); // this->dest_file_path );

	// Yield shared pointer.
	this->track_file = NULL;

}

void TrackRecorder::slot_update_progress() {

	++this->track_length_sec;
	std::cout << "Progress: " << this->track_length_sec << " secs" << std::endl;

}

void TrackRecorder::set_auto_start( bool yesno ) {

	this->auto_start = yesno;

}

void TrackRecorder::set_auto_stop( bool yesno ) {

	this->auto_stop = yesno;

}

void TrackRecorder::cmov_avg( unsigned short int *out_avg, unsigned short int new_sample, int sample_count ) {

	*out_avg = ( new_sample + sample_count * (*out_avg) ) / ( sample_count + 1 );

}

bool TrackRecorder::sound_reached( unsigned short int *buffer, int buffer_size ) {

	unsigned short int average = 0;

	for( int i = 0; i < buffer_size; ++i ) {
		if( buffer[i] < 65200 ) {
			this->cmov_avg( &average, buffer[i], i );
		}
	}

	std::cout << "Avg: " << average << std::endl;

	// Average over thresh: sound detected
	// Threshold matched to buffer size in this->stop()
	if( average > 25000 ) {
		return true;
	}

	return false;

}
