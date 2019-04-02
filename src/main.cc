#include "QtGLTestMainWindow.hh"

#include <QApplication>

int main(int argc, char * * argv) {
	QApplication app {argc, argv};
	QtGLTestMainWindow win {};
	win.show();
	return app.exec();
}
