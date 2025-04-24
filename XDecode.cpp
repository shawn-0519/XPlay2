#include "XDecode.h"
extern "C"
{
#include<libavcodec/avcodec.h>
#include<libavutil/avutil.h>
}
#include <iostream>
using namespace std;

void XDecode::Close()
{
    mux.lock();
    if (codec) {

        //avcodec_close(codec); //已废弃avcodec_close
        avcodec_free_context(&codec);//自动调用 avcodec_close()；
    }
    mux.unlock();
}

void XDecode::Clear()
{
    mux.lock();
    if (codec) {
        avcodec_flush_buffers(codec);
    }
    mux.unlock();
}

//打开解码器
bool XDecode::Open(AVCodecParameters* para)
{
    if (!para) return false;
    Close();

    //////////////////////////////////////////////////////////
    ///解码器打开
    ///找到解码器
    const AVCodec* vcodec = avcodec_find_decoder(para->codec_id);
    if (!vcodec)
    {
        avcodec_parameters_free(&para);
        cout << "can't find the codec id " << para->codec_id;
        return false;
    }
    cout << "find the AVCodec " << para->codec_id << endl;

    mux.lock();
    codec = avcodec_alloc_context3(vcodec);

    ///配置解码器上下文参数
    avcodec_parameters_to_context(codec, para);
    avcodec_parameters_free(&para);
    //八线程解码
    codec->thread_count = 8;

    ///打开解码器上下文
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0)
    {
        //释放解码器
        avcodec_free_context(&codec);
        mux.unlock();
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        cout << "avcodec_open2  failed! :" << buf << endl;
        return false;
    }
    mux.unlock();
    cout << "video avcodec_open2 success!" << endl;
    return true;
}



bool XDecode::Send(AVPacket* pkt)
{
    //容错处理
    if(!pkt || pkt->size <= 0 || !pkt->data)return false;
    mux.lock();
    if (!codec){
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec, pkt);
    mux.unlock();
    av_packet_free(&pkt);
    if (re != 0) return false;
    return true;
}

AVFrame * XDecode::Recv()
{
    mux.lock();
    if (!codec) {
        mux.unlock();
        return nullptr;
    }
    AVFrame *frame = av_frame_alloc();
    int re = avcodec_receive_frame(codec,frame);
    mux.unlock();
    if (re != 0) {
        av_frame_free(&frame);
        return nullptr;
    }
    //cout << "[" << frame->linesize[0] << "] " << flush;
    return frame;
}

XDecode::XDecode()
{
}

XDecode::~XDecode()
{
}
