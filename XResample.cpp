#include "XResample.h"
extern "C"
{
#include <libswresample/swresample.h>
#include <libavutil/channel_layout.h>
#include <libavcodec/avcodec.h>
}
#include <iostream>
#pragma comment(lib,"swresample.lib")

void XResample::Close()
{
    mux.lock();
    if (actx) {
        swr_free(&actx);
    }
    mux.unlock();
}

//输出参数和输入参数一致除了采样格式，输出为S16
bool XResample::Open(AVCodecParameters* para, bool isClearPara)
{
    if (!para)return false;
    // 定义输出和输入声道布局
    mux.lock();
    AVChannelLayout out_ch_layout, in_ch_layout;

    av_channel_layout_default(&out_ch_layout, 2);// stereo output
    av_channel_layout_default(&in_ch_layout, para->ch_layout.nb_channels); // input channels
    
    int re = swr_alloc_set_opts2(
        &actx,                        //SwrContext
        &out_ch_layout,               //out_channel
        (AVSampleFormat)outFormat,    //out_sample_fmt
        para->sample_rate,            //out_sample_rate
        &in_ch_layout,                //in_ch_layout
        (AVSampleFormat)para->format, //in_sample_fmt//如果将para->format错误改为outFormat，则会产生巨大噪音
        para->sample_rate,            //in_sample_rate
        0,0                           //log_offset/ log_ctx
        );
    if (isClearPara) 
    {
        avcodec_parameters_free(&para);
    }

    re = swr_init(actx);
    mux.unlock();
    if (re != 0)
    {
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        std::cout << "swr_init  failed! :" << buf << std::endl;
        return false;
    }

    //unsigned char *pcm = NULL;
    return true;
}


int XResample::ReSample(AVFrame* indata, unsigned char* outdata)
{
    if (!indata)return 0;
    if (!outdata) {
        av_frame_free(&indata);
        return 0;
    }
    uint8_t* data[2] = { 0 };
    data[0] = outdata;
    int re = swr_convert(
        actx,                           //SwrContext
        data,                           //out
        indata->nb_samples,             //out_count
        (const uint8_t**)indata->data,  //in
        indata->nb_samples              //in_count
    );
    if (re <= 0)return re;
    int outsize = re * indata->ch_layout.nb_channels * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    return outsize;
}



XResample::XResample()
{
}

XResample::~XResample()
{
}
