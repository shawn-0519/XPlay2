#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include "XDecode.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	///测试XDemux
	XDemux demux;

	const char* url = "Tom-and-jerry.mp4";
	cout << "demux.Open = " << demux.Open(url);
	cout << "CopyVPara = " << demux.CopyVPara() << endl;
	cout << "CopyAPara = " << demux.CopyAPara() << endl;
	cout << "seek=" << demux.Seek(0.95) << endl;

	/////////////////////////////
	///解码测试
    XDecode vdecodec;
	cout << "vdecode.Open() = " << vdecodec.Open(demux.CopyVPara()) << endl;
	//vdecode.Clear();
	//vdecode.Close();
    XDecode adecodec;
	cout << "adecode.Open() = " << adecodec.Open(demux.CopyAPara()) << endl;



    for(;;)
    {
        AVPacket *pkt = demux.Read();
        if (demux.IsAudio(pkt))
        {
            adecodec.Send(pkt);
            AVFrame *frame = adecodec.Recv();
        }
        else
        {
            vdecodec.Send(pkt);
            AVFrame *frame = vdecodec.Recv();
        }
        if (!pkt)break;
    }



    QApplication a(argc, argv);
    XPlay2 w;
    w.show();
    return a.exec();
}
