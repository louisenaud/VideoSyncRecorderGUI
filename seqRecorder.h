#pragma once
#include <date_time\local_time\local_time.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>
using namespace std;
class seqRecorder
{
public:
	seqRecorder(void);
	~seqRecorder(void);
	const string currentDateTime() ;
	int closeSEQ(ofstream &myfile, int32_t width, int32_t height,int32_t bytesPerFrame,int32_t trueImageSize) ;
	int32_t hexCharToDecimal(char ch);
	int hex2Dec(const string& hex);
	
private:
	ofstream IntelDepth;
	ofstream IntelIR;
	string g_fileNameIntelDepth;
	string g_fileNameIntelIR;
	string g_fileNameIntelRGB;
	string g_fileNamePG1;
	string g_fileNamePG2;
	string dateTime;
	time_t begTime;
	int bpp;
	int bps;
	int totalFrames;
	int fps;
	int sec;
};

