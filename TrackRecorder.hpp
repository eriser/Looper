#ifndef __TRACKRECORDER_HPP__
#define __TRACKRECORDER_HPP__

#include <QtMultimedia>
#include <QTemporaryFile>
#include <QSharedPointer>
#include <QObject>

class TrackRecorder : public QObject {

	Q_OBJECT

	public slots:

		void slot_recording_start();
		void slot_recording_stop();
		void slot_update_progress();

	public:

		typedef QSharedPointer<QTemporaryFile> TmpFilePtr;

		TrackRecorder();
		~TrackRecorder();

		void set_auto_start( bool yesno );
		void set_auto_stop( bool yesno );

		// QVector<QString> get_avail_codecs();
		// ...

		// Cumulative moving average
		static void cmov_avg( unsigned short int *out_avg, unsigned short int new_sample, int sample_count );

	private:

		bool sound_reached( unsigned short int *buffer, int buffer_size );

		bool auto_start;
		bool auto_stop;

		QTimer track_length_timer;

		QAudioRecorder *rec;
		QAudioInput *audio;
		QTemporaryFile *track_file;
		int track_length_sec;

	signals:

		void sig_finished( TrackRecorder::TmpFilePtr file_path, quint64 start_offset, quint64 end_offset );


};

#endif
