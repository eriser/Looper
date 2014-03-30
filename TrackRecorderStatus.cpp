#include "TrackRecorderStatus.hpp"
#include "ui_TrackRecorderStatus.h"

TrackRecorderStatus::TrackRecorderStatus( QWidget *parent ) :
	QMainWindow( parent ),
	ui( new Ui::TrackRecorderStatus ) {

	ui->setupUi(this);

	this->setWindowFlags( Qt::FramelessWindowHint );

	connect( ui->pushButtonStopRecording, SIGNAL( clicked() ), this, SLOT( slot_stop_recording_clicked() ) );

}

TrackRecorderStatus::~TrackRecorderStatus() {

	delete ui;

}

void TrackRecorderStatus::slot_stop_recording_clicked() {

	emit sig_recording_stop();

}

void TrackRecorderStatus::slot_recording_started() {

}

void TrackRecorderStatus::slot_recording_status( int seconds_recorded ) {

	int hours = ( ( seconds_recorded / 60 ) /60 );
	int minutes = ( seconds_recorded / 60 );
	int seconds = seconds_recorded % 60;

	QString track_len = QString::number( hours );
	track_len += ":";
	track_len += QString("%1").arg( minutes, 2, 10, QChar('0') );
	track_len += ":";
	track_len += QString("%1").arg( seconds, 2, 10, QChar('0') );

	ui->labelTrackLength->setText( track_len );

}

void TrackRecorderStatus::slot_sound_start_detected() {

}

void TrackRecorderStatus::slot_sound_stop_detected() {

}

void TrackRecorderStatus::slot_recording_stopped() {

}
