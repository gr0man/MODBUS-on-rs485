/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMbProcessor.cpp
	����������� ������ SlaveMbProcessor.
*/

#include "StdAfx.h"
#include "SlaveMbProcessor.h"

/**
 *	�����������.
 */
SlaveMbProcessor::SlaveMbProcessor(void)
: receiveCallback_(0)
, sendCallback_(0)
{
}

/**
 *	����������.
 */
SlaveMbProcessor::~SlaveMbProcessor(void)
{
}

/**
 *	������������ ������� � ����� ������.
 *	\param portName ��� �����.
 *	\param address ����� MODBUS.
 *	\param errCode ��� ������.
 */
void SlaveMbProcessor::ProcessError(const std::wstring& portName, BYTE address, DWORD errCode)
{
}

/**
 *	������������ ������� � �������.
 *	\param portName ��� �����.
 *	\param address ����� MODBUS.
 *	\param data ������ � �������.
 *	\param len ������ �������.
 */
void SlaveMbProcessor::ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len)
{
	if( receiveCallback_ )
		receiveCallback_(portName.c_str(), address, data, len);
}

/**
 *	������������ ������� ��������� ���������.
 *	\param portName ��� �����.
 *	\param address ����� MODBUS.
 */
void SlaveMbProcessor::ProcessSend(const std::wstring& portName, BYTE address)
{
	if( sendCallback_ )
		sendCallback_(portName.c_str());
}

/**
 *	������������ ������� � ��������� ��������.
 *	\param portName ��� �����.
 *	\param address ����� MODBUS.
 */
void SlaveMbProcessor::ProcessTimeout(const std::wstring& portName, BYTE address)
{
}