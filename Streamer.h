#pragma once
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <qrunnable.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FrameGrabberIntel.h"
#include "FrameGrabberPG.h"
#include "Manager.h"

class Streamer : public QThread
{
	Q_OBJECT
private:
	bool stop;
	bool stream;
	QMutex mutex;
	QWaitCondition condition;
	Mat frame;
	vector<Mat> frames;
	int frameRate;
	VideoCapture capture;
	Mat RGBframe;
	QImage img;
	vector<QImage> imgs;
	QImage imgIntel;
	QImage imgIntelDepth;
	QImage imgIntelIR;
	QImage imgPtGrSide;
	FrameGrabberIntel grabIntel;
	FrameGrabberPG* grabPG1;
	//FrameGrabberPG grabPGSide;
	Manager *g_man;
	Manager *g_manPG;
	vector<Mat> framesPG;
	Mat framePG;

signals:
	//Signal to output frame to be displayed
	void processedImageStream(const vector<QImage> &imgs, const QImage &image2, const QImage &image3, const QImage &image4);
	//void processedImageStream(QImage &image, QImage &image2);

protected:
	void run();
	void msleep(int ms);
public:
	//Constructor
	Streamer(QObject *parent = 0);
	//Destructor
	~Streamer(void);
	void initGrabber();
	

	// Stream from available sensors
	void Stream();
	// Stop streaming
	void StopStreaming();
	//check if the player is streamimg
	bool isStopped();
};




