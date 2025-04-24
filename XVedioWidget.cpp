#include "XVedioWidget.h"
#include <Qdebug>
XVedioWidget::XVedioWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{}

XVedioWidget::~XVedioWidget()
{}

void XVedioWidget::paintGL()
{
	qDebug() << "paintGL";
}

void XVedioWidget::initializeGL()
{
	qDebug() << "initializeGL";
}

void XVedioWidget::resizeGL(int width, int height)
{
	qDebug() << "resizeGL" << width << ":" << height;
}
