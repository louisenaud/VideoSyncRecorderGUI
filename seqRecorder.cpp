#include "seqRecorder.h"

using namespace std;
seqRecorder::seqRecorder(void)
{
	// Filename string
		dateTime = seqRecorder::currentDateTime();
		string fnDepth = "Depth-" + dateTime + ".seq";
		string fnRGB = "RGB-" + dateTime + ".seq";
		// Open depth seq filestream
		IntelDepth.open(fnDepth.c_str(),ios::out|ios::binary);
	   
		// Allocate space for depth seq header
		for (int i=0; i<1024;i++)
		{
			int zer = 0;
			&IntelDepth.write((char*)&zer,1);
		}
		// Open RGB seq filestream
		IntelIR.open(fnRGB.c_str(),ios::out|ios::binary);
		// Allocate RGB for depth seq header
		for (int i=0; i<1024;i++)
		{
			int zer = 0;
			IntelIR.write((char*)&zer,1);
		}
		// Reference time
		begTime = clock();
		std::cout << "Recording" << std::endl;
}


seqRecorder::~seqRecorder(void)
{
	
}

int seqRecorder::closeSEQ(std::ofstream &myfile, int32_t width, int32_t height,int32_t bytesPerFrame,int32_t trueImageSize)
{
	// Close the filestream containing the seq file
//void closeSEQ(std::ofstream &myfile, int32_t width, int32_t height,int32_t bytesPerFrame,int32_t trueImageSize)

	myfile.seekp(0);
	// Write header
	int32_t var = 65261;
	int32_t ver = 3;
	int32_t sizeHeader = 1024;
	string desc = "Depth image sequence.";
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
	fps = totalFrames / sec;
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
	return 0;
}



const std::string seqRecorder::currentDateTime() 
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf,80,"%Y-%m-%d-%H-%M-%S",&tstruct);
	return buf;
}

int32_t seqRecorder::hexCharToDecimal(char ch)
{
	ch = toupper(ch); // Change it to uppercase
	if (ch >= 'A' && ch <= 'F')
		return 10 + ch - 'A';
	else // ch is '0', '1', ..., or '9'
		return ch - '0';
}
int seqRecorder::hex2Dec(const string& hex)
{
	int decimalValue = 0;
	for (unsigned i = 0; i < hex.size(); i++)
		decimalValue = decimalValue * 16 + seqRecorder::hexCharToDecimal(hex[i]);

	return decimalValue;
}