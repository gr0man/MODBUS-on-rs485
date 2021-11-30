/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMB.cpp
	Определения классов SlaveBehaviorMB и SlaveMB.
*/

#include "StdAfx.h"
#include "SlaveMB.h"
#include "FrameMB.h"
#include "errorCodes.h"

#include "PortRs485.h"

#include "rs485.h"


bool SlaveBehaviorMB::IsAddressValid(BYTE address)
{
	LOG_Error("SlaveBehaviorMB addres %d\n", address_);
	LOG_Error("SlaveBehaviorMB addres in frame %d\n", address);

	return (address==address_);
}

/**
 *	Конструктор.
 *	\param portName имя порта.
 *	\param address адрес слейва MODBUS.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\param charTimeout таймаут чтения (записи) символа.
 */
SlaveMB::SlaveMB(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout, CmdProcessor& processor)
: behavior_(address)
, trans_(portName, baudrate, parity, charTimeout, behavior_, processor)
, address_(address)
{
	LOG_Info("SlaveMB on \"%s\" constructor\n", portName);
}

/**
 *	Деструктор.
 */
SlaveMB::~SlaveMB(void)
{
	//LOG_Info("SlaveMB on \"%s\" destructor\n", trans_.GetPortName().c_str());
	trans_.Stop();
}

/**
 *	Устанавливает адрес слейва MODBUS.
 *	\param address адрес.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMB::SetAddress(BYTE address)
{
	DWORD err = OPERATION_COMPLETE;
	if( (address_ < 1) ||
		(address_ > 247) )
		err = WRONG_ADDRESS;

	address_ = address;
	behavior_.SetAddress(address);

	return err;
}

/**
 *	Инициализирует посылку ответа слейвом MODBUS.
  *	\param data указатель на буфер с данными.
 *	\param dataLen размер буфера в байтах.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMB::SendReply(const BYTE* data, int dataLen)
{
	return trans_.NewSend(address_, data, dataLen);
}

/**
 *	Изменяет настройки RS485.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMB::Settings(DWORD baudrate, BYTE parity)
{
	return trans_.Settings(baudrate, parity);
}