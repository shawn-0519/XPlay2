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
	//ˢ����ʾ
	void paintGL();
	//��ʼ��gl
	void initializeGL();
	//���ڳ�ʼ��
	void resizeGL(int width, int height);
	
};
