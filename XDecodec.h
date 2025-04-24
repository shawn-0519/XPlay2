#pragma once
#include <mutex>
struct AVCodecParameters;
struct AVCodecContext;

class XDecodec
{
public:

	//打开解码器,不管成功与否都释放para空间
	virtual bool Open(AVCodecParameters *para);

	//关闭解码器
	virtual void Close();

	//释放空间
	virtual void Clear();

	XDecodec();
	virtual ~XDecodec();

protected:
	std::mutex mux;
	AVCodecContext* codec = nullptr;
};

