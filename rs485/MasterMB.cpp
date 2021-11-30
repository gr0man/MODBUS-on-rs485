/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMB.cpp
	Определения классов MasterBehaviorMB и MasterMB.
*/

#include "StdAfx.h"
#include "MasterMB.h"

#include "errorCodes.h"

#include "FrameMB.h"
#include "Command.h"

#include "time.h"

#include "rs485.h"

/**
 *	Конструктор.
 */
MasterBehaviorMB::MasterBehaviorMB(void)
: responceTimeout_(0)
, slaveAddress_(0)
, requestTime_(0)
, isNeedToRead_(false)
{
}

/**
 *	Фиксирует окончание отправки фрейма.
 *	\param address адрес получателя.
 */
void MasterBehaviorMB::FinishSend(BYTE address)
{ /*fix time and address*/
	requestTime_ = GetCurrentTime();
	slaveAddress_ = address;

	isNeedToRead_ = (address != 0);

	LOG_Error("MasterBehaviorMB::FinishSend: set addres %d\n", slaveAddress_);
}

void MasterBehaviorMB::FinishReceive(BYTE address)
{
	isNeedToRead_ = (address != slaveAddress_);
}

/**
 *	Проверяет праильность адреса принятого фрейма.
 *	\param addres адрес принятого фрейма.
 *	\return true, если адрес совпадает с ожидаемым.
 */
bool MasterBehaviorMB::IsAddressValid(BYTE address)
{ 
	LOG_Error("MasterBehaviorMB addres %d\n", slaveAddress_);
	LOG_Error("MasterBehaviorMB addres in frame %d\n", address);

	return (slaveAddress_ == address);
}

/**
 *	Проверяет, не истекло ли время ожидания ответа.
 *	\return true, если таймаут ожидания превышен.
 */
bool MasterBehaviorMB::IsTimeoutExpiration()
{ 
	if( requestTime_ == 0 || responceTimeout_ == 0 )
		return false;

	ULONGLONG crTime = GetCurrentTime();

	return ( (crTime-requestTime_) > responceTimeout_ );
}

/**
 *	Сбрасывает параметры ожидания.
 */
void MasterBehaviorMB::ResetSettings()
{
	slaveAddress_ = 0;
	requestTime_ = 0;
	isNeedToRead_ = false;
}


///////////////////////////////////////////////////////////////////////////////////////

/**
 *	Конструктор.
 *	\param portName имя порта.
 *	\param baudrate скороть обмена.
 *	\param parity контроль четности.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\param responceTimeout таймаут ожидания ответа от слейва.
 */
MasterMB::MasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout, CmdProcessor& processor)
: trans_(portName, baudrate, parity, charTimeout, behavior_, processor)
{
	LOG_Info("MasterMB on \"%s\" constructor\n", portName);
}

/**
 *	Деструктор.
 */
MasterMB::~MasterMB(void)
{
	//LOG_Info("MasterMB on \"%s\" destructor\n", trans_.GetPortName().c_str());
	trans_.Stop();
}

/**
 *	Изменяет настройки RS485.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMB::Settings(DWORD baudrate, BYTE parity)
{
	return trans_.Settings(baudrate, parity);
}

/**
 *	Инициализирует посылку запроса мастера MODBUS.
 *	\param address адрес получателя.
 *	\param data указатель на буфер с данными.
 *	\param dataLen размер буфера в байтах.
 *	\return код ошибки (всегда OPERATION_COMPLETE).
 */
DWORD MasterMB::SendRequest(BYTE address, const BYTE* data, int dataLen)
{
	return trans_.NewSend(address, data, dataLen);
}

/**
 *	Устанавливает таймаут ожидания ответа от слейва
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\param responceTimeout таймаут ожидания ответа от слейва.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMB::SetTimeouts(int charTimeout, int responceTimeout)
{
	return trans_.SetTimeouts(charTimeout, responceTimeout);
}
