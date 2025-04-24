#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
class XVedioWidget  : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	XVedioWidget(QWidget *parent);
	~XVedioWidget();

protected:
	//刷新显示
	void paintGL();
	//初始化gl
	void initializeGL();
	//窗口初始化
	void resizeGL(int width, int height);
	
};
