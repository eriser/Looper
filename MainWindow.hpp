#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "TrackRecorder.hpp"
#include "TrackRecorderStatus.hpp"
#include <QMainWindow>
#include <QMediaRecorder>
#include <QAudioRecorder>
#include <QVector>
#include <QTemporaryFile>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {

	Q_OBJECT

	public:

		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:

		void slot_recording_start_clicked();
		void slot_recording_stop_clicked();
		void slot_recording_finished( TrackRecorder::TmpFilePtr tmp_file, quint64 start_offset, quint64 end_offset );
		void slot_handle_audio_state( QAudio::State new_state );

	signals:

		void sig_recording_start();
		void sig_recording_stop();

	private:

		Ui::MainWindow *ui;
		TrackRecorder track_rec;
		TrackRecorderStatus track_rec_status;
		QAudioOutput *audio;
		QFile f;

		QVector<TrackRecorder::TmpFilePtr> recorded_tracks; // + int start; int stop

};

#endif // MAINWINDOW_HPP
