#pragma once
#include <mutex>
struct AVCodecParameters;
struct AVCodecContext;

class XDecodec
{
public:

	//�򿪽�����,���ܳɹ�����ͷ�para�ռ�
	virtual bool Open(AVCodecParameters *para);

	//�رս�����
	virtual void Close();

	//�ͷſռ�
	virtual void Clear();

	XDecodec();
	virtual ~XDecodec();

protected:
	std::mutex mux;
	AVCodecContext* codec = nullptr;
};

