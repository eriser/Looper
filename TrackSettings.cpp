#include "TrackSettings.hpp"
#include "ui_TrackSettings.h"

TrackSettings::TrackSettings(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::TrackSettings) {

	ui->setupUi(this);

}

TrackSettings::~TrackSettings() {

	delete ui;

}
