/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               创建：夏曹俊，此代码可用作为学习参考                       **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : FFmpeg
** Description   : FFMPEG项目创建示例
** Contact       : xiacaojun@qq.com
**        博客   : http://blog.csdn.net/jiedichina
**		视频课程
**网易云课堂	http://study.163.com/u/xiacaojun
**腾讯课堂		https://jiedi.ke.qq.com/
**csdn学院		http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961
**51cto学院	    http://edu.51cto.com/lecturer/index/user_id-12016059.html
**下载最新的ffmpeg版本 http://www.ffmpeg.club
**
**   ffmpeg+qt播放器 学员群 ：462249121 加入群下载代码和交流
**   微信公众号  : jiedi2007
**		头条号	 : 夏曹俊
**
*******************************************************************************/
//！！！！！！！！！ 学员加群462249121下载代码和交流




#pragma once

///解码和显示视频
struct AVPacket;
struct AVCodecParameters;
class XDecode;
#include <list>
#include <mutex>
#include <QThread>
#include "IVideoCall.h"
class XVideoThread :public QThread
{
public:
	//打开，不管成功与否都清理
	virtual bool Open(AVCodecParameters* para, IVideoCall* call, int width, int height);
	virtual void Push(AVPacket* pkt);
	void run();

	XVideoThread();
	virtual ~XVideoThread();
	//最大队列
	int maxList = 100;
	bool isExit = false;
protected:
	std::list <AVPacket*> packs;
	std::mutex mux;
	XDecode* decode = 0;
	IVideoCall* call = 0;


};

