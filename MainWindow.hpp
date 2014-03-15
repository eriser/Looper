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

	signals:

		void sig_recording_start();
		void sig_recording_stop();

	private:

		Ui::MainWindow *ui;
		QThread *recorder_thread;
		TrackRecorder track_rec;

};

#endif // MAINWINDOW_HPP
