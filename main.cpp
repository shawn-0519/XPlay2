#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include "XDecodec.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    XDemux demux;
    cout << "demux.Open = " << demux.Open("Tom-and-jerry.mp4");
    demux.Read();
    demux.Clear();
    demux.Close();
    cout << "demux.Open = " << demux.Open("v1080.mp4");

    cout << "demux.CopyVPara() = " << demux.CopyVPara() << endl;
    cout << "demux.CopyAPara() = " << demux.CopyAPara() << endl;

    cout << "demux.Seek() = " << demux.Seek(0.95) << endl;

    XDecodec vdecodec;
    cout << "vdecodec.Open(demux.CopyVPara()):" << vdecodec.Open(demux.CopyVPara()) << endl;
    vdecodec.Clear();
    vdecodec.Close();
    XDecodec adecode;
    cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;

    for(;;)
    {
        AVPacket *pkt = demux.Read();
        if (!pkt)break;
    }



    QApplication a(argc, argv);
    XPlay2 w;
    w.show();
    return a.exec();
}
