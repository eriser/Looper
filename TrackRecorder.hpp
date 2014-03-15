#ifndef __TRACKRECORDER_HPP__
#define __TRACKRECORDER_HPP__

#include <QtMultimedia>
#include <QObject>

class TrackRecorder : public QObject {

	Q_OBJECT

	public slots:

		void start();
		void stop();
		void update_progress( qint64 progress );
		void rec_state_changed( QMediaRecorder::State new_state );
		void rec_error();

	signals:

		void sig_finished( QString file_path );

	public:

		TrackRecorder();
		~TrackRecorder();

		void set_dest_file( QString path );
		void set_max_silence_time( int max_silence_sec );
		void set_auto_start( bool yesno );
		void set_auto_stop( bool yesno );

		// QVector<QString> get_avail_codecs();
		// ...

	private:

		int max_silence_sec;
		QString dest_file_path;
		bool auto_start;
		bool auto_stop;
		QAudioRecorder *rec;
		QAudioInput *audio;
		QFile f;

};

#endif
