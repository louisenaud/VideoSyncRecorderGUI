#pragma once
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FrameGrabberIntel.h"
#include "FrameGrabberPG.h"
using namespace cv;
class Player : public QThread
{
	Q_OBJECT
private:
	bool stop;
	bool stream;
	QMutex mutex;
	QWaitCondition condition;
	Mat frame;
	int frameRate;
	VideoCapture capture;
	Mat RGBframe;
	QImage img;
	QImage imgIntel;
	QImage imgPtGrSide;

signals:
	//Signal to output frame to be displayed
	void processedImage(const QImage &image);
protected:
	void run();
	void msleep(int ms);
public:
	//Constructor
	Player(QObject *parent = 0);
	//Destructor
	~Player();
	//Load a video from memory
	bool loadVideo(string filename);
	//Play the video
	void Play();
	//Stop the video
	void Stop();
	//check if the player has been stopped
	bool isStopped() const;

};

