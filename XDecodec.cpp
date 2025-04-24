#include "XDecodec.h"
extern "C"
{
#include<libavcodec/avcodec.h>
}
#include <iostream>
using namespace std;

void XDecodec::Close()
{
    mux.lock();
    if (codec) {

        //avcodec_close(codec); //�ѷ���avcodec_close
        avcodec_free_context(&codec);//�Զ����� avcodec_close()��
    }
    mux.unlock();
}

void XDecodec::Clear()
{
    mux.lock();
    if (codec) {
        avcodec_flush_buffers(codec);
    }
    mux.unlock();
}

//�򿪽�����
bool XDecodec::Open(AVCodecParameters* para)
{
    if (!para) return false;
    Close();

    //////////////////////////////////////////////////////////
    ///��������
    ///�ҵ�������
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

    ///���ý����������Ĳ���
    avcodec_parameters_to_context(codec, para);
    avcodec_parameters_free(&para);
    //���߳̽���
    codec->thread_count = 8;

    ///�򿪽�����������
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0)
    {
        //�ͷŽ�����
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



XDecodec::XDecodec()
{
}

XDecodec::~XDecodec()
{
}
