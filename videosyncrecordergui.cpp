#include "videosyncrecordergui.h"
#include "pgtsettings.h"
#include "pgfsettings.h"
#include "depthsettings.h"
#include "colorsettings.h"
#include "FrameGrabberPG.h"
#include <QtDebug>
//#include <date_time\local_time\local_time.hpp>

VideoSyncRecorderGUI::VideoSyncRecorderGUI(QWidget *parent)
	: QMainWindow(parent)
{
	myPlayer = new Player();
	QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),
		this, SLOT(updatePlayerUI(QImage)));
	myStreamer = new Streamer();
	QObject::connect(myStreamer, SIGNAL(processedImageStream(vector<QImage>,QImage,QImage,QImage)),
		this, SLOT(updateStreamerUI(vector<QImage>, QImage, QImage,QImage)));
	pgtsettings = new PGTsettings();
	QObject::connect(pgtsettings, SIGNAL(buttonClick(QString)), this, SLOT(onPGTsettingsok(QString)));
	ui.setupUi(this);

	record = false;
	stream = false;
	widthRGB = 640;
	heightRGB = 480;
	bytesPerFrame = widthRGB * heightRGB * 24 / 8;
	trueImageSize = bytesPerFrame + 8;

	widthDepth = 320;
	heightDepth = 240;
	bytesPerDepth = widthDepth * heightDepth * 16 / 8;
	bytesPerFrameDepth = widthDepth * heightDepth * 16 / 8;
	trueImageSizeDepth = bytesPerFrameDepth + 8;
	fps = 30;
	bpp = 24;
	bps = 8;

	//setting all view checkboxes to checked state
	ui.view_PGT->setCheckState(Qt::CheckState::Checked);
	ui.view_PGF->setCheckState(Qt::CheckState::Checked);
	ui.view_depth->setCheckState(Qt::CheckState::Checked);
	ui.view_color->setCheckState(Qt::CheckState::Checked);
	//set default Intel depth image to grey
	ui.greyButton->setChecked(true);
	
	//stream video as default
	myStreamer->Stream();
	
	//set values on sidebar
	setSideBar();
}



VideoSyncRecorderGUI::~VideoSyncRecorderGUI()
{
	delete myPlayer;
	delete myStreamer;
	//delete ui;
}

void VideoSyncRecorderGUI::onPGTsettingsok(QString txt)
{
	QMessageBox box;
	box.setText("It works!");
	box.exec();
	ui.PGT_fps->setText(txt);
}

void VideoSyncRecorderGUI::updatePlayerUI(QImage img)
{
	if (!img.isNull())
	{
		ui.label->setAlignment(Qt::AlignCenter);
		ui.label->setPixmap(QPixmap::fromImage(img).scaled(ui.label->size()));//.Qt::KeepAspectRatio, Qt::FastTransformation));
	}
}

//when the PGT view checkbox is clicked
void VideoSyncRecorderGUI::on_view_PGT_stateChanged()
{
	if (ui.view_PGT->isChecked())
	{
		myStreamer->Stream();
	}
	else
	{
		//ui.label->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));
		myStreamer->StopStreaming();
		ui.label->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));
	}
}

//when the PGF view checkbox is clicked
void VideoSyncRecorderGUI::on_view_PGF_stateChanged()
{
	if (ui.view_PGF->isChecked())
	{
		myStreamer->Stream();
	}
	else
	{
		myStreamer->StopStreaming();
		ui.label->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));
	}
}

void VideoSyncRecorderGUI::on_pushButton_4_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,tr("Open Video"), ".",tr("Video Files (*.avi *.mpg *.mp4)"));
	if (!filename.isEmpty())
	{
		if (!myPlayer->loadVideo(filename.toUtf8().data()))
		{
			QMessageBox msgBox;
			msgBox.setText("The selected video could not be opened!");
			msgBox.exec();
		}
	}
}

void VideoSyncRecorderGUI::on_pushButton_3_clicked()
{
	if (myPlayer->isStopped())
	{
		myPlayer->Play();
		ui.pushButton_3->setText(tr("Stop"));
	}else
	{
		myPlayer->Stop();
		ui.pushButton_3->setText(tr("Play"));
	}
}

