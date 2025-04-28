#pragma once
class XAudioPlay
{
public:
	//打开音频播放	纯虚函数
	virtual bool Open() = 0;

	virtual void Close() = 0;

	//播放音频
	virtual bool Write(const unsigned char* data, int datasize) = 0;
	virtual int GetFree() = 0;

	//实现单例模式 ，确保整个程序中只有一个 CXAudioPlay 对象实例
	static XAudioPlay* Get();
	
	XAudioPlay();
	virtual ~XAudioPlay();


	int sampleRate = 44100;
	//int sampleSize = 16;
	int channels = 2;

};

