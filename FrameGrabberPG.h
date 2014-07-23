#pragma once
#include <opencv2\opencv.hpp>
#include "FrameGrabber.h"
#include "Manager.h"
#include <vector>
#include "FlyCapture2.h"

using namespace FlyCapture2;
using namespace cv;
class FrameGrabberPG : public FrameGrabber
{
public:
	FrameGrabberPG(void);
	FrameGrabberPG(Manager *man);
	vector<Mat> grabFrame();
	~FrameGrabberPG(void);
	float getFPSvalue(int camNum);
	float getShuttervalue(int camNum);
	float getGainvalue(int camNum);
	float getBrightvalue(int camNum);
	//int fr;
	
	//void PrintError(Error error);

	Error g_error;

    BusManager g_busMgr;
    unsigned int g_numCameras;
	Camera** g_ppCameras;
	Mat g_bgrImage					 ;
	vector<Mat> g_frames;
	//Mat g_depthImage				 ;
	//Mat g_irImage					 ;
	int bpp;
	int bps;
	Manager *g_man;
	char timeStamp[512];
};

