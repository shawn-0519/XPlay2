#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    XDemux demux;

    demux.Read();
    //demux.Clear();
    //demux.Close();
    cout << "demux.Open = " << demux.Open("v1080.mp4");

    cout << "demux.CopyVPara() = " << demux.CopyVPara() << endl;
    cout << "demux.CopyAPara() = " << demux.CopyAPara() << endl;

    cout << "demux.Seek() = " << demux.Seek(0.95);

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
