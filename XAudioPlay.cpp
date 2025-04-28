#include "XAudioPlay.h"
#include <QAudioFormat>
#include <QAudioSink>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QAudioDevice>
#include <mutex>
class CXAudioPlay :public XAudioPlay {
public:

	QAudioSink* output = nullptr;
	QIODevice* io = nullptr;
	//��Ƶ�豸
	QAudioDevice dev = QMediaDevices::defaultAudioOutput();
	std::mutex mux;

	virtual void Close()
	{
		mux.lock();
		if (io) {
			io->close();
			io = nullptr;
		}
		if (output) {
			output->stop();
			delete output;;
			output = 0;
		}
		mux.unlock();
	}

	virtual bool Open()
	{
		Close();
		QAudioFormat fmt;
		fmt.setSampleRate(sampleRate);
		fmt.setSampleFormat(QAudioFormat::Int16);
		fmt.setChannelCount(channels);

		//fmt.setSampleSize(sampleSize);
		//setSampleFormat����� setSampleSize �� setSampleType

		//fmt.setCodec("audio/pcm");
		//The audio codec is now always "audio/pcm" and cannot be changed
		
		//ֻ֧��С�ˣ�LittleEndian���ֽ��򣬲���������Ϊ��˻�С�ˡ�
		//fmt.setByteOrder(QAudioFormat::LittleEndian);
		mux.lock();
		//Qt6 ���봫����Ƶ�豸�͸�ʽ������ֻ����ʽ��
		//output = new QAudioOutput(fmt);
		output = new QAudioSink(dev,fmt);
		io = output->start();
		mux.unlock();
		if (io) {
			return true;
		}
		return false;
	}
	virtual bool Write(const unsigned char* data, int datasize)
	{
		if (!data || datasize <= 0)return false;
		mux.lock();
		if (!output || !io)
		{
			mux.unlock();
			return false;
		}
		int size = io->write((char*)data, datasize);
		mux.unlock();
		if (datasize != size)
			return false;
		return true;
	}

	virtual int GetFree()
	{
		mux.lock();
		if (!output)
		{
			mux.unlock();
			return 0;
		}
		int free = output->bytesFree();
		mux.unlock();
		return free;
	}

};


XAudioPlay *XAudioPlay::Get()
{
	static CXAudioPlay play;
	return &play;
}

XAudioPlay::XAudioPlay()
{
}

XAudioPlay::~XAudioPlay()
{
}
