#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include "XDecode.h"
#include <iostream>
#include "XVideoWidget.h"
#include "XResample.h"
#include <QThread>
using namespace std;

class TestThread :public QThread {
public:
    void Init() {
       

        const char* url = "Tom-and-jerry.mp4";
        cout << "demux.Open = " << demux.Open(url);
        cout << "CopyVPara = " << demux.CopyVPara() << endl;
        cout << "CopyAPara = " << demux.CopyAPara() << endl;
        cout << "seek=" << demux.Seek(0.95) << endl;

        /////////////////////////////
        ///解码测试
        cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
        //vdecode.Clear();
        //vdecode.Close();
        cout << "resample.Open() = " << resample.Open(demux.CopyAPara()) << endl;
        cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
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
            cout << "ReSample: " << resample.ReSample(frame, pcm) << " ";
        }
        else
        {
            vdecode.Send(pkt);
            AVFrame *frame = vdecode.Recv();
        }
        if (!pkt)break;
    }
    }
    ///测试XDemux
    XDemux demux;
    ///解码测试
    XDecode vdecode;
    XDecode adecode;
    XVideoWidget* video;
    XResample resample;
};

int main(int argc, char *argv[])
{
	

    TestThread tt;
    tt.Init();
    
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

    QApplication a(argc, argv);
    XPlay2 w;
    w.show();

    //初始化gl窗口
    w.ui.openGLWidget->Init(tt.demux.width, tt.demux.height);
    tt.video = w.ui.openGLWidget;
    tt.start();


    return a.exec();
}
