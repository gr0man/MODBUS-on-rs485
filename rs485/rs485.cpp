/*
	Author:		R.Gorohov (K1 SOFT)

	rs485.cpp
	Определения экспортируемых функций библиотеки.
*/

#include "stdafx.h"

#include <map>

#include "rs485.h"
#include "PortRs485.h"
#include "MasterMB.h"
#include "SlaveMB.h"

#include "errorCodes.h"

#include "Rs485Manager.h"

#include "FrameMB.h"


Rs485Manager manager;

/**
 *	Открывает заданный COM-порт.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI OpenRS485(LPCTSTR portName)
{
	return manager.OpenRs485(portName);
}

/**
 *	Закрывает заданный COM-порт.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI CloseRS485(LPCTSTR portName)
{
	return manager.CloseRs485(portName);
}

/**
 *	Читает из COM-порта данные.
 *	\param portName имя порта.
 *	\param data указатель на буфер для приема данных.
 *	\param len размер приемного буфера.
 *	\param nmbRead указатель на переменную, в котолрой сохраняется количество прочитанных байт.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI ReadRS485(LPCTSTR portName, BYTE* data, int len, int* nmbRead)
{
	return manager.ReadRs485(portName, data, len, nmbRead);
}

/**
 *	Записывает в COM-порт данные. 
 *	\param portName имя порта.
 *	\param data указатель на данных для записи.
 *	\param len размер буфера.
 *	\param nmbRead указатель на переменную, в котолрой сохраняется количество записанных байт.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI WriteRS485(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten)
{
	return manager.WriteRs485(portName, data, len, nmbWritten);
}

/**
 *	Устанавливает настройки COM-порта.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param byteSize размер байта в битах.
 *	\param stopBits количество стоповых бит.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SettingsRS485(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity)
{
	return manager.SettingsRs485(portName, baudrate, byteSize, stopBits, parity);
}

/**
 *	Устанавливает таймаут чтения (записи) COM-порта.
 *	\param portName имя порта.
 *	\param timeout таймаут чтения (записис) порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SetTimeoutRS485(LPCTSTR portName, int timeout)
{
	return manager.SetTimeoutRs485(portName, timeout);
}


// MODBUS функции

/**
 *	Запускает мастер MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\param responceTimeout время ожидания ответа от слейва.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI StartMasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout)
{
	return manager.StartMaster(portName, baudrate, parity, charTimeout, responceTimeout);
}

/**
 *	Посылает unicast-запрос мастера MODBUS.
 *	\param portName имя порта.
 *	\param address адрес получателя.
 *	\param data указатель на массив с данными.
 *	\param dataLen размер массива в байтах.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SendRequestMasterMB(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen)
{
	return manager.SendRequestMaster(portName, address, data, dataLen);
}

/**
 *	Устанавливает таймауты мастера MODBUS.
 *	\param portName имя порта.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\param responceTimeout время ожидания ответа от слейва.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SetTimeoutsMasterMB(LPCTSTR portName, int charTimeout, int responceTimeout)
{
	return manager.SetTimeoutsMaster(portName, charTimeout, responceTimeout);
}

/**
 *	Завершает работу мастера MODBUS.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI StopMasterMB(LPCTSTR portName)
{
	return manager.StopMaster(portName);
}

/**
 *	Устанавливает настройки мастера MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SettingsMasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	return manager.SettingsMaster(portName, baudrate, parity);
}

/**
 *	Инициализирует указатель на callback-функцию получения ответа от слейва.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SetReceiveMasterMBCallback(pReceiveMasterMBCallback callback)
{
	return manager.SetReceiveMasterCallback(callback);
}

/**
 *	Инициализирует указатель на callback-функцию истечения таймаута ответа слейва.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SetTimeoutMasterMBCallback(pTimeoutMasterMBCallback callback)
{
	return manager.SetTimeoutMasterCallback(callback);
}

/**
 *	Инициализирует указатель на callback-функцию окончания пересылки данных мастером MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SetSendMasterMBCallback(pSendMasterMBCallback callback)
{
	return manager.SetSendMasterCallback(callback);
}


/**
 *	Запускает слейв MODBUS.
 *	\param portName имя порта.
 *	\param adddress адрес слейва.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI StartSlaveMB(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout)
{
	return manager.StartSlave(portName, address, baudrate, parity, charTimeout);
}

/**
 *	Посылает ответ на запрос мастера MODBUS.
 *	\param portName имя порта.
 *	\param data указатель на массив с данными.
 *	\param dataLen размер массива в байтах.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SendReplySlaveMB(LPCTSTR portName, const BYTE* data, int dataLen)
{
	return manager.SendReplySlave(portName, data, dataLen);
}

/**
 *	Завершает работу слейва MODBUS.
 *	\param portName имя порта.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI StopSlaveMB(LPCTSTR portName)
{
	return manager.StopSlave(portName);
}

/**
 *	Устанавливает настройки слейва MODBUS.
 *	\param portName имя порта.
 *	\param baudrate скорость обмена.
 *	\param parity контроль четности.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SettingsSlaveMB(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	return manager.SettingsSlave(portName, baudrate, parity);
}

/**
 *	Устанавливает таймаут на чтение/запись символа.
 *	\param portName имя порта.
 *	\param charTimeout таймаут чтения (записи) символа.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SetTimeoutSlaveMB(LPCTSTR portName, int charTimeout)
{
	return manager.SetTimeoutSlave(portName, charTimeout);
}

/**
 *	Инициализирует указатель на callback-функцию получения запроса от мастера MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SetReceiveSlaveMBCallback(pReceiveSlaveMBCallback callback)
{
	return manager.SetReceiveSlaveCallback(callback);
}

/**
 *	Инициализирует указатель на callback-функцию окончания пересылки данных слейвом MODBUS.
 *	\param адрес callback-функции.
 *	\return код ошибки (\see ErrorCode).
 */
