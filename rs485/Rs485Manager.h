/*
	Author:		R.Gorohov (K1 SOFT)

	Rs485Manager.h
	Объявление класса Rs485Manager.
*/

#pragma once
#include "stdAfx.h"

#include <map>
#include <string>

#include "rs485.h"

#include "ComPortMgr.h"
#include "MasterMbMgr.h"
#include "SlaveMbMgr.h"

class PortRs485;
class MasterMB;
class SlaveMB;


/** Менеджер всех устройств на RS485. */
class Rs485Manager
{
public:
	/** Конструктор. */
	Rs485Manager(void);
	/** Деструктор. */
	~Rs485Manager(void);

	/** Открывает заданный COM-порт.  */
	DWORD OpenRs485(LPCTSTR portName);
	/** Закрывает заданный COM-порт. */
	DWORD CloseRs485(LPCTSTR portName);
	/** Читает из COM-порта данные. */
	DWORD ReadRs485(LPCTSTR portName, BYTE* data, int len, int* nmbRead);
	/** Записывает в COM-порт данные. */
	DWORD WriteRs485(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten);
	/** Настройки COM-порта. */
	DWORD SettingsRs485(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity);
	/** Настройка таймаута COM-порта. */
	DWORD SetTimeoutRs485(LPCTSTR portName, int timeout);

	
	/** Запускает мастер MODBUS. */
	DWORD StartMaster(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout);
	/** Посылает unicast-запрос мастера MODBUS. */
	DWORD SendRequestMaster(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen);
	/** Устанавливает настройки мастера MODBUS. */
	DWORD SettingsMaster(LPCTSTR portName, DWORD baudrate, BYTE parity);
	/** Завершает работу мастера MODBUS. */
	DWORD StopMaster(LPCTSTR portName);
	/** Устанавливает таймауты мастера MODBUS. */
	DWORD SetTimeoutsMaster(LPCTSTR portName, int charTimeout, int responceTimeout);

	/** Инициализирует указатель на callback-функцию получения ответа от слейва */
	DWORD SetReceiveMasterCallback(pReceiveMasterMBCallback callback);
	/** Инициализирует указатель на callback-функцию истечения таймаута ответа слейва. */
	DWORD SetTimeoutMasterCallback(pTimeoutMasterMBCallback callback);
	/** Инициализирует указатель на callback-функцию окончания пересылки данных мастером MODBUS. */
	DWORD SetSendMasterCallback(pSendMasterMBCallback callback);

	
	/** Запускает слейв MODBUS. */
	DWORD StartSlave(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout);
	/** Посылает ответ на запрос мастера MODBUS. */
	DWORD SendReplySlave(LPCTSTR portName, const BYTE* data, int dataLen);
	/** Устанавливает настройки слейва MODBUS. */
	DWORD SettingsSlave(LPCTSTR portName, DWORD baudrate, BYTE parity);
	/** Завершает работу слейва MODBUS. */
	DWORD StopSlave(LPCTSTR portName);
	/** Устанавливает таймаут на чтение/запись символа. */
	DWORD SetTimeoutSlave(LPCTSTR portName, int charTimeout);

	/** Инициализирует указатель на callback-функцию получения запроса от мастера MODBUS. */
	DWORD SetReceiveSlaveCallback(pReceiveSlaveMBCallback callback);
	/** Инициализирует указатель на callback-функцию окончания пересылки данных слейвом MODBUS. */
	DWORD SetSendSlaveCallback(pSendSlaveMBCallback callback);


	/** Возвращает код системной ошибки RS485. */
	DWORD GetSystemErrorRs485(LPCTSTR portName);
	/** Возвращает код системной ошибки мастера MODBUS. */
	DWORD GetSystemErrorMaster(LPCTSTR portName);
	/** Возвращает код системной ошибки слейва MODBUS. */
	DWORD GetSystemErrorSlave(LPCTSTR portName);

private:
	/** Менеждер COM портов RS485. */
	ComPortMgr	comPortManager_;
	/** Менеджер мастепров MODBUS. */
	MasterMbMgr masterMbManager_;
	/** Менеджер слейвов MODBUS. */
	SlaveMbMgr	slaveMbManager_;

};
