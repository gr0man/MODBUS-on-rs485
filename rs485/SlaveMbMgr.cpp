/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMbMgr.сзз
	Определение класса SlaveMbMgr.
*/

#include "StdAfx.h"
#include "SlaveMbMgr.h"
#include "SlaveMB.h"

#include "ErrorCodes.h"

/**
 *	Конструктор.
 */
SlaveMbMgr::SlaveMbMgr(void)
: receiveCallback_(0)
, sendCallback_(0)
{
	InitializeCriticalSection(&cs_);

	defaultSettings_.baudrate = CBR_4800;
	defaultSettings_.charTimeout = 100;
	defaultSettings_.parity = EVENPARITY;
}

/**
 *	Деструктор.
 */
SlaveMbMgr::~SlaveMbMgr(void)
{
	CloseAllPorts();
	DeleteCriticalSection(&cs_);
}

/**
 *	Устанавливает многопоточную блокировку.
 */
void SlaveMbMgr::Lock()
{
	EnterCriticalSection(&cs_);
}

/**
 *	Снимает многопоточную блокировку.
 */
void SlaveMbMgr::Unlock()
{
	LeaveCriticalSection(&cs_);
}

/**
 *	Закрывает все открытые COM-порты.
 */
void SlaveMbMgr::CloseAllPorts()
{
	std::map<std::wstring, SlaveMB*>::iterator slaveIter;
	for(slaveIter = slaveMbTable_.begin(); slaveIter != slaveMbTable_.end(); ++slaveIter)
	{
		delete slaveIter->second;
	}
	slaveMbTable_.clear();
}

/**
 *	Запускает слейв MODBUS.\
 *	\param portName имя порта.
 *	\param adddress адрес слейва.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMbMgr::Start(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout)
{
	DWORD ret = OPERATION_COMPLETE;

	try
	{
		SlaveMB* slave = new SlaveMB(portName, address, baudrate, parity, charTimeout, processor_);

		slaveMbTable_.insert(std::pair<std::wstring, SlaveMB*>(std::wstring(portName), slave));
		errorTable_.erase(std::wstring(portName));

		PortSettings sett;
		sett.baudrate = baudrate;
		sett.parity = parity;
		sett.charTimeout = charTimeout;

		AddSettingsInTable(portName, sett);

	}
	catch(DWORD err)
	{
		ret = err;
		errorTable_.insert(std::pair<std::wstring, DWORD>(std::wstring(portName), err));
	}

	return ret;
}

/**
 *	Завершает работу слейва MODBUS.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMbMgr::Stop(LPCTSTR portName)
{
	LOG_Info("Stop SlaveMB on port \"%s\"\n", portName);
	SlaveMB* slave = Find(portName);

	if( slave )
	{
		slaveMbTable_.erase(std::wstring(portName));
		delete slave;

		return OPERATION_COMPLETE;
	}

	return PORT_NOT_OPEN;
}

/**
 *	Посылает ответ на запрос мастера MODBUS.
 *	\param portName имя порта.
 *	\param data указатель на массив с данными.
 *	\param dataLen размер массива в байтах.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMbMgr::SendReply(LPCTSTR portName, const BYTE* data, int dataLen)
{
	SlaveMB* slave = Find(portName);

	if( slave )
		return slave->SendReply(data, dataLen);

	return PORT_NOT_OPEN;
}

/**
 *	Устанавливает настройки слейва MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMbMgr::Settings(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.baudrate = baudrate;
	sett.parity = parity;
	AddSettingsInTable(portName, sett);

	SlaveMB* slave = Find(portName);

	if( slave )
		return slave->Settings(baudrate, parity);
		
	return PORT_NOT_OPEN;
}

/**
 *	Устанавливает таймаут на чтение/запись символа.
 *	\param portName имя порта.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMbMgr::SetTimeout(LPCTSTR portName, int charTimeout)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.charTimeout = charTimeout;
	AddSettingsInTable(portName, sett);

	SlaveMB* slave = Find(portName);

	if( slave )
		return slave->SetTimeout(charTimeout);

	return PORT_NOT_OPEN;
}

/**
 *	Инициализирует указатель на callback-функцию получения запроса от мастера MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMbMgr::SetReceiveCallback(pReceiveSlaveMBCallback callback)
{
	receiveCallback_ = callback;
	processor_.SetReceiveCallback(callback);
	return OPERATION_COMPLETE;
}

/**
 *	Инициализирует указатель на callback-функцию окончания пересылки данных слейвом MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD SlaveMbMgr::SetSendCallback(pSendSlaveMBCallback callback)
{
	sendCallback_ = callback;
	processor_.SetSendCallback(callback);
	return OPERATION_COMPLETE;
}

/**
 *	Возвращает код системной ошибки.
 *	\param portName имя порта.
 *	\return код системной ошибки.
 */
DWORD SlaveMbMgr::GetSystemError(LPCTSTR portName)
{
	SlaveMB* slave = Find(portName);
	if( slave )
		return slave->GetSystemErrorCode();

	std::map<std::wstring, DWORD>::iterator iter = errorTable_.find(portName);
	if( iter != errorTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	Ищет слейв MODBUS в таблице по имени COM порта.
 *	\return Указатель на экземпляр класса SlaveMB или 0.
 */
SlaveMB* SlaveMbMgr::Find(LPCTSTR portName)
{
	std::map<std::wstring, SlaveMB*>::iterator iter = slaveMbTable_.find(std::wstring(portName));
	if( iter != slaveMbTable_.end() )
	{
		return iter->second;
	}
	return 0;
}

/**
 *	Добавляет настройки слейва MODBUS в таблицу.
 *	\param portName имя порта.
 *	\param settyings настройки слейва MODBUS.
 */
void SlaveMbMgr::AddSettingsInTable(LPCTSTR portName, PortSettings& settings)
{
	settingsTable_.insert(std::pair<std::wstring, PortSettings>(std::wstring(portName), settings));
}

/**
 *	Возвращает настройки слейва MODBUS из таблицы для заданного COM порта.
 *	\param portName имя порта.
 *	\return настройки слейва MODBUS.
 */
SlaveMbMgr::PortSettings SlaveMbMgr::GetSettingsFromTable(LPCTSTR portName)
{
	std::map<std::wstring, PortSettings>::iterator iter = settingsTable_.find(portName);
	if( iter == settingsTable_.end() )
		return defaultSettings_;
	else
		return iter->second;
}

bool SlaveMbMgr::IsExist(LPCTSTR portName)
{ 
	return (Find(portName)!=0); 
}