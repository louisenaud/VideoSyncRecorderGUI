#pragma once
#include <opencv2\opencv.hpp>
#include "Manager.h"
#include "FrameGrabber.h"
using namespace cv;
class FrameGrabberIntel : public FrameGrabber
{
public:
	bool g_printStreamSetting        ;
	int g_imageStreamProfileIdx      ;
	int g_depthStreamProfileIdx      ;
	bool g_irStreamShow              ;
	double g_imageBrightness         ;
	double g_imageContrast           ;
	bool g_printTiming               ;
	bool g_showClosedPoint           ;
	Mat g_bgrImage					 ;
	Mat g_depthImage				 ;
	Mat g_irImage					 ;
	cv::VideoCapture g_capture       ;
	Manager *g_man                   ;
	vector<Mat> g_frames             ;

	FrameGrabberIntel(void);
	FrameGrabberIntel(Manager *man);
	~FrameGrabberIntel(void);
	vector<Mat> grabFrame();
};