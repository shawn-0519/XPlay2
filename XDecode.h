#pragma once
#include <mutex>
struct AVCodecParameters;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;

class XDecode
{
public:

	//�򿪽�����,���ܳɹ�����ͷ�para�ռ�
	virtual bool Open(AVCodecParameters *para);

	//�رս�����
	virtual void Close();

	//�ͷſռ�
	virtual void Clear();

	//���͵������̣߳����ܳɹ�����ͷ�pkt�ռ䣨�����ý�����ݣ�
	virtual bool Send(AVPacket *pkt);

	//��ȡ�������ݣ�һ��send������Ҫ���Recv����ȡ�����е�����Send NULL��Recv���
	//ÿ�θ���һ�ݣ��ɵ������ͷ� av_frame_free
	virtual AVFrame *Recv();

	XDecode();
	virtual ~XDecode();

protected:
	std::mutex mux;
	AVCodecContext* codec = 0;
};

