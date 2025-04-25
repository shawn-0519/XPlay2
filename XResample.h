#pragma once
struct AVCodecParameters;
struct SwrContext;
struct AVFrame;

#include <mutex>
class XResample
{
public:
	//输出参数和输入参数一致除了采样格式，输出为S16 ,会释放para
	virtual bool Open(AVCodecParameters* para);

	virtual void Close();

	//返回重采样后大小,不管成功与否都释放indata空间
	virtual int ReSample(AVFrame *indata, unsigned char *outdata);



	XResample();
	virtual ~XResample();


	//AV_SAMPLE_FMT_S16
	int outFormat = 1;

protected:
	SwrContext* actx = 0;
	std::mutex mux;

};

