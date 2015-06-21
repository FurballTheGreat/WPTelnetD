#pragma once

#include "stdafx.h"
#include "Connection.h"
#include<mutex>
#include <thread>
#include <vector>
#include "TelnetConsole.h"

class CommandProcessor;

enum TelnetCommand { 
	InterpretAsCommand = 255,
	DontOption = 254,
	DoOption = 253,
	WontOption = 252,
	WillOption = 251,
	SubNegotiation = 250,
	GoAhead = 249,
	EraseLine = 248,
	EraseCharacter = 247,
	AreYouThre = 246,
	AbortOutout = 245,
	InteruptProcess = 244,
	Break = 243,
	DataMark = 242,
	NoOperation = 241, 
	EndOfSubNegotiation = 240 
};

enum TelnetOption
{
	Echo = 1,
	SuppressGoAhead = 3,
	Status = 5, 
	TimingMark = 6,
	TerminalType = 24,
	WindowSize = 31,
	TerminalSpeed = 32,
	RemoteFlowControl = 33,
	Linemode = 34,
	EnvironmentVariables = 36
};




class TelnetSession
{
	std::function<void(int, int)> _sizeChanged;
	Connection *_connection;
	BYTE *_buffer;
	void ProcessOptionSubNegotiation(TelnetCommand pCommand, TelnetOption pOption, BYTE *pData, int pDataLength);
	void ProcessSimpleCommand(TelnetCommand pCommand);
	void ProcessOptionNegotiation(TelnetCommand pCommand,TelnetOption pOption);
	void SendNegotiation(TelnetCommand pCommand, TelnetOption pOption);

	BYTE* _optionBuf;
	BYTE* _optionBufPtr;
	bool _inOption;
	BYTE* _outputBuf;
	int _outputUsed;
	
public:
	TelnetSession(Connection *pConnection);
	~TelnetSession();
	int Read(int pBufferLen, BYTE *pBuffer);
	bool Write(int pBytesCount, BYTE *pData);
	void SetSizeChangedHandler(std::function<void(int, int)> pSizeChanged);
};