DWORD WINAPI SetSendSlaveMBCallback(pSendSlaveMBCallback callback)
{
	return manager.SetSendSlaveCallback(callback);
}


/**
 *	Возвращает код системной ошибки RS485.
 *	\param portName имя порта.
 *	\return код системной ошибки.
 */
DWORD WINAPI GetSystemErrorRS485(LPCTSTR portName)
{
	return manager.GetSystemErrorRs485(portName);
}

/**
 *	Возвращает код системной ошибки мастера MODBUS.
 *	\param portName имя порта.
 *	\return код системной ошибки.
 */
DWORD WINAPI GetSystemErrorMasterMB(LPCTSTR portName)
{
	return manager.GetSystemErrorMaster(portName);
}

/**
 *	Возвращает код системной ошибки слейва MODBUS.
 *	\param portName имя порта.
 *	\return код системной ошибки.
 */
DWORD WINAPI GetSystemErrorSlaveMB(LPCTSTR portName)
{
	return manager.GetSystemErrorSlave(portName);
}



//////////////////////////////////////////////////////////////
// отладочные
pLogCallback logCallback(0);

DWORD SetLogCallback(pLogCallback callback)
{
	logCallback = callback;
	//if(logCallback)
	//	MessageBox(NULL, L"SetLogCallback", L"Info", MB_OK);
	return 0;
}

void Log(const char* info)
{
	if(logCallback)
		logCallback(info);
}


void LOG_Info(const char* info)
{
#ifdef USE_LOG	
	Log(info);
#endif
}

void LOG_Info(const char* info, const BYTE* data, int len)
{
#ifdef USE_LOG
	char buffer[20];
	std::string str(info);
	int i;
	for(i=0; i<len; ++i)
	{
		sprintf_s(buffer, 20, " %02X", data[i]);
		str += std::string(buffer);
	}
	str += "\n";
	Log(str.c_str());
#endif
}

void LOG_Info(const char* info, const char* param)
{
#ifdef USE_LOG
	char buffer[2000];
	sprintf_s(buffer, 2000, info, param);
	Log(buffer);
#endif
}

void LOG_Info(const char* info, const wchar_t* param)
{
#ifdef USE_LOG
	std::string str;
	ConvertWS2S(param, str);
	char buffer[2000];
	sprintf_s(buffer, 2000, info, str.c_str());
	Log(buffer);
#endif
}

void LOG_Error(const char* info, DWORD param1)
{
#ifdef USE_LOG
	char buffer[2000];
	sprintf_s(buffer, 2000, info, param1);
	Log(buffer);
#endif
}

void ConvertWS2S(LPCTSTR src, std::string& dst)
{
	char buf[1000];	
	std::wstring srcStr(src);
	wcstombs(buf, srcStr.c_str(), srcStr.size()+1);
	dst = std::string(buf);
}

#include "FrameMB_Tests.h"

DWORD StartFrameTests()
{
	FrameMB_Tests tests;

	tests.FrameFromBinary();
	tests.FrameFromAscii();

	return 0;
}