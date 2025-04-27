#pragma once
#include <QThread>
#include <mutex>
struct AVCodecParameters;
class XDecode;
class XAudioPlay;
class XResample;


class XAudioThread:public QThread
{
public:
	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters *para, int SampleRate, int channels);

	//
	void run();

	//
	virtual void Close();

	XAudioThread();
	virtual ~XAudioThread();


protected:
	std::mutex mux;
	XDecode* decode = 0;
	XAudioPlay* ap = 0;
	XResample* res= 0;

};

