#include "FrameGrabberPG.h"
#include <QApplication>
#include <QtDebug>
#include <QtGlobal>
#include <stdio.h>

FrameGrabberPG::FrameGrabberPG(void)
{

	this->g_man = new Manager();
	g_error = g_busMgr.GetNumOfCameras(&g_numCameras);
	if (g_error != PGRERROR_OK)
	{
		std::cout << g_error.GetDescription() << std::endl;
		return;
	}

	printf("Number of cameras detected: %u\n", g_numCameras);

	if (g_numCameras < 1)
	{
		printf("Insufficient number of cameras... press Enter to exit.\n");
		getchar();
		return;
	}

	g_ppCameras = new Camera*[g_numCameras];
	//g_ppCameras = new GigECamera*[g_numCameras];

	// Connect to all detected cameras and attempt to set them to
	// a common video mode and frame rate
	for (unsigned int i = 0; i < g_numCameras; i++)
	{
		g_ppCameras[i] = new Camera();
		//g_ppCameras[i] = new GigECamera();

		PGRGuid guid;
		g_error = g_busMgr.GetCameraFromIndex(i, &guid);
		if (g_error != PGRERROR_OK)
		{
			std::cout << g_error.GetDescription() << std::endl;
			return;
		}

		// Connect to a camera
		g_error = g_ppCameras[i]->Connect(&guid);
		if (g_error != PGRERROR_OK)
		{
			std::cout << g_error.GetDescription() << std::endl;
			return;
		}

		// Get the camera information
		CameraInfo camInfo;
		g_frames.at(i) = Mat();
		g_error = g_ppCameras[i]->GetCameraInfo(&camInfo);
		if (g_error != PGRERROR_OK)
		{
			std::cout << g_error.GetDescription() << std::endl;
			return;
		}

		//PrintCameraInfo(&camInfo); 



	}
	bpp = 8;
	bps = 8;
}

