/*
	Author:		R.Gorohov (K1 SOFT)

	rs485.h
	Объявления экспортируемых функций библиотеки.
*/

#pragma once

/** Открывает заданный COM-порт. */
DWORD WINAPI OpenRS485(LPCTSTR portName);
/** Закрывает заданный COM-порт. */
DWORD WINAPI CloseRS485(LPCTSTR portName);
/** Читает из COM-порта данные. */
DWORD WINAPI ReadRS485(LPCTSTR portName, BYTE* data, int len, int* nmbRead);
/** Записывает в COM-порт данные. */
DWORD WINAPI WriteRS485(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten);
/** Устанавливает настройки COM-порта. */
DWORD WINAPI SettingsRS485(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity);
/** Устанавливает таймаут чтения (записи) COM-порта. */
DWORD WINAPI SetTimeoutRS485(LPCTSTR portName, int timeout);

// MODBUS функции

/** Тип callback-функции, вызываемой при получении ответа от слейва MODBUS */
typedef void (WINAPI *pReceiveMasterMBCallback)(LPCTSTR portName, BYTE slaveAddress, const BYTE* data, int len);
/** Тип callback-функции, вызываемой при истечении таймаута ожидания ответа от слейва MODBUS */
typedef void (WINAPI *pTimeoutMasterMBCallback)(LPCTSTR portName, BYTE address);
/** Тип callback-функции, вызываемой по окончании пересылки данных мастером MODBUS */
typedef void (WINAPI *pSendMasterMBCallback)(LPCTSTR portName);

/** Запускает мастер MODBUS. */
DWORD WINAPI StartMasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout);
/** Посылает unicast-запрос мастера MODBUS. */
DWORD WINAPI SendRequestMasterMB(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen);
/** Завершает работу мастера MODBUS. */
DWORD WINAPI StopMasterMB(LPCTSTR portName);
/** Устанавливает таймауты мастера MODBUS. */
DWORD WINAPI SetTimeoutsMasterMB(LPCTSTR portName, int charTimeout, int responceTimeout);
/** Устанавливает настройки мастера MODBUS. */
DWORD WINAPI SettingsMasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity);
/** Инициализирует указатель на callback-функцию получения ответа от слейва */
DWORD WINAPI SetReceiveMasterMBCallback(pReceiveMasterMBCallback callback);
/** Инициализирует указатель на callback-функцию истечения таймаута ответа слейва. */
DWORD WINAPI SetTimeoutMasterMBCallback(pTimeoutMasterMBCallback callback);
/** Инициализирует указатель на callback-функцию окончания пересылки данных мастером MODBUS. */
DWORD WINAPI SetSendMasterMBCallback(pSendMasterMBCallback callback);

/** Тип callback-функции, вызываемой при получении запроса от мастера MODBUS. */
typedef void (WINAPI *pReceiveSlaveMBCallback)(LPCTSTR portName, BYTE slaveAddress, const BYTE* data, int len);
/** Тип callback-функции, вызываемой по окончании пересылки данных слейвом MODBUS. */
typedef void (WINAPI *pSendSlaveMBCallback)(LPCTSTR portName);

/** Запускает слейв MODBUS. */
DWORD WINAPI StartSlaveMB(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout);
/** Посылает ответ на запрос мастера MODBUS. */
DWORD WINAPI SendReplySlaveMB(LPCTSTR portName, const BYTE* data, int dataLen);
/** Завершает работу слейва MODBUS. */
DWORD WINAPI StopSlaveMB(LPCTSTR portName);
/** Устанавливает настройки слейва MODBUS. */
DWORD WINAPI SettingsSlaveMB(LPCTSTR portName, DWORD baudrate, BYTE parity);
/** Устанавливает таймаут на чтение/запись символа. */
DWORD WINAPI SetTimeoutSlaveMB(LPCTSTR portName, int charTimeout);
/** Инициализирует указатель на callback-функцию получения запроса от мастера MODBUS. */
DWORD WINAPI SetReceiveSlaveMBCallback(pReceiveSlaveMBCallback callback);
/** Инициализирует указатель на callback-функцию окончания пересылки данных слейвом MODBUS. */
DWORD WINAPI SetSendSlaveMBCallback(pSendSlaveMBCallback callback);

/** Возвращает код системной ошибки RS485. */
DWORD WINAPI GetSystemErrorRS485(LPCTSTR portName);
/** Возвращает код системной ошибки мастера MODBUS. */
DWORD WINAPI GetSystemErrorMasterMB(LPCTSTR portName);
/** Возвращает код системной ошибки слейва MODBUS. */
DWORD WINAPI GetSystemErrorSlaveMB(LPCTSTR portName);


DWORD StartFrameTests();


// функции для отладки
typedef void (*pLogCallback)(const char*);
DWORD SetLogCallback(pLogCallback callback);

void LOG_Info(const char* info);
void LOG_Info(const char* info, const BYTE* data, int len);
void LOG_Info(const char* info, const char* param);
void LOG_Info(const char* info, const wchar_t* param);
void LOG_Error(const char* info, DWORD param1);


#include <string>

void ConvertWS2S(LPCTSTR src, std::string& dst);

