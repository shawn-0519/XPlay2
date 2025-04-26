#include "XDemux.h"
#include <iostream>
#include <mutex>
using namespace std;
extern "C"
{
#include <libavformat/avformat.h>
}

#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avcodec.lib")

static double r2d(AVRational r)
{
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

bool XDemux::Open(const char* url)
{
    Close();
   //参数设置
    AVDictionary* opts = NULL;
    //设置rtsp流已tcp协议打开
    av_dict_set(&opts, "rtsp_transport", "tcp", 0);

    //网络延时时间
    av_dict_set(&opts, "max_delay", "500", 0);

    mux.lock();
    int re = avformat_open_input(
        &ic,
        url,
        0,  // 0表示自动选择解封器
        &opts //参数设置，比如rtsp的延时时间
    );

    if (re != 0)
    {
        mux.unlock();
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        cout << "open " << url << " failed! :" << buf << endl;
        return false;
    }
    cout << "open " << url << " success! " << endl;

    //获取流信息
    re = avformat_find_stream_info(ic, 0);

    //总时长 毫秒
    //int totalMs = ic->duration / (AV_TIME_BASE / 1000);
    // Fix the warning by explicitly casting the int64_t value to int after ensuring it is within the range of int.
    int totalMs = static_cast<int>(ic->duration / (AV_TIME_BASE / 1000));
    cout << "totalMs = " << totalMs << endl;

    //打印视频流详细信息
    av_dump_format(ic, 0, url, 0);

    //音视频索引，读取时区分音视频
    int videoStream = 0;
    int audioStream = 1;

    //获取视频流
    videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    AVStream* as = ic->streams[videoStream];

    //set width height
    width = as->codecpar->width;
    height = as->codecpar->height;

    cout << "===============================================" << endl;
    cout << videoStream << "Vedio Format" << endl;
    cout << "codec_id = " << as->codecpar->codec_id << endl;
    cout << "format = " << as->codecpar->format << endl;
    cout << "width=" << as->codecpar->width << endl;
    cout << "height=" << as->codecpar->height << endl;
    //帧率 fps 分数转换
    cout << "video fps = " << r2d(as->avg_frame_rate) << endl;

    cout << "===============================================" << endl;
    cout << audioStream << "Audio Format" << endl;
    //获取音频流
    audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    as = ic->streams[audioStream];

    //set sampleRate channels
    sampleRate = as->codecpar->sample_rate;
    channels = as->codecpar->ch_layout.nb_channels;

    cout << "codec_id = " << as->codecpar->codec_id << endl;
    cout << "format = " << as->codecpar->format << endl;
    cout << "sample_rate = " << as->codecpar->sample_rate << endl;
    //AVSampleFormat;
    cout << "channels = " << as->codecpar->ch_layout.nb_channels << endl;
    //一帧数据？？ 单通道样本数 
    cout << "frame_size = " << as->codecpar->frame_size << endl;
    //1024 * 2 * 2 = 4096  fps = sample_rate/frame_size

    mux.unlock();
    
    return true;
}

AVPacket* XDemux::Read()
{
    mux.lock();
    if (!ic) {
        mux.unlock();
        return 0;
    }
    AVPacket* pkt = av_packet_alloc();
    //读取一帧，并分配空间
    int re = av_read_frame(ic, pkt);
    if (re != 0) {
        mux.unlock();
        av_packet_free(&pkt);
        return 0;
    }

    //显示时间戳  转为毫秒
    pkt->pts = pkt->pts * (1000 * (r2d(ic->streams[pkt->stream_index]->time_base)));
    //解码时间戳  转为毫秒
    pkt->dts = pkt->dts * (1000 * (r2d(ic->streams[pkt->stream_index]->time_base)));

    mux.unlock();
    cout << pkt->pts << " " << flush;//强制刷新缓冲区的数据
    return pkt;
}

AVCodecParameters* XDemux::CopyVPara()
{
    mux.lock();
    if (!ic) {
        mux.unlock();
        return 0;
    }
    AVCodecParameters* par = avcodec_parameters_alloc();
    avcodec_parameters_copy(par, ic->streams[vedioStream]->codecpar);
    mux.unlock();
    return par;
}

AVCodecParameters* XDemux::CopyAPara()
{
    mux.lock();
    if (!ic) {
        mux.unlock();
        return 0;
    }
    AVCodecParameters* par = avcodec_parameters_alloc();
    avcodec_parameters_copy(par, ic->streams[audioStream]->codecpar);
    mux.unlock();
    return par;
}

bool XDemux::IsAudio(AVPacket* pkt)
{
    if (!pkt)return false;
    if (pkt->stream_index == vedioStream)return false;
    return true;
}

bool XDemux::Seek(double pos)
{
    mux.lock();
    if (!ic)
    {
        mux.unlock();
        return false;
    }
    //清理读取缓冲
    avformat_flush(ic);

    long long seekPos = 0;
    seekPos = ic->streams[vedioStream]->duration * pos;
    int re = av_seek_frame(ic, vedioStream, seekPos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
    mux.unlock();
    if (re < 0) return false;
    return true;
}

void XDemux::Clear()
{
    mux.lock();
    if (!ic) {
        mux.unlock();
        return;
    }
    //清理读取缓冲
    avformat_flush(ic);
    mux.unlock();
}

void XDemux::Close()
{
    mux.lock();
    if (!ic) {
        mux.unlock();
        return;
    }
    avformat_close_input(&ic);
    totalMs = 0;
    mux.unlock();
}

XDemux::XDemux()
{
    static bool isFirst = true;
    static std::mutex dmux;
    dmux.lock();
    if (isFirst)
    {
        //初始化封装库
        //av_register_all();

        //初始化网络库 （可以打开rtsp rtmp http 协议的流媒体视频）
        avformat_network_init();
        isFirst = false;
    }
    dmux.unlock();
}

XDemux::~XDemux()
{
}