FrameGrabberPG::FrameGrabberPG(Manager *man)
{
	this->g_man = man;
	g_error = g_busMgr.GetNumOfCameras(&g_numCameras);
	g_frames.resize(g_numCameras);
	if (g_error != PGRERROR_OK)
	{
		std::cout << g_error.GetDescription() << std::endl;
		return;
	}

	qDebug() << "Number of cameras detected: " << g_numCameras;

	if (g_numCameras < 1)
	{
		qDebug() << "Insufficient number of cameras... press Enter to exit.\n";
		return;
	}
	g_ppCameras = new Camera*[g_numCameras];
	//g_ppCameras = new GigECamera*[g_numCameras];

	// Connect to all detected cameras and attempt to set them to
	// a common video mode and frame rate
	for (unsigned int i = 0; i < g_numCameras; i++)
	{
		g_ppCameras[i] = new Camera();
		//g_ppCameras[i] = new GigECamera();

		PGRGuid guid;
		g_error = g_busMgr.GetCameraFromIndex(i, &guid);
		if (g_error != PGRERROR_OK)
		{
			qDebug() << g_error.GetDescription();
			return;
		}

		// Connect to a camera
		g_error = g_ppCameras[i]->Connect(&guid);
		if (g_error != PGRERROR_OK)
		{
			qDebug() << g_error.GetDescription();
			return;
		}

		// Get the camera information
		CameraInfo camInfo;
		g_error = g_ppCameras[i]->GetCameraInfo(&camInfo);
		g_frames.at(i) = Mat();
		if (g_error != PGRERROR_OK)
		{
			std::cout << g_error.GetDescription() << std::endl;
			return;
		}


		//testing out packet size
		//GigEProperty packetSizeProp;
		//packetSizeProp.propType = PACKET_SIZE;
		//packetSizeProp.value = 6764;
		//g_error = g_ppCameras[i]->SetGigEProperty(&packetSizeProp);

		//VideoMode currVideoMode;
		//FrameRate currFrameRate;

		//we know for sure it is in format 7 mode!
		//g_error = g_ppCameras[i]->GetVideoModeAndFrameRate(&currVideoMode, &currFrameRate);
		//if (g_error != PGRERROR_OK) 
		//{
		//	qDebug() << "Error getting video mode";
		//}
		//if (currVideoMode != VIDEOMODE_FORMAT7)
		//{
		//	g_ppCameras[i]->SetVideoModeAndFrameRate(VIDEOMODE_FORMAT7,currFrameRate);
		//	qDebug() << "Format 7 mode set!";
		//}

		//Format7ImageSettings pImageSettings;
		//Format7Info pInfo;
		//Format7Info pInfo2;
		//unsigned int  pPacketSize;
		//float pPercentage;
		//bool pSupported;
		//g_error = g_ppCameras[i]->GetFormat7Info(&pInfo, &pSupported);
		//if (g_error != PGRERROR_OK)
		//{
		//	qDebug() << "Didn't get format info.......";
		//	qDebug() << g_error.GetDescription();
		//}
		//qDebug() << "Supported: " << pSupported;
		//qDebug() << "Size: " << pInfo.packetSize;

		//g_error = g_ppCameras[i]->GetFormat7Configuration(&pImageSettings, &pPacketSize, &pPercentage);

		//if (g_error != PGRERROR_OK)
		//{
		//	qDebug() << "Packet size not retrieved.......";
		//	qDebug() << g_error.GetDescription();
		//}
		//qDebug() << "Packet size: " << pPacketSize;
		//qDebug() << "Percentage: " << pPercentage;
		//const Format7ImageSettings* pImageSettings;
		//pPacketSize = 6764;

		//g_error = g_ppCameras[i]->SetFormat7Configuration(&pImageSettings, pPacketSize);
		//if (g_error != PGRERROR_OK);
		//{
		//	qDebug() << "didn't set packet size correctly.";
		//	qDebug() << g_error.GetDescription();
		//}

		//g_error = g_ppCameras[i]->GetFormat7Info(&pInfo2, &pSupported);
		//if (g_error != PGRERROR_OK)
		//{
		//	qDebug() << "Didn't get format info.......";
		//	qDebug() << g_error.GetDescription();
		//}
		//qDebug() << "Supported: " << pSupported;
		//qDebug() << "Size: " << pInfo2.packetSize;

		//property info
		PropertyInfo camPropInfo;
		camPropInfo.type = FRAME_RATE;
		if (g_ppCameras[i]->GetPropertyInfo(&camPropInfo) != PGRERROR_OK)
		{

			qDebug() << "Doesn't work...";
			//	qDebug() << camPropInfo.pUnitAbbr;
			//std::cout << camPropInfo.pUnitAbbr << std::endl;

		}
		qDebug() << camPropInfo.absMax;
		qDebug() << camPropInfo.absMin;

		//properties
		Property camPropFR;
		Property camPropBright;
		Property camPropShutter;
		Property camPropGain;
		camPropFR.type = FRAME_RATE;
		camPropBright.type = BRIGHTNESS;
		camPropGain.type = GAIN;
		camPropShutter.type = SHUTTER;

		//set frame rate values!
		camPropFR.absValue = 30.0;
		camPropFR.absControl = true;
		camPropFR.onePush = false;
		camPropFR.onOff = true;
		camPropFR.autoManualMode = false;

		//set gain values!
		camPropGain.absValue = 4.0;
		camPropGain.absControl = true;
		camPropGain.onePush = false;
		camPropGain.onOff = true;
		camPropGain.autoManualMode = false;

		//set brightness values!
		camPropBright.absValue = 9.204;
		camPropBright.absControl = true;
		camPropBright.onePush = false;
		camPropBright.onOff = true;
		camPropBright.autoManualMode = false;

		//set shutter values!
		camPropShutter.absValue = 19.871;
		camPropShutter.absControl = true;
		camPropShutter.onePush = false;
		camPropShutter.onOff = true;
		camPropShutter.autoManualMode = false;

		//set property values!
		if (g_ppCameras[i]->SetProperty(&camPropFR) != PGRERROR_OK)
		{
			qDebug() << "wrong";
		}

		if (g_ppCameras[i]->SetProperty(&camPropGain) != PGRERROR_OK)
		{
			qDebug() << "wrong";
		}

		if (g_ppCameras[i]->SetProperty(&camPropBright) != PGRERROR_OK)
		{
			qDebug() << "wrong";
		}

		if (g_ppCameras[i]->SetProperty(&camPropShutter) != PGRERROR_OK)
		{
			qDebug() << "wrong";
		}

		//retrieve property values
		if (g_ppCameras[i]->GetProperty(&camPropFR) != PGRERROR_OK)
		{
			qDebug() << "wrong haha";
		}
		if (g_ppCameras[i]->GetProperty(&camPropBright) != PGRERROR_OK)
		{
			qDebug() << "wrong haha";
		}
		if (g_ppCameras[i]->GetProperty(&camPropGain) != PGRERROR_OK)
		{
			qDebug() << "wrong haha";
		}
		if (g_ppCameras[i]->GetProperty(&camPropShutter) != PGRERROR_OK)
		{
			qDebug() << "wrong haha";
		}

		//print out retrieved values
		qDebug() << "Frame Rate: " << camPropFR.absValue;
		qDebug() << "Brightness: " << camPropBright.absValue;
		qDebug() << "Gain: " << camPropGain.absValue;
		qDebug() << "Shutter: " << camPropShutter.absValue;

		//qDebug() << "Property Info:" << camPropInfo.pUnitAbbr; 
		VideoMode* pVideoMode;
		FrameRate* pFrameRate;

	}
	//g_error = Camera::StartSyncCapture( g_numCameras, (const Camera**)g_ppCameras );
	for (int k = 0; k < g_numCameras; k++)
	{
		g_error = g_ppCameras[k]->StartCapture();
		if (g_error != PGRERROR_OK)
		{
			qDebug() << g_error.GetDescription();
			return;
		}
	}
	/*else
		g_man->isGrabbing = true;*/
}