void VideoSyncRecorderGUI::on_pushButton_clicked()
{
	if (myStreamer->isStopped() && stream == false)
	{
		myStreamer->Stream();
		stream = true;
		ui.pushButton->setText(tr("Stop Streaming"));
	}
	else
	{
		myStreamer->StopStreaming();
		stream = false;
		ui.pushButton->setText(tr("Start Streaming"));
	}
}



//When Start/Stop Recording button is clicked. 
void VideoSyncRecorderGUI::on_recordButton_clicked()
{
	if (ui.recordButton->text() == "Start Recording!")
	{
		ui.recordButton->setText(tr("Stop Recording!"));
	}
	else
	{
		ui.recordButton->setText(tr("Start Recording!"));
	}
}

//when PG Top settings button is clicked
void VideoSyncRecorderGUI::on_PGTsettingsButton_clicked()
{
	PGTsettings box;
	box.setModal(true); //needs to close the new window before going back to the old window
	box.exec();
}

//when PG Front settings button is clicked
void VideoSyncRecorderGUI::on_PGFsettingsButton_clicked()
{
	//ui.PGFsettingsButton->setText(tr("working"));
	PGFsettings box2;
	box2.setModal(true); //needs to close the new window before going back to the old window
	box2.exec();
}

//when Intel Depth settings button is clicked
void VideoSyncRecorderGUI::on_DepthButton_clicked()
{
	Depthsettings box3;
	box3.setModal(true); //needs to close the new window before going back to the old window
	box3.exec();
}

//when Intel Color settings button is clicked
void VideoSyncRecorderGUI::on_ColorButton_clicked()
{
	Colorsettings box4;
	box4.setModal(true); //needs to close the new window before going back to the old window
	box4.exec();
}

