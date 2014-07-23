
#include "Streamer.h"
#include <thread>
#include <chrono>
#include <qdebug.h>
#include <omp.h>
#include <qmessagebox.h>                                            

Streamer::Streamer(QObject *parent) : QThread(parent)
{
	stop = true;
	grabIntel = FrameGrabberIntel(g_man);
	grabPG1 = new FrameGrabberPG(g_man);
	g_man = new Manager();
	g_manPG = new Manager();
	//frameRate = 50;
}


Streamer::~Streamer(void)
{
	delete grabPG1;
	mutex.lock();
	stop = true;
	capture.release();
	grabIntel.~FrameGrabberIntel();
	condition.wakeOne();
	mutex.unlock();
	wait();
}

void Streamer::initGrabber()
{
	grabIntel = FrameGrabberIntel(g_man);
	grabPG1 = new FrameGrabberPG(g_manPG);


}

void Streamer::Stream()
{
	if (!isRunning())
	{
		if (isStopped())
		{
			stop = false;
		}
		start(TimeCriticalPriority);
	}
	qDebug() << "stop : " << stop;
	qDebug() << "isRunning : " << isRunning();
}

// Grab frames from sensor
void Streamer::run()
{
	//if (stop == false && isRunning())
	//{
		qDebug() << "Grabbing?";
		//int delay = (1000/frameRate);
		int delay = (1000 / 10);
		qDebug() << "stop : " << stop;
		qDebug() << "frameRate : " << frameRate;
		// Variables for depth display
		cv::Mat adjMap;
		double min;
		double max;
		imgs.resize(2);

		while (!stop)
		{
#pragma omp parallel sections
		{
			//Senz3D grabbing
#pragma omp section
			{
				frames = grabIntel.grabFrame();
				qDebug() << "Grabbing Intel!!";
				if (frames.size() == 0)
				{
					stop = true;
				}
				for (int i = 0; i < frames.size(); i++)
				{
					frame = frames[i];

					if (frame.channels() == 3)
					{
						switch (i)
						{
						case 0:
							cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
							imgIntel = QImage((const unsigned char*)(RGBframe.data),
								RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);
							qDebug() << "Channels: " << frame.channels();
						case 1:
							threshold(frame, frame, 500, 255, 2);
							threshold(frame, frame, 245, 255, 3);
							frame.convertTo(frame, CV_8U);
							resize(frame, frame, Size(640, 480));

							imgIntelDepth = QImage((const unsigned char*)(frame.data),
								frame.cols, frame.rows, QImage::Format_RGB888);
							qDebug() << "Channels Depth RGB: " << frame.channels();

						default:
							break;
						}
					}
					else
					{
						switch (i)
						{
						case 0:
							imgIntel = QImage((const unsigned char*)(frame.data),
								frame.cols, frame.rows, QImage::Format_Indexed8);
							qDebug() << "Channels: " << frame.channels();
						case 1:
							//threshold(frame, frame, 500, 255, 2);
							//threshold(frame, frame, 245, 255, 3);
							cv::minMaxIdx(frame, &min, &max);
							cv::convertScaleAbs(frame, adjMap, 255 * min / (max - min), -255 * min / (max - min));

							adjMap.convertTo(frame, CV_8U);
							frame = adjMap;
							imgIntelDepth = QImage((const unsigned char*)(frame.data),
								frame.cols, frame.rows, QImage::Format_Indexed8);
							qDebug() << "Channels: " << frame.channels();
						case 2:
							imgIntelIR = QImage((const unsigned char*)(frame.data),
								frame.cols, frame.rows, QImage::Format_Indexed8);
						default:
							break;
						}
					}
					imshow("Intel",frame);

				}
			}

			// Grab Frames from Point Grey Sensor
#pragma omp section
			{
				framesPG = grabPG1->grabFrame();
				qDebug() << "Grabbing PG!!";
				if (frames.size() == 0)
				{
					stop = true;
				}

				for (int i = 0; i < framesPG.size(); i++)
				{

					framePG = framesPG[i];

					if (framePG.channels() == 3)
					{
						cv::cvtColor(framePG, RGBframe, CV_BGR2RGB);
						img = QImage((const unsigned char*)(RGBframe.data),
							RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);

					}
					else
					{
						img = QImage((const unsigned char*)(framePG.data),
							framePG.cols, framePG.rows, QImage::Format_Indexed8);

					}
					imgs[i] = img;
					imshow("PG", framePG);
					waitKey(30);
				}
			}

		}
			emit processedImageStream(imgs, imgIntel, imgIntelDepth, imgIntelIR);
			this->msleep(delay);
		}
	//}

}


void Streamer::StopStreaming()
{
	qDebug() << "Stop = " << stop;
	stop = true;
}

//check if the player is streamimg
bool Streamer::isStopped()
{
	return this->stop;
}

// Micro sleep
void Streamer::msleep(int ms)
{
	int ts = ms;
	std::this_thread::sleep_for(std::chrono::milliseconds(ts));
}



