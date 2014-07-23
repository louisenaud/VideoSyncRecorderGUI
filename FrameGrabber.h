#pragma once
#include <opencv2\opencv.hpp>
#include <vector>
using namespace cv;
class FrameGrabber
{
public:
	//FrameGrabber(void);
	virtual ~FrameGrabber(void);
	virtual vector<Mat> grabFrame() = 0;
};



