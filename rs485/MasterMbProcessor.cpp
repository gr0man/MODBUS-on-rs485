/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMbProcessor.cpp
	����������� ������ MasterMbProcessor.
*/

#include "StdAfx.h"
#include "MasterMbProcessor.h"

/**
 *	�����������.
 */
MasterMbProcessor::MasterMbProcessor(void)
: receiveCallback_(0)
, timeoutCallback_(0)
, sendCallback_(0)
{
}

/**
 *	����������.
 */
MasterMbProcessor::~MasterMbProcessor(void)
{
}

/**
 *	������������ ������� � ����� ������.
 *	\param portName ��� �����.
 *	\param address ����� MODBUS.
 *	\param errCode ��� ������.
 */
void MasterMbProcessor::ProcessError(const std::wstring& portName, BYTE address, DWORD errCode)
{
}

/**
 *	������������ ������� � �������.
 *	\param portName ��� �����.
 *	\param address ����� MODBUS.
 *	\param data ������ � �������.
 *	\param len ������ �������.
 */
void MasterMbProcessor::ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len)
{
	if( receiveCallback_ )
		receiveCallback_(portName.c_str(), address, data, len);
}

/**
 *	������������ ������� ��������� ���������.
 *	\param portName ��� �����.
 *	\param address ����� MODBUS.
 */
void MasterMbProcessor::ProcessSend(const std::wstring& portName, BYTE address)
{
	if( sendCallback_ )
	{
		sendCallback_(portName.c_str());
	}
}

/**
 *	������������ ������� � ��������� ��������.
 *	\param portName ��� �����.
 *	\param address ����� MODBUS.
 */
void MasterMbProcessor::ProcessTimeout(const std::wstring& portName, BYTE address)
{
	if( timeoutCallback_ )
		timeoutCallback_(portName.c_str(), address);
}