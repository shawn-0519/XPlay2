#pragma once
#include <mutex>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
struct AVFrame;
class XVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:

	//���ڳ�ʼ��
	void Init(int width,int height);

	//���ܳɹ�����ͷ�frame�ռ�
	virtual void Repaint(AVFrame* frame);

	XVideoWidget(QWidget *parent);
	~XVideoWidget();

protected:
	//ˢ����ʾ
	void paintGL();
	//��ʼ��gl
	void initializeGL();
	//���ڳ�ʼ��
	void resizeGL(int width, int height);
	
private:
	std::mutex mux;
	//shader����
	QOpenGLShaderProgram program;

	//shader��yuv������ַ
	GLuint unis[3] = { 0 };
	//openg�� texture��ַ
	GLuint texs[3] = { 0 };

	//�����ڴ�ռ�
	unsigned char* datas[3] = { 0 };

	int width = 240;
	int height = 128;

};
