#pragma once
struct AVCodecParameters;
struct SwrContext;
struct AVFrame;

#include <mutex>
class XResample
{
public:
	//����������������һ�³��˲�����ʽ�����ΪS16 ,���ͷ�para
	virtual bool Open(AVCodecParameters* para);

	virtual void Close();

	//�����ز������С,���ܳɹ�����ͷ�indata�ռ�
	virtual int ReSample(AVFrame *indata, unsigned char *outdata);



	XResample();
	virtual ~XResample();


	//AV_SAMPLE_FMT_S16
	int outFormat = 1;

protected:
	SwrContext* actx = 0;
	std::mutex mux;

};

