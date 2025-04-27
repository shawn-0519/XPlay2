#include "XAudioThread.h"
#include "XResample.h"
#include "XDecode.h"
#include "XAudioPlay.h"
#include <iostream>
using namespace std;
bool XAudioThread::Open(AVCodecParameters* para, int SampleRate, int channels)
{
	if (!para) return false;
	mux.lock();
	if (!res) res = new XResample();
	if (!decode) decode = new XDecode();
	if (!ap) ap = XAudioPlay::Get();
	bool re = true;
	if (!res->Open(para,false))
	{
		cout << "XResample open failed!" << endl;
		re = false;
	}

	ap->channels = channels;
	ap->sampleRate = SampleRate;
	if (!ap->Open())
	{
		cout << "XAudioPlay open failed!" << endl;
		re = false;
	}
	if (!decode->Open(para))
	{
		cout << "XDecode open failed!" << endl;
		re = false;
	}
	mux.unlock();
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void XAudioThread::run()
{
}

void XAudioThread::Close()
{
}

XAudioThread::XAudioThread()
{
}

XAudioThread::~XAudioThread()
{
}