vector<Mat> FrameGrabberPG::grabFrame()
{

	// Display the timestamps for all cameras to show that the image
	// capture is synchronized for each image
	for (unsigned int i = 0; i < g_numCameras; i++)
	{
		Image image;
		g_error = g_ppCameras[i]->RetrieveBuffer(&image);
		if (g_error != PGRERROR_OK)
		{
			qDebug() << g_error.GetDescription();
		}

		TimeStamp timestamp = image.GetTimeStamp();
		/*printf(
			"Cam %d  - TimeStamp [%d %d]\n",
			i,
			timestamp.cycleSeconds,
			timestamp.cycleCount);*/

		g_frames[i] = Mat(image.GetRows(), image.GetCols(), CV_8U, image.GetData());
	}
	return g_frames;

}

FrameGrabberPG::~FrameGrabberPG(void)
{
	for (unsigned int i = 0; i < g_numCameras; i++)
	{
		g_ppCameras[i]->StopCapture();
		g_ppCameras[i]->Disconnect();
		delete g_ppCameras[i];
	}
	delete[] g_ppCameras;
	//g_man->isGrabbing = false;
}

float FrameGrabberPG::getFPSvalue(int camNum)
{
	Property camPropFR;
	camPropFR.type = FRAME_RATE;
	if (g_ppCameras[camNum]->GetProperty(&camPropFR) != PGRERROR_OK)
	{
		qDebug() << "wrong haha";
	}
	return camPropFR.absValue;
}

float FrameGrabberPG::getShuttervalue(int camNum)
{
	Property camPropShutter;
	camPropShutter.type = SHUTTER;
	if (g_ppCameras[camNum]->GetProperty(&camPropShutter) != PGRERROR_OK)
	{
		qDebug() << "wrong haha";
	}
	return camPropShutter.absValue;
}

float FrameGrabberPG::getGainvalue(int camNum)
{
	Property camPropGain;
	camPropGain.type = GAIN;
	if (g_ppCameras[camNum]->GetProperty(&camPropGain) != PGRERROR_OK)
	{
		qDebug() << "wrong haha";
	}
	return camPropGain.absValue;
}

float FrameGrabberPG::getBrightvalue(int camNum)
{
	Property camPropBright;
	camPropBright.type = BRIGHTNESS;
	if (g_ppCameras[camNum]->GetProperty(&camPropBright) != PGRERROR_OK)
	{
		qDebug() << "wrong haha";
	}
	return camPropBright.absValue;
}