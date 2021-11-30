/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMbProcessor.cpp
	Определение класса SlaveMbProcessor.
*/

#include "StdAfx.h"
#include "SlaveMbProcessor.h"

/**
 *	Конструктор.
 */
SlaveMbProcessor::SlaveMbProcessor(void)
: receiveCallback_(0)
, sendCallback_(0)
{
}

/**
 *	Деструктор.
 */
SlaveMbProcessor::~SlaveMbProcessor(void)
{
}

/**
 *	Обрабатывает команду с кодом ошибки.
 *	\param portName имя порта.
 *	\param address адрес MODBUS.
 *	\param errCode код ошибки.
 */
void SlaveMbProcessor::ProcessError(const std::wstring& portName, BYTE address, DWORD errCode)
{
}

/**
 *	Обрабатывает команду с данными.
 *	\param portName имя порта.
 *	\param address адрес MODBUS.
 *	\param data массив с данными.
 *	\param len размер массива.
 */
void SlaveMbProcessor::ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len)
{
	if( receiveCallback_ )
		receiveCallback_(portName.c_str(), address, data, len);
}

/**
 *	Обрабатывает команду окончания пересылки.
 *	\param portName имя порта.
 *	\param address адрес MODBUS.
 */
void SlaveMbProcessor::ProcessSend(const std::wstring& portName, BYTE address)
{
	if( sendCallback_ )
		sendCallback_(portName.c_str());
}

/**
 *	Обрабатывает команду с признаком таймаута.
 *	\param portName имя порта.
 *	\param address адрес MODBUS.
 */
void SlaveMbProcessor::ProcessTimeout(const std::wstring& portName, BYTE address)
{
}