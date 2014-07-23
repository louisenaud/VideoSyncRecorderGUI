#include "FrameGrabberIntel.h"
#include <iostream>
using namespace std;
FrameGrabberIntel::FrameGrabberIntel(void)
{
	g_man = new Manager();

	g_printStreamSetting			= false;
	g_imageStreamProfileIdx			= 0;
	g_depthStreamProfileIdx			= 0;
	g_irStreamShow					= true;
	g_imageBrightness				= 1000;
	g_imageContrast					= 1000;
	g_printTiming					= false;
	g_showClosedPoint				= false;

	cout << "Is Grabbing : " << g_man->isGrabbing << endl;

	g_capture.open(CV_CAP_INTELPERC);
    if (!g_capture.isOpened())
    {
        cerr << "Can not open a capture object." << endl;
		system("PAUSE");
	}
	else
		g_man->isGrabbing = true;

	//Setup additional properies only after set profile of the stream
    if ( (-10000.0 < g_imageBrightness) && (g_imageBrightness < 10000.0))
        g_capture.set(CV_CAP_INTELPERC_IMAGE_GENERATOR | CV_CAP_PROP_BRIGHTNESS, g_imageBrightness);
    if ( (0 < g_imageContrast) && (g_imageContrast < 10000.0))
        g_capture.set(CV_CAP_INTELPERC_IMAGE_GENERATOR | CV_CAP_PROP_BRIGHTNESS, g_imageContrast);
	g_capture.set(CV_CAP_INTELPERC_IMAGE_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, (double)0.0);
	g_capture.set(CV_CAP_INTELPERC_DEPTH_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, 0.0);
	g_capture.set(CV_CAP_INTELPERC_DEPTH_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, 0.0);
	//g_bgrImage = Mat(480,640,3,CV_8U);
}

FrameGrabberIntel::FrameGrabberIntel(Manager *Man)
{
	//g_man = Manager();
	this->g_man = Man;

	g_printStreamSetting			= false;
	g_imageStreamProfileIdx			= 0;
	g_depthStreamProfileIdx			= 0;
	g_irStreamShow					= true;
	g_imageBrightness				= 1000;
	g_imageContrast					= 1000;
	g_printTiming					= false;
	g_showClosedPoint				= false;

	//cout << "Is Grabbing : " << g_man->isGrabbing << endl;

	g_capture.open(CV_CAP_INTELPERC);
    if (!g_capture.isOpened())
    {
        cerr << "Can not open a capture object." << endl;
		system("PAUSE");
	}
	/*else
		g_man->isGrabbing = true;*/

	//Setup additional properies only after set profile of the stream
    if ( (-10000.0 < g_imageBrightness) && (g_imageBrightness < 10000.0))
        g_capture.set(CV_CAP_INTELPERC_IMAGE_GENERATOR | CV_CAP_PROP_BRIGHTNESS, g_imageBrightness);
    if ( (0 < g_imageContrast) && (g_imageContrast < 10000.0))
        g_capture.set(CV_CAP_INTELPERC_IMAGE_GENERATOR | CV_CAP_PROP_BRIGHTNESS, g_imageContrast);
	g_capture.set(CV_CAP_INTELPERC_IMAGE_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, (double)0.0);
	g_capture.set(CV_CAP_INTELPERC_DEPTH_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, 0.0);
	g_capture.set(CV_CAP_INTELPERC_DEPTH_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, 0.0);
	//g_bgrImage = Mat(480,640,3,CV_8U);
	
}


vector<Mat> FrameGrabberIntel::grabFrame()
{
	vector<Mat> imgs;
	double ts;
	ts = g_capture.get(CV_CAP_PROP_POS_MSEC );
	cout << "Time Stamp : " << ts << endl;
	g_capture.grab();
	g_capture.retrieve(g_bgrImage, CV_CAP_INTELPERC_IMAGE);
	imgs.push_back(g_bgrImage);
	g_capture.retrieve(g_depthImage, CV_CAP_INTELPERC_DEPTH_MAP);
	imgs.push_back(g_depthImage);
	g_capture.retrieve(g_irImage, CV_CAP_INTELPERC_IR_MAP);
	imgs.push_back(g_irImage);
    return imgs;
}

FrameGrabberIntel::~FrameGrabberIntel(void)
{
	g_capture.release();
	//g_man->isGrabbing = false;
}
