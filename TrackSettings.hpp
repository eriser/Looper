#ifndef TRACKSETTINGS_HPP
#define TRACKSETTINGS_HPP

#include "TrackRecorder.hpp"
#include <QMainWindow>

namespace Ui {
	class TrackSettings;
}

class TrackSettings : public QMainWindow {

	Q_OBJECT

	public:

		explicit TrackSettings(QWidget *parent = 0);
		~TrackSettings();

	private:

		Ui::TrackSettings *ui;

	private slots:

		// void slot_..._clicked();
		// changed

	signals:

		void sig_settings_finished( TrackRecorder::TmpFilePtr file, quint64 start, quint64 stop );

};

#endif // TRACKSETTINGS_HPP