void VideoSyncRecorderGUI::updateStreamerUI(vector<QImage> imgs, QImage imgIntel, QImage imgIntelDepth, QImage imgIntelIR)
{
	if (imgIntel.height() == 0)
	{
		//QMessageBox::warning(0, "Empty image", "Warning, the image is empty");
	}
#pragma omp parallel sections
		{
#pragma omp section
			{
				if (!imgs[0].isNull())
				{
					ui.label->setAlignment(Qt::AlignCenter);
				//img.scaled((int)img.width() / 4,(int)img.height() / 4,Qt::KeepAspectRatio);
					ui.label->setPixmap(QPixmap::fromImage(imgs[0]).scaled(ui.label->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
				}
				if (!imgs[1].isNull())
				{
					ui.label_3->setAlignment(Qt::AlignCenter);
				//img.scaled((int)img.width() / 4,(int)img.height() / 4,Qt::KeepAspectRatio);
					ui.label_3->setPixmap(QPixmap::fromImage(imgs[1]).scaled(ui.label_3->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
				}
			}
#pragma omp section
			{
				if (!imgIntel.isNull())
				{

					ui.label_2->setAlignment(Qt::AlignCenter);
					ui.label_2->setPixmap(QPixmap::fromImage(imgIntel).scaled(ui.label_2->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
					if (record == true)
					{
						outputVideo.write(QImage2Mat(imgIntel));
					}
					if (!imgIntelDepth.isNull())
					{
						ui.label_4->setAlignment(Qt::AlignCenter);
						ui.label_4->setPixmap(QPixmap::fromImage(imgIntel).scaled(ui.label_4->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
					}
					
				}
			}
		}
}

void VideoSyncRecorderGUI::setSideBar() 
{
	pg = new FrameGrabberPG(g_man);
	QString txt;
	int num = 0;

	//set FPS values
	float val = pg->getFPSvalue(num);
	txt = QString::number(val, 'f', 2);
	ui.PGT_fps->setText(txt);

	//set Shutter values
	val = pg->getShuttervalue(num);
	txt = QString::number(val, 'f', 2);
	ui.PGT_shutter->setText(txt);

	//set Gain values
	val = pg->getGainvalue(num);
	txt = QString::number(val, 'f', 2);
	ui.PGT_gain->setText(txt);

	//set Brightness values
	val = pg->getBrightvalue(num);
	txt = QString::number(val, 'f', 2);
	ui.PGT_bright->setText(txt);
}

void VideoSyncRecorderGUI::changeBrightness() 
{
	Error error;
	Property camProp;
	camProp.type = BRIGHTNESS;

	//error = 
}

//const string VideoSyncRecorderGUI::currentDateTime() 
//{
//	time_t     now = time(0);
//	struct tm  tstruct;
//	char       buf[80];
//	tstruct = *localtime(&now);
//	strftime(buf,80,"%Y-%m-%d-%H-%M-%S",&tstruct);
//	return buf;
//}

int32_t VideoSyncRecorderGUI::hexCharToDecimal(char ch)
{
	ch = toupper(ch); // Change it to uppercase
	if (ch >= 'A' && ch <= 'F')
		return 10 + ch - 'A';
	else // ch is '0', '1', ..., or '9'
		return ch - '0';
}
int VideoSyncRecorderGUI::hex2Dec(const string& hex)
{
	int decimalValue = 0;
	for (unsigned i = 0; i < hex.size(); i++)
		decimalValue = decimalValue * 16 + hexCharToDecimal(hex[i]);

	return decimalValue;
}

void VideoSyncRecorderGUI::closeSEQ(std::ofstream &myfile, int32_t width, int32_t height,int32_t bytesPerFrame,int32_t trueImageSize)
{
	myfile.seekp(0);
	// Write header
	int32_t var = 65261;
	int32_t ver = 3;
	int32_t sizeHeader = 1024;
	string desc = "Norpix sequence file";
	const char* ptr = desc.c_str();
	int32_t   imageFormat;
	int32_t origin = 0;
	var = hex2Dec("FEED");
	cout << "var = " << var << endl;
	// Write magic number
	myfile.write((char*)&var,4);
	myfile.write("Norpix seq\n",12*sizeof(char));
	for (int i=0; i<24 - 12*sizeof(char);i++)
	{
		int zer = 0;
		myfile.write((char*)&zer,1);
	}

	// Write 4 bytes: version number (3) (int32)
	myfile.write((char*)&ver, 4);
	myfile.write((char*)&sizeHeader, 4);
	// Generate and write description (512 bytes)
	myfile.write((char*)ptr,desc.size());
	for (int i=0; i<512 - desc.size();i++)
	{
		int zer = 0;
		myfile.write((char*)&zer,1);
	}
	// Write CImage data
	myfile.write((char*)&width,4);
	myfile.write((char*)&height,4);
	myfile.write((char*)&bpp,4);
	myfile.write((char*)&bps,4);
	myfile.write((char*)&bytesPerFrame,4);


	switch (bpp)
	{
	case 8:
		imageFormat = 100; // mono
		break;
	case 24:
		imageFormat = 200; // BGR
		break;
	case 32:
		imageFormat = 500; // BGRx
		break;
	default:
		imageFormat = 0; // Unknown
		break;
	}
	//imageFormat = 201;
	myfile.write((char*)&imageFormat,4);

	// Write 4 bytes: number of frames (uint32)
	myfile.write((char*)&totalFrames, 4);
	cout << "Total frames : " << totalFrames << endl;

	// Write 4 bytes: origin (0) (uint32)
	myfile.write((char*)&origin, 4);

	// Write 4 bytes: trueImageSize (uint32)
	myfile.write((char*)&trueImageSize, 4);

	// Write 8 bytes: framerate (double)
	//fps = totalFrames / sec;
	cout << "fps = " << fps << endl;
	myfile.write((char*)&fps, 8);

	// Fill rest of 1024 bytes with 0
	for (int i=0; i<432;i++)
	{
		int zer = 0;
		myfile.write((char*)&zer,1);
	}
	// Close file
	myfile.close();
}

QImage VideoSyncRecorderGUI::Mat2QImage(cv::Mat const& src)
{
	cv::Mat temp; // make the same cv::Mat
	cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
	QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
	dest.bits(); // enforce deep copy, see documentation 
	// of QImage::QImage ( const uchar * data, int width, int height, Format format )
	return dest;
}

cv::Mat VideoSyncRecorderGUI::QImage2Mat(QImage const& src)
{
	cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
	cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
	cvtColor(tmp, result,CV_BGR2RGB);
	return result;
}