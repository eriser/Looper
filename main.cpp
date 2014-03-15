#include "MainWindow.hpp"
#include <QApplication>
#include <QtMultimedia>

/*
 * http://qt-project.org/doc/qt-5.0/qtmultimedia/multimedia-audiorecorder.html
*/
int main( int argc, char **argv ) {

	QApplication a( argc, argv );
	MainWindow w;
	w.show();

	return a.exec();

}
