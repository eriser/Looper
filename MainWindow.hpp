#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "TrackRecorder.hpp"
#include <QMainWindow>
#include <QMediaRecorder>
#include <QAudioRecorder>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {

	Q_OBJECT

	public:

		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:

		void whatevz();
		void recording_stop();
		void slot_recording_finished( QString file_path );

	signals:

		void sig_recording_start();
		void sig_recording_stop();

	private:

		Ui::MainWindow *ui;
		TrackRecorder track_rec;
		QAudioOutput *audio;
		QFile f;

};

#endif // MAINWINDOW_HPP
