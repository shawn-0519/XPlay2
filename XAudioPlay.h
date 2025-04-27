#pragma once
class XAudioPlay
{
public:
	//����Ƶ����	���麯��
	virtual bool Open() = 0;


	virtual void Close() = 0;


	virtual bool Write(const unsigned char *data, int datasize) = 0;

	//���ռ��Ƿ��㹻
	virtual int Getfree() = 0;

	//ʵ�ֵ���ģʽ ��ȷ������������ֻ��һ�� CXAudioPlay ����ʵ��
	static XAudioPlay* Get();
	
	XAudioPlay();
	virtual ~XAudioPlay();


	int sampleRate = 44100;
	//int sampleSize = 16;
	int channels = 2;

};

