#pragma once
#include <mutex>
struct AVCodecParameters;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;

class XDecode
{
public:

	//打开解码器,不管成功与否都释放para空间
	virtual bool Open(AVCodecParameters *para);

	//关闭解码器
	virtual void Close();

	//释放空间
	virtual void Clear();

	//发送到解码线程，不管成功与否都释放pkt空间（对象和媒体内容）
	virtual bool Send(AVPacket *pkt);

	//获取解码数据，一次send可能需要多次Recv，获取缓冲中的数据Send NULL在Recv多次
	//每次复制一份，由调用者释放 av_frame_free
	virtual AVFrame *Recv();

	XDecode();
	virtual ~XDecode();

protected:
	std::mutex mux;
	AVCodecContext* codec = 0;
};

