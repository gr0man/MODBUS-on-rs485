/*
	Author:		R.Gorohov (K1 SOFT)

	Command.h
	���������� ������� Command, DataCommand, SendCommand, ErrorCommand, TimeoutCommand.
*/

#pragma once

#include <string>
#include "FrameMB.h"

class CmdProcessor;

/** ������� ����������� ����� �������. */
class Command
{
public:
	/** �����������. */
	Command(const std::wstring& portName, BYTE address);
	/** ����������. */
	virtual ~Command(void);

	/** ���������� ��� ��������� ����� RS485. */
	const std::wstring GetPortName(){ return portName_; }
	/** ���������� ����� MODBUS ����������. */
	BYTE GetAddress(){ return address_; }

	/** ���������� ������� (����������� � ����������).*/
	virtual bool Process(CmdProcessor* processor) = 0;

#ifdef USE_LOG
	virtual const char* GetName() = 0;
#endif

private:
	/** ��� ����� RS485. */
	std::wstring portName_;
	/** ����� MODBUS ����������. */
	BYTE address_;
};

////////////////////////////////////////////////////////////////////////////

/** ����� �������, ��������������� ����� � �������. */
class DataCommand : public Command
{
public:
	/** �����������. */
	DataCommand(const std::wstring& portName, BYTE address, const BYTE* data, int len);
	/** ����������. */
	~DataCommand();

	/** �������� ����� ����������� ������. */
	virtual bool Process(CmdProcessor* processor);

	/** ���������� ��������� �� ����� � ������� �������. */
	const BYTE *GetBuffer()const { return data_; }
	/** ���������� ������ ������ � ������� �������. */
	int GetLen()const { return len_; }

#ifdef USE_LOG
	virtual const char* GetName(){return "DataCommand";}
#endif

private:
	/** ����� � �������. */
	BYTE* data_;
	/** ������ ������. */
	int len_;
};

/////////////////////////////////////////////////////////////////////////////

/** �������, ��������������� ������� ���������� �������� ������. */
class SendCommand : public Command
{
public:
	/** �����������. */
	SendCommand(const std::wstring& portName, BYTE address);
	/** ����������. */
	~SendCommand();

	/** �������� ����� ����������� ������. */
	virtual bool Process(CmdProcessor* processor);

#ifdef USE_LOG
	virtual const char* GetName(){return "SendCommand";}
#endif
};

/////////////////////////////////////////////////////////////////////////////

/** ����� �������, ��������������� ������� ������. */
class ErrorCommand : public Command
{
public:
	/** �����������. */
	ErrorCommand(const std::wstring& portName, BYTE address, DWORD errorCode);
	/** �����������. */
	~ErrorCommand();

	/** �������� ����� ����������� ������. */
	virtual bool Process(CmdProcessor* processor);

#ifdef USE_LOG
	virtual const char* GetName(){return "ErrorCommand";}
#endif

private:
	/** ��� ������. */
	DWORD errorCode_;
};

//////////////////////////////////////////////////////////////////////////////

/** ����� �������, ��������������� ������� ���������� ������� �������� ������. */
class TimeoutCommand : public Command
{
public:
	/** �����������. */
	TimeoutCommand(const std::wstring& portName, BYTE address);
	/** ����������. */
	~TimeoutCommand();

	/** �������� ����� ����������� �����. */
	virtual bool Process(CmdProcessor* processor);

	#ifdef USE_LOG
	virtual const char* GetName(){return "TimeoutCommand";}
#endif
};