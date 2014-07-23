#pragma once
class Manager
{
	
public:
	bool isRecording;
	bool isReplaying;
	bool isProcessing;
	bool isGrabbing;

	Manager(void);
	~Manager(void);
};

