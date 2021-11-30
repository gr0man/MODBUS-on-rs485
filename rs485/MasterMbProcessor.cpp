/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMbProcessor.cpp
	Определение класса MasterMbProcessor.
*/

#include "StdAfx.h"
#include "MasterMbProcessor.h"

/**
 *	Конструктор.
 */
MasterMbProcessor::MasterMbProcessor(void)
: receiveCallback_(0)
, timeoutCallback_(0)
, sendCallback_(0)
{
}

/**
 *	Деструктор.
 */
MasterMbProcessor::~MasterMbProcessor(void)
{
}

/**
 *	Обрабатывает команду с кодом ошибки.
 *	\param portName имя порта.
 *	\param address адрес MODBUS.
 *	\param errCode код ошибки.
 */
void MasterMbProcessor::ProcessError(const std::wstring& portName, BYTE address, DWORD errCode)
{
}

/**
 *	Обрабатывает команду с данными.
 *	\param portName имя порта.
 *	\param address адрес MODBUS.
 *	\param data массив с данными.
 *	\param len размер массива.
 */
void MasterMbProcessor::ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len)
{
	if( receiveCallback_ )
		receiveCallback_(portName.c_str(), address, data, len);
}

/**
 *	Обрабатывает команду окончания пересылки.
 *	\param portName имя порта.
 *	\param address адрес MODBUS.
 */
void MasterMbProcessor::ProcessSend(const std::wstring& portName, BYTE address)
{
	if( sendCallback_ )
	{
		sendCallback_(portName.c_str());
	}
}

/**
 *	Обрабатывает команду с признаком таймаута.
 *	\param portName имя порта.
 *	\param address адрес MODBUS.
 */
void MasterMbProcessor::ProcessTimeout(const std::wstring& portName, BYTE address)
{
	if( timeoutCallback_ )
		timeoutCallback_(portName.c_str(), address);
}