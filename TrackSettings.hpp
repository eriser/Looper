#ifndef TRACKSETTINGS_HPP
#define TRACKSETTINGS_HPP

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

};

#endif // TRACKSETTINGS_HPP
