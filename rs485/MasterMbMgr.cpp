/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMbMgr.cpp
	Определение класса MasterMbMgr.
*/

#include "StdAfx.h"
#include "MasterMbMgr.h"

#include "MasterMB.h"

#include "ErrorCodes.h"

/**
 *	Конструктор.
 */
MasterMbMgr::MasterMbMgr(void)
: receiveCallback_(0)
, timeoutCallback_(0)
, sendCallback_(0)
{
	InitializeCriticalSection(&cs_);

	defaultSettings_.baudrate = CBR_4800;
	defaultSettings_.charTimeout = 100;
	defaultSettings_.responceTimeout = 10000;
	defaultSettings_.parity = EVENPARITY;
}

/**
 *	Деструктор.
 */
MasterMbMgr::~MasterMbMgr(void)
{
	CloseAllPorts();
	DeleteCriticalSection(&cs_);
}

/**
 *	Устанавливает многопоточную блокировку.
 */
void MasterMbMgr::Lock()
{
	EnterCriticalSection(&cs_);
}

/**
 *	Снимает многопоточную блокировку.
 */
void MasterMbMgr::Unlock()
{
	LeaveCriticalSection(&cs_);
}

/**
 *	Закрывает все открытые COM-порты.
 */
void MasterMbMgr::CloseAllPorts()
{
	std::map<std::wstring, MasterMB*>::iterator masterIter;
	for(masterIter = masterMbTable_.begin(); masterIter != masterMbTable_.end(); ++masterIter)
	{
		delete masterIter->second;
	}
	masterMbTable_.clear();
}

/**
 *	Запускает мастер MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\param responceTimeout время ожидания ответа от слейва.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMbMgr::Start(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout)
{
	DWORD ret = OPERATION_COMPLETE;

	try
	{
		MasterMB* master = new MasterMB(portName, baudrate, parity, charTimeout, responceTimeout, processor_);

		masterMbTable_.insert(std::pair<std::wstring, MasterMB*>(std::wstring(portName), master));
		errorTable_.erase(std::wstring(portName));

		PortSettings sett;
		sett.baudrate = baudrate;
		sett.parity = parity;
		sett.charTimeout = charTimeout;
		sett.responceTimeout = responceTimeout;

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
 *	Завершает работу мастера MODBUS.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMbMgr::Stop(LPCTSTR portName)
{
	MasterMB* master = Find(portName);

	if( master )
	{

		masterMbTable_.erase(std::wstring(portName));
		LOG_Info("Delete MasterMB on port \"%s\"\n", portName);		
		delete master;

		return OPERATION_COMPLETE;
	}

	return PORT_NOT_OPEN;
}

/**
 *	Посылает unicast-запрос мастера MODBUS.
 *	\param portName имя порта.
 *	\param address адрес получателя.
 *	\param data указатель на массив с данными.
 *	\param dataLen размер массива в байтах.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMbMgr::SendRequest(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen)
{
	MasterMB* master = Find(portName);

	if( master )
		return master->SendRequest(address, data, dataLen);

	return PORT_NOT_OPEN;
}

/**
*	Устанавливает настройки мастера MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMbMgr::Settings(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.baudrate = baudrate;
	sett.parity = parity;
	AddSettingsInTable(portName, sett);

	MasterMB* master = Find(portName);

	if( master )
		return master->Settings(baudrate, parity);

	return PORT_NOT_OPEN;
}

/**
 *	Устанавливает таймауты мастера MODBUS.
 *	\param portName имя порта.\
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\param responceTimeout время ожидания ответа от слейва.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMbMgr::SetTimeouts(LPCTSTR portName, int charTimeout, int responceTimeout)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.charTimeout = charTimeout;
	sett.responceTimeout = responceTimeout;
	AddSettingsInTable(portName, sett);

	MasterMB* master = Find(portName);

	if( master )
		return master->SetTimeouts(charTimeout, responceTimeout);

	return PORT_NOT_OPEN;
}

/**
 *	Инициализирует указатель на callback-функцию получения ответа от слейва.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMbMgr::SetReceiveCallback(pReceiveMasterMBCallback callback)
{
	receiveCallback_ = callback;
	processor_.SetReceiveCallback(callback);
	return OPERATION_COMPLETE;
}

/**
 *	Инициализирует указатель на callback-функцию истечения таймаута ответа слейва.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMbMgr::SetTimeoutCallback(pTimeoutMasterMBCallback callback)
{
	timeoutCallback_ = callback;
	processor_.SetTimeoutCallback(callback);
	return OPERATION_COMPLETE;
}

/**
 *	Инициализирует указатель на callback-функцию окончания пересылки данных мастером MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD MasterMbMgr::SetSendCallback(pSendMasterMBCallback callback)
{
	sendCallback_ = callback;
	processor_.SetSendCallback(callback);
	return OPERATION_COMPLETE;
}

/**
 *	Возвращает код последней системной ошибки.
 *	\param portName имя порта.
 *	\return код системной ошибки.
 */
DWORD MasterMbMgr::GetSystemError(LPCTSTR portName)
{
	MasterMB* master = Find(portName);
	if( master )
		return master->GetSystemErrorCode();

	std::map<std::wstring, DWORD>::iterator iter = errorTable_.find(portName);
	if( iter != errorTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	Ищет мастер MODBUS в таблице по имени COM порта.
 *	\param portName имя порта.
 *	\return Указатель на экземпляр класса MasterMB, или 0.
 */
MasterMB* MasterMbMgr::Find(LPCTSTR portName)
{
	std::map<std::wstring, MasterMB*>::iterator iter = masterMbTable_.find(std::wstring(portName));
	if( iter != masterMbTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	Добавляет настройки мастера MODBUS в таблицу.
 *	\param portName имя порта.
 *	\param settings настройки мастера MODBUS.
 */
void MasterMbMgr::AddSettingsInTable(LPCTSTR portName, PortSettings& settings)
{
	settingsTable_.insert(std::pair<std::wstring, PortSettings>(std::wstring(portName), settings));
}

/**
 *	Возвращает настройки мастера MODBUS из таблицы.
 *	\param portName имя порта.
 *	\return настройки мастера MODBUS.
 */
MasterMbMgr::PortSettings MasterMbMgr::GetSettingsFromTable(LPCTSTR portName)
{
	std::map<std::wstring, PortSettings>::iterator iter = settingsTable_.find(portName);
	if( iter == settingsTable_.end() )
		return defaultSettings_;
	else
		return iter->second;
}