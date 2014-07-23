#ifndef VIDEOSYNCRECORDERGUI_H
#define VIDEOSYNCRECORDERGUI_H
#include <QFileDialog>
#include <QMessageBox>
#include <player.h>
#include <Streamer.h>
#include <QtWidgets/QMainWindow>
#include "ui_videosyncrecordergui.h"
#include <omp.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <pgtsettings.h>
using namespace std;

class VideoSyncRecorderGUI : public QMainWindow
{
	Q_OBJECT

private:
	Ui::VideoSyncRecorderGUIClass ui;

	Player* myPlayer;
	Streamer* myStreamer;
	PGTsettings* pgtsettings;
	FrameGrabberPG* pg;
	Manager *g_man;
	VideoWriter outputVideo;
	int ex;

	int32_t hexCharToDecimal(char ch);
	int hex2Dec(const string& hex);
	//const string currentDateTime();
	void closeSEQ(std::ofstream &myfile, int32_t width, int32_t height, int32_t bytesPerFrame, int32_t trueImageSize);
	QImage Mat2QImage(cv::Mat const& src);

	cv::Mat QImage2Mat(QImage const& src);

	int32_t totalFrames;
	int widthRGB;
	int heightRGB;
	int bytesPerFrame;
	int trueImageSize;
	int fps;
	int bpp;
	int bps;
	int widthDepth;
	int heightDepth;
	int bytesPerDepth;
	int trueImageSizeDepth;
	int bytesPerFrameDepth;

private slots:
	//Display video frame in player UI
	void updatePlayerUI(QImage img);
	//Slot for the load video push button.
	void on_pushButton_4_clicked();
	// Slot for the play push button.
	void on_pushButton_3_clicked();
	// Slot for the stream push button.
	void on_pushButton_clicked();
	// Slot for the Start/Stop Recording button
	void on_recordButton_clicked();

	void on_PGTsettingsButton_clicked();

	void on_PGFsettingsButton_clicked();

	void on_DepthButton_clicked();

	void on_ColorButton_clicked();

	void on_view_PGT_stateChanged();

	void on_view_PGF_stateChanged();

	//Display video frame in streamer UI
	void updateStreamerUI(vector<QImage> imgs, QImage imgIntel, QImage imgIntelDepth, QImage imgIntelIR);

	//Display new values on the side
	void onPGTsettingsok(QString txt);

	void changeBrightness();

public:
	VideoSyncRecorderGUI(QWidget *parent = 0);
	~VideoSyncRecorderGUI();
	bool record;
	bool stream;
	ofstream IntelRGB;
	ofstream IntelDepth;
	string dateTime;
	string g_fileNameIntelDepth;
	string g_fileNameIntelRGB;
	string g_fileNameIntelIR;
	void setSideBar();
};

#endif // VIDEOSYNCRECORDERGUI_H
