/*
	Author:		R.Gorohov (K1 SOFT)

	ComportMgr.cpp
	Определение класса ComPortMgr.
*/

#include "StdAfx.h"
#include "ComPortMgr.h"

#include "ErrorCodes.h"
#include "PortRs485.h"

/**
 *	Конструктор.
 */
ComPortMgr::ComPortMgr(void)
{
	InitializeCriticalSection(&cs_);

	defaultSettings_.baudrate = CBR_4800;
	defaultSettings_.byteSize = 8;
	defaultSettings_.charTimeout = 100;
	defaultSettings_.stopBits = ONESTOPBIT;
	defaultSettings_.parity = EVENPARITY;
}

/**
 *	Деструктор.
 */
ComPortMgr::~ComPortMgr(void)
{
	CloseAllPorts();
	DeleteCriticalSection(&cs_);
}

/**
 *	Устанавливает многопоточную блокировку.
 */
void ComPortMgr::Lock()
{
	EnterCriticalSection(&cs_);
}

/**
 *	Снимает многопоточную блокировку.
 */
void ComPortMgr::Unlock()
{
	LeaveCriticalSection(&cs_);
}

/**
 *	Закрывает все COM порты, перечисленные в таблице.
 */
void ComPortMgr::CloseAllPorts()
{
	std::map<std::wstring, PortRs485*>::iterator portIter;
	for(portIter = portTable_.begin(); portIter != portTable_.end(); ++portIter)
	{
		delete portIter->second;
	}
	portTable_.clear();
}

/**
 *	Открывает COM порт.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD ComPortMgr::Open(LPCTSTR portName)
{
	DWORD ret = OPERATION_COMPLETE;

	try
	{
		PortRs485* port = new PortRs485(portName);

		portTable_.insert(std::pair<std::wstring, PortRs485*>(std::wstring(portName), port));
		errorTable_.erase(std::wstring(portName));
	}
	catch(DWORD err)
	{
		ret = err;
		errorTable_.insert(std::pair<std::wstring, DWORD>(std::wstring(portName), err));
	}

	return ret;
}

/**
 *	Закрывает COM порт.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD ComPortMgr::Close(LPCTSTR portName)
{
	PortRs485* port = Find(portName);
	if( port )
	{
		portTable_.erase(std::wstring(portName));
		delete port;

		return OPERATION_COMPLETE;
	}

	return PORT_NOT_OPEN;
}

/**
 *	Читает из COM порта данные.
 *	\param portName имя порта.
 *	\param data указатель на буфер для приема данных.
 *	\param len размер буфера в байтах.
 *	\param nmbRead указатель на количество принятых байт.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD ComPortMgr::Read(LPCTSTR portName, BYTE* data, int len, int* nmbRead)
{
	PortRs485* port = Find(portName);
	if( port )
	{
		return port->Read(data, len, nmbRead);
	}

	return PORT_NOT_OPEN;
}

/**
 *	Записывает данные в COM порт.
 *	\param portName имя порта.
 *	\param data указатель на буфер с данными.
 *	\param len количество байт в буфере.
 *	\param nmbWritten указатель на количество записанных байт.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD ComPortMgr::Write(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten)
{
	PortRs485* port = Find(portName);
	if( port )
	{
		return port->Write(data, len, nmbWritten);
	}

	return PORT_NOT_OPEN;
}

/**
 *	Устанавливает настройки COM порта.
 *	\param portName имя порта.
 *	\param baudrate скорость порта.
 *	\param byteSize размер байта в битах.
 *	\param stopBits количество стоповых бит.
 *	\param parity настройки контроля четкости.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD ComPortMgr::Settings(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.baudrate = baudrate;
	sett.byteSize = byteSize;
	sett.stopBits = stopBits;
	sett.parity = parity;

	AddSettingsInTable(portName, sett);
	
	PortRs485* port = Find(portName);
	if( port )
	{
		
		return port->Settings(baudrate, byteSize, stopBits, parity);
	}

	return PORT_NOT_OPEN;
}

/**
 *	Устанавливает таймаут приема (записи) символа.
 *	\param portName имя порта.
 *	\param timeout значение таймаута в мс.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD ComPortMgr::SetTimeout(LPCTSTR portName, int timeout)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.charTimeout = timeout;

	AddSettingsInTable(portName, sett);
	
	PortRs485* port = Find(portName);
	if( port )
	{
		return port->SetTimeout(timeout);
	}

	return PORT_NOT_OPEN;
}

/**
 *	Возвращает код последней системной ошибки.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD ComPortMgr::GetSystemError(LPCTSTR portName)
{
	PortRs485* port = Find(portName);
	if( port )
		return port->GetSystemErrorCode();

	std::map<std::wstring, DWORD>::iterator iter = errorTable_.find(portName);
	if( iter != errorTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	Осуществляет поиск открытого COM порта в таблице.
 *	\param portName имя порта.
 *	\return указатель на класс PortRs485, если порт найден в таблице,
 *			0 в противном случае.
 */
PortRs485* ComPortMgr::Find(LPCTSTR portName)
{
	std::map<std::wstring, PortRs485*>::iterator iter = portTable_.find(std::wstring(portName));
	if( iter != portTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	Добавляет в таблицу настройки для заданного COM порта.
 *	\param portName имя порта.
 *	\param settings ссылка на настройки.
 */

void ComPortMgr::AddSettingsInTable(LPCTSTR portName, PortSettings& settings)
{
	portSettingsTable_.insert(std::pair<std::wstring, PortSettings>(std::wstring(portName), settings));
}

/**
 *	Возвращает настройки для заданного COM порта.
 *	\param portName имя порта.
 *	\return настройки порта из таблицы, если они есть в таблице или
  *			настройки по-умолчанию в противном случае.
 */
ComPortMgr::PortSettings ComPortMgr::GetSettingsFromTable(LPCTSTR portName)
{
	std::map<std::wstring, PortSettings>::iterator iter = portSettingsTable_.find(portName);
	if( iter == portSettingsTable_.end() )
		return defaultSettings_;
	else
		return iter->second;
}