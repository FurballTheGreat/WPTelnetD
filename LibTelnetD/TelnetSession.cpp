#include "stdafx.h"
#include "TelnetSession.h"

#define BUFFER_SIZE 10000
#include <iostream>

TelnetSession::TelnetSession(Connection *pConnection)
{
	_connection = pConnection;
	_buffer = static_cast<BYTE*>(malloc(BUFFER_SIZE));
	_optionBuf = static_cast<BYTE*>(malloc(BUFFER_SIZE));
	_outputBuf = static_cast<BYTE*>(malloc(BUFFER_SIZE));
	_optionBufPtr = _optionBuf;
	_outputUsed = 0;
	_inOption = false;
}
void TelnetSession::SetSizeChangedHandler(std::function<void(int, int)> pSizeChanged)
{
	_sizeChanged = pSizeChanged;
}

TelnetSession::~TelnetSession()
{
	free(_buffer);
}

bool TelnetSession::Write(int pBytesCount, BYTE *pData)
{
	return _connection->Write(pBytesCount, pData);
}


int TelnetSession::Read(int pBufferLength, BYTE *pBuffer)
{
	while (true) {
		if (_outputUsed > 0)
		{
			auto amount = pBufferLength > _outputUsed ? _outputUsed : pBufferLength;
			memcpy(pBuffer, _buffer, amount);
			if (amount < _outputUsed)
				memcpy(_buffer, _buffer + amount, _outputUsed - amount);
			_outputUsed -= amount;
			return amount;
		}

		auto read = _connection->Read(BUFFER_SIZE, _buffer);

		auto bufPtr = _buffer;

		while (bufPtr - _buffer < read)
		{
			if (_inOption)
			{
				*_optionBufPtr = *bufPtr;

				auto command = static_cast<TelnetCommand>(*_optionBuf);
				switch (command)
				{
				case DontOption:
				case DoOption:
				case WontOption:
				case WillOption:
					if (_optionBufPtr - _optionBuf == 1)
					{
						ProcessOptionNegotiation(command, static_cast<TelnetOption>(*(_optionBuf + 1)));
						_inOption = false;

					}
					break;
				case SubNegotiation:
					if (static_cast<TelnetCommand>(*_optionBufPtr) == EndOfSubNegotiation &&
						_optionBufPtr > _optionBuf &&
						static_cast<TelnetCommand>(*(_optionBufPtr - 1)) == InterpretAsCommand)
					{
						if (_optionBufPtr - _optionBuf > 4)
						{
							ProcessOptionSubNegotiation(command, static_cast<TelnetOption>(*(_optionBuf + 1)), _optionBuf + 2, (_optionBufPtr - _optionBuf) - 3);
						}

						_inOption = false;
					}
					break;
				case EndOfSubNegotiation:
				case InterpretAsCommand:
				case GoAhead:
				case EraseLine:
				case EraseCharacter:
				case AreYouThre:
				case AbortOutout:
				case InteruptProcess:
				case Break:
				case DataMark:
				case NoOperation:
				default:
					ProcessSimpleCommand(command);
					_inOption = false;
					break;
				}

				_optionBufPtr++;
				if (_optionBufPtr - _optionBuf >= BUFFER_SIZE)
					_inOption = false;

			}
			else if (*bufPtr == 255)
			{
				_inOption = true;
				_optionBufPtr = _optionBuf;
			}
			else
			{
				_outputBuf[_outputUsed] = *bufPtr;
				_outputUsed++;
			}
			bufPtr++;
		}

		if (_outputUsed > 0)
		{
			auto amount = pBufferLength > _outputUsed ? _outputUsed : pBufferLength;
			memcpy(pBuffer, _buffer, amount);
			if (amount < _outputUsed)
				memcpy(_buffer, _buffer + amount, _outputUsed - amount);
			_outputUsed -= amount;
			return amount;
		}
	}
}



void TelnetSession::ProcessOptionSubNegotiation(TelnetCommand pCommand, TelnetOption pOption, BYTE *pData, int pDataLength)
{
	switch (pOption)
	{
	case Echo: break;
	case SuppressGoAhead: break;
	case Status: break;
	case TimingMark: break;
	case TerminalType: break;
	case WindowSize: 
		if (pDataLength == 4) {
			auto vals = reinterpret_cast<USHORT*>(pData);
			auto width = ntohs(vals[0]);
			auto height = ntohs(vals[1]);
			_sizeChanged(width, height);
		}
		break;
	case TerminalSpeed: break;
	case RemoteFlowControl: break;
	case Linemode: break;
	case EnvironmentVariables: break;
	default: break;
	}
	
}
void TelnetSession::ProcessSimpleCommand(TelnetCommand pCommand)
{
	
}


void TelnetSession::SendNegotiation(TelnetCommand pCommand, TelnetOption pOption)
{
	BYTE buf[3];
	buf[0] = static_cast<BYTE>(InterpretAsCommand);
	buf[1] = static_cast<BYTE>(pCommand);
	buf[2] = static_cast<BYTE>(pOption);
	_connection->Write(sizeof(buf), buf);
}


void TelnetSession::ProcessOptionNegotiation(TelnetCommand pCommand, TelnetOption pOption)
{
	switch (pCommand)
	{
	case DontOption:
		std::cout << "Don't ";
		break;
	case DoOption:
		std::cout << "Do ";
		break;
	case WontOption:
		std::cout << "Won't ";
		break;
	case WillOption: 
		std::cout << "Will ";
		break;	
	}

	switch (pOption)
	{
	case Echo: 
	case SuppressGoAhead: 
	
	case TerminalType: 
	case WindowSize: 
	case TerminalSpeed: 
		if (pCommand == WillOption)
			SendNegotiation(DoOption, pOption);
		if (pCommand == DoOption)
			SendNegotiation(WillOption, pOption);
		break;
	}

	switch (pOption)
	{
	case Echo:
		std::cout << "echo" << std::endl;
		break;
	case SuppressGoAhead: 
		std::cout << "suppress goahead" << std::endl;
		break;
	case Status:
		std::cout << "status" << std::endl;
		break;
	case TimingMark:
		std::cout << "timingmark" << std::endl;
		break;
	case TerminalType:
		std::cout << "terminaltype" << std::endl;
		break;
	case WindowSize:
		std::cout << "windowsize" << std::endl;
		break;
	case TerminalSpeed:
		std::cout << "terminalspeed" << std::endl;
		break;
	case RemoteFlowControl:
		std::cout << "remote flow control" << std::endl;
		break;
	case Linemode:
		std::cout << "line mode" << std::endl;
		break;
	case EnvironmentVariables:
		std::cout << "env" << std::endl;
		break;
	}
}