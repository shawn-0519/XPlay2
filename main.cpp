#include <QThread>
#include <iostream>
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include "XDecode.h"
#include "XVideoWidget.h"
#include "XResample.h"
#include "XPlay2.h"
#include "XAudioPlay.h"

using namespace std;

class TestThread :public QThread {
public:
    void Init() {
       //Tom-and-jerry.mp4

        const char* url = "v1080.mp4";
        cout << "demux.Open = " << demux.Open(url);
        cout << "CopyVPara = " << demux.CopyVPara() << endl;
        cout << "CopyAPara = " << demux.CopyAPara() << endl;
        //cout << "seek=" << demux.Seek(0.95) << endl;

        /////////////////////////////
        ///解码测试
        cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
        //vdecode.Clear();
        //vdecode.Close();
        cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
        cout << "resample.Open() = " << resample.Open(demux.CopyAPara()) << endl;

        /////////////////////////////
        ///XAudioPlay测试
        XAudioPlay::Get()->channels = demux.channels;
        XAudioPlay::Get()->sampleRate = demux.sampleRate;
        cout << "XAudioPlay::Get()->Open() = " << XAudioPlay::Get()->Open() << endl;


    }
    unsigned char* pcm = new unsigned char[1024 * 1024];
    void run() {
        for(;;)
    {
        AVPacket *pkt = demux.Read();
        if (demux.IsAudio(pkt))
        {
            adecode.Send(pkt);
            AVFrame *frame = adecode.Recv();
            int len = resample.ReSample(frame, pcm);
            cout << "Resample:" << len << " ";
            while (len > 0) 
            {
                if (XAudioPlay::Get()->Getfree() > len) 
                {
                    XAudioPlay::Get()->Write(pcm,len);
                    break;
                }
                msleep(1);
            }
           

        }
        else
        {
            vdecode.Send(pkt);
            AVFrame *frame = vdecode.Recv();
            video->Repaint(frame);
            //msleep(40);
        }
        if (!pkt)break;
    }
    }
    ///测试XDemux
    XDemux demux;
    ///解码测试
    XDecode vdecode;
    XDecode adecode;
    XResample resample;
    XVideoWidget* video;

};

int main(int argc, char *argv[])
{
    /*for (;;)
    {
        AVPacket* pkt = demux.Read();
        if (demux.IsAudio(pkt))
        {
            adecode.Send(pkt);
            AVFrame* frame = adecode.Recv();
        }
        else
        {
            vdecode.Send(pkt);
            AVFrame* frame = vdecode.Recv();
        }
        if (!pkt)break;
    }*/
    TestThread tt;
    tt.Init();

    QApplication a(argc, argv);
    XPlay2 w;
    w.show();

    //初始化gl窗口
    w.ui.openGLWidget->Init(tt.demux.width, tt.demux.height);
    tt.video = w.ui.openGLWidget;
    tt.start();


    return a.exec();
}
