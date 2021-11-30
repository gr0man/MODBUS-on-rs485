/*
	Author:		R.Gorohov (K1 SOFT)

	Command.cpp
	����������� ������� Command, DataCommand, SendCommand, ErrorCommand, TimeoutCommand.
*/

#include "StdAfx.h"
#include "Command.h"

#include "CmdProcessor.h"

/**
 *	�����������.
 *	\param portName ��� ����� RS485, �� ������� ������� ����������, �������������� �������� �������.
 *	\param address ����� MODBUS ����������.
 */
Command::Command(const std::wstring& portName, BYTE address)
: portName_(portName)
, address_(address)
{
}

/**
 *	����������.
 */
Command::~Command(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////

/**
 *	�����������.
 *	\param portName ��� ����� RS485, �� ������� ������� ����������, �������������� �������� �������.
 *	\param address ����� MODBUS ����������.
 *	\param data ��������� �� ����� � �������.
 *	\param len ������ ������ � ������.
 */
DataCommand::DataCommand(const std::wstring& portName, BYTE address, const BYTE* data, int len)
: Command(portName, address)
, data_(new BYTE[len])
, len_(len)
{
	memcpy(data_, data, len);
}

/**
 *	����������.
 */
DataCommand::~DataCommand()
{
	delete[] data_;
	data_ = NULL;
}

/**
 *	�������� ��������������� ���� ����������� ������.
 *	\param processor ��������� �� ���������� ������.
 */
bool DataCommand::Process(CmdProcessor* processor)
{
	processor->ProcessData(GetPortName(), GetAddress(), data_, len_);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 *	�����������.
 *	\param portName ��� ����� RS485, �� ������� ������� ����������, �������������� �������� �������.
 *	\param address ����� MODBUS ����������.
 */
SendCommand::SendCommand(const std::wstring& portName, BYTE address)
: Command(portName, address)
{
}

/**
 *	����������.
 */
SendCommand::~SendCommand()
{
}

/**
 *	�������� ��������������� ���� ����������� ������.
 *	\param processor ��������� �� ���������� ������.
 */
bool SendCommand::Process(CmdProcessor* processor)
{
	processor->ProcessSend(GetPortName(), GetAddress());
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////

/**
 *	�����������.
 *	\param portName ��� ����� RS485, �� ������� ������� ����������, �������������� �������� �������.
 *	\param address ����� MODBUS ����������.
 *	\param errorCode ��� ������.
 */
ErrorCommand::ErrorCommand(const std::wstring& portName, BYTE address, DWORD errorCode)
: Command(portName, address)
, errorCode_(errorCode)
{
}

/**
 *	����������.
 */
ErrorCommand::~ErrorCommand()
{
}

/**
 *	�������� ��������������� ���� ����������� ������.
 *	\param processor ��������� �� ���������� ������.
 */
bool ErrorCommand::Process(CmdProcessor* processor)
{
	processor->ProcessError(GetPortName(), GetAddress(), errorCode_);
	return false;
}