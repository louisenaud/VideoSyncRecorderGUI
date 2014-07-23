#include "Player.h"
#include <thread>
#include <chrono>

Player::Player(QObject *parent)
	: QThread(parent)
{
	stop = true;
}  

// Destructor
Player::~Player(void)
{
	mutex.lock();
	stop = true;
	capture.release();
	condition.wakeOne();
	mutex.unlock();
	wait();
}

// Load video 
bool Player::loadVideo(string filename) 
{
	capture.open(filename);
	if (capture.isOpened())
	{
		frameRate = (int) capture.get(CV_CAP_PROP_FPS);
		return true;
	}
	else
		return false;
}

// 
void Player::Play()
{
	if (!isRunning()) {
		if (isStopped()){
			stop = false;
		}
		start(LowPriority);
	}
}

// Run player
void Player::run()
{
	int delay = (1000/frameRate);
	while(!stop){
		if (!capture.read(frame))
		{
			stop = true;
		}
		if (frame.channels()== 3)
		{
			cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
			img = QImage((const unsigned char*)(RGBframe.data),
				RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
		}
		else
		{
			img = QImage((const unsigned char*)(frame.data),
				frame.cols,frame.rows,QImage::Format_Indexed8);
		}
		emit processedImage(img);
		this->msleep(delay);
	}
}

// Stop player
void Player::Stop()
{
	stop = true;
}
// micro sleep
void Player::msleep(int ms)
{
	int ts =  ms  ;
	std::this_thread::sleep_for(std::chrono::milliseconds(ts));
}
// check if player is stopped
bool Player::isStopped() const{
	return this->stop;
}

