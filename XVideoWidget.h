#pragma once
#include <mutex>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "IVideoCall.h"
struct AVFrame;
class XVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions, public IVideoCall
{
	Q_OBJECT

public:

	//窗口初始化
	void Init(int width,int height);

	//不管成功与否都释放frame空间
	virtual void Repaint(AVFrame* frame);

	XVideoWidget(QWidget *parent);
	~XVideoWidget();

protected:
	//刷新显示
	void paintGL();
	//初始化gl
	void initializeGL();
	//窗口初始化
	void resizeGL(int width, int height);
	
private:
	std::mutex mux;
	//shader程序
	QOpenGLShaderProgram program;

	//shader中yuv变量地址
	GLuint unis[3] = { 0 };
	//openg的 texture地址
	GLuint texs[3] = { 0 };

	//材质内存空间
	unsigned char* datas[3] = { 0 };

	int width = 240;
	int height = 128;

};
