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
	//音频设备
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
		//setSampleFormat替代了 setSampleSize 与 setSampleType

		//fmt.setCodec("audio/pcm");
		//The audio codec is now always "audio/pcm" and cannot be changed
		
		//只支持小端（LittleEndian）字节序，不能再设置为大端或小端。
		//fmt.setByteOrder(QAudioFormat::LittleEndian);
		mux.lock();
		//Qt6 必须传入音频设备和格式，不能只传格式。
		//output = new QAudioOutput(fmt);
		output = new QAudioSink(dev,fmt);
		io = output->start();
		mux.unlock();
		if (io) {
			return true;
		}
		return false;
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
