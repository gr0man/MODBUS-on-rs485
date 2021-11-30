/*
	Author:		R.Gorohov (K1 SOFT)

	Command.cpp
	Определение классов Command, DataCommand, SendCommand, ErrorCommand, TimeoutCommand.
*/

#include "StdAfx.h"
#include "Command.h"

#include "CmdProcessor.h"

/**
 *	Конструктор.
 *	\param portName имя порта RS485, на котором открыто устройство, инициировавшее создание команды.
 *	\param address адрес MODBUS устройства.
 */
Command::Command(const std::wstring& portName, BYTE address)
: portName_(portName)
, address_(address)
{
}

/**
 *	Деструктор.
 */
Command::~Command(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////

/**
 *	Конструктор.
 *	\param portName имя порта RS485, на котором открыто устройство, инициировавшее создание команды.
 *	\param address адрес MODBUS устройства.
 *	\param data указатель на буфер с данными.
 *	\param len размер буфера в байтах.
 */
DataCommand::DataCommand(const std::wstring& portName, BYTE address, const BYTE* data, int len)
: Command(portName, address)
, data_(new BYTE[len])
, len_(len)
{
	memcpy(data_, data, len);
}

/**
 *	Деструктор.
 */
DataCommand::~DataCommand()
{
	delete[] data_;
	data_ = NULL;
}

/**
 *	Вызывает соответствующий член обработчика команд.
 *	\param processor указатель на обработчик команд.
 */
bool DataCommand::Process(CmdProcessor* processor)
{
	processor->ProcessData(GetPortName(), GetAddress(), data_, len_);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 *	Конструктор.
 *	\param portName имя порта RS485, на котором открыто устройство, инициировавшее создание команды.
 *	\param address адрес MODBUS устройства.
 */
SendCommand::SendCommand(const std::wstring& portName, BYTE address)
: Command(portName, address)
{
}

/**
 *	Деструктор.
 */
SendCommand::~SendCommand()
{
}

/**
 *	Вызывает соответствующий член обработчика команд.
 *	\param processor указатель на обработчик команд.
 */
bool SendCommand::Process(CmdProcessor* processor)
{
	processor->ProcessSend(GetPortName(), GetAddress());
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////

/**
 *	Конструктор.
 *	\param portName имя порта RS485, на котором открыто устройство, инициировавшее создание команды.
 *	\param address адрес MODBUS устройства.
 *	\param errorCode код ошибки.
 */
ErrorCommand::ErrorCommand(const std::wstring& portName, BYTE address, DWORD errorCode)
: Command(portName, address)
, errorCode_(errorCode)
{
}

/**
 *	Деструктор.
 */
ErrorCommand::~ErrorCommand()
{
}

/**
 *	Вызывает соответствующий член обработчика команд.
 *	\param processor указатель на обработчик команд.
 */
bool ErrorCommand::Process(CmdProcessor* processor)
{
	processor->ProcessError(GetPortName(), GetAddress(), errorCode_);
	return false;
}