#pragma once

#include <array>
#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QTimer>

struct GLTestWidget : public QOpenGLWidget, private QOpenGLFunctions_4_5_Core {
	Q_OBJECT
public:
	GLTestWidget(QWidget * parent);
	~GLTestWidget();
	
protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
	
public slots:
	void randomizeColor();
	
private:
	struct Trunk;
	std::unique_ptr<Trunk> trunk;
	
	QTimer * frameMaster;
};
