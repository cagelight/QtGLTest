#include "QtGLTestMainWindow.hh"
#include "GLTestWidget.hh"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QPushButton>

QtGLTestMainWindow::QtGLTestMainWindow() : QMainWindow() {
	QWidget * center = new QWidget {this};
	QGridLayout * centerLayout = new QGridLayout {center};
	this->setCentralWidget(center);
	
	QWidget * controlPane = new QWidget {center};
	controlPane->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	centerLayout->addWidget(controlPane, 0, 0, 1, 1);
	
	GLTestWidget * glPane = new GLTestWidget {center};
	glPane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	centerLayout->addWidget(glPane, 0, 1, 1, 1);
	
	QVBoxLayout * controlLayout = new QVBoxLayout { controlPane };
	QPushButton * testButton = new QPushButton {"TEST", controlPane};
	controlLayout->addWidget(testButton);
	
	connect(testButton, &QPushButton::pressed, glPane, &GLTestWidget::randomizeColor);
}
