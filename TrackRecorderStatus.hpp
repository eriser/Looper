#ifndef TRACKRECORDERSTATUS_HPP
#define TRACKRECORDERSTATUS_HPP

#include <QMainWindow>

namespace Ui {
	class TrackRecorderStatus;
}

class TrackRecorderStatus : public QMainWindow {

	Q_OBJECT

	signals:

		void sig_recording_stop();

	public slots:

		void slot_stop_recording_clicked();

		void slot_recording_started();
		void slot_record_status( int seconds_recorded );
		void slot_sound_start_detected();
		void slot_sound_stop_detected();
		void slot_recording_stopped();

	public:

		explicit TrackRecorderStatus(QWidget *parent = 0);
		~TrackRecorderStatus();

	private:

		Ui::TrackRecorderStatus *ui;

};

#endif // TRACKRECORDERSTATUS_HPP
