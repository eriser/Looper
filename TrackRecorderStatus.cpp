#include "TrackRecorderStatus.hpp"
#include "ui_TrackRecorderStatus.h"

TrackRecorderStatus::TrackRecorderStatus(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::TrackRecorderStatus) {

	ui->setupUi(this);

}

TrackRecorderStatus::~TrackRecorderStatus() {

	delete ui;

}

void TrackRecorderStatus::slot_stop_recording_clicked() {

}

void TrackRecorderStatus::slot_recording_started() {

}

void TrackRecorderStatus::slot_record_status( int seconds_recorded ) {

}

void TrackRecorderStatus::slot_sound_start_detected() {

}

void TrackRecorderStatus::slot_sound_stop_detected() {

}

void TrackRecorderStatus::slot_recording_stopped() {

}
