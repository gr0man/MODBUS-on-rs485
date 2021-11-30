/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMbMgr.h
	Объявление класса MasterMbMgr.
*/

#pragma once

#include <map>
#include <string>

#include "rs485.h"
#include "MasterMbProcessor.h"

class MasterMB;

class MasterMbMgr
{
	/** Настройки мастера MODBUS. */
	struct PortSettings
	{
		DWORD baudrate;
		BYTE parity;
		int charTimeout;
		int responceTimeout;
	};

	PortSettings defaultSettings_;

public:
	/** Конструктор. */
	MasterMbMgr(void);
	/** Деструктор. */
	~MasterMbMgr(void);

	/** Закрывает все открытые COM-порты. */
	void CloseAllPorts();

	/** Запускает мастер MODBUS. */
	DWORD Start(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout);
	/** Завершает работу мастера MODBUS. */
	DWORD Stop(LPCTSTR portName);
	/** Устанавливает настройки мастера MODBUS. */
	DWORD Settings(LPCTSTR portName, DWORD baudrate, BYTE parity);
	/** Посылает unicast-запрос мастера MODBUS. */
	DWORD SendRequest(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen);
	/** Устанавливает таймауты мастера MODBUS. */
	DWORD SetTimeouts(LPCTSTR portName, int charTimeout, int responceTimeout);

	/** Инициализирует указатель на callback-функцию получения ответа от слейва. */
	DWORD SetReceiveCallback(pReceiveMasterMBCallback callback);
	/** Инициализирует указатель на callback-функцию истечения таймаута ответа слейва. */
	DWORD SetTimeoutCallback(pTimeoutMasterMBCallback callback);
	/** Инициализирует указатель на callback-функцию окончания пересылки данных мастером MODBUS. */
	DWORD SetSendCallback(pSendMasterMBCallback callback);

	//! Возвращает код системной ошибки.
	DWORD GetSystemError(LPCTSTR portName);

	/** Проверяет, открыт ли мастер MODBUS на заданном COM порту. */
	bool IsExist(LPCTSTR portName){ return (Find(portName)!=0); }
	/** Завершает работу мастера на заданном COM порту и удаляет его из таблицы. */
	void Remove(LPCTSTR portName);

	/** Устанавливает многопоточную блокировку. */
	void Lock();
	/** Снимает многопоточную блокировку. */
	void Unlock();

private:
	/** Обработчик команд мастера MODBUS. */
	MasterMbProcessor processor_;

	/** Таблица портов RS485, на которых открыты MODBUS мастер'ы */
	std::map<std::wstring, MasterMB*> masterMbTable_;
	/** Таблица последних системных ошибок. */
	std::map<std::wstring, DWORD> errorTable_;
	/** Таблица настроек мастеров MODBUS. */
	std::map<std::wstring, PortSettings> settingsTable_;

	/** Элемент многопоточной синхронизации. */
	CRITICAL_SECTION cs_;
	
	/** Указатель на callback-функцию получения ответа от слейва. */
	pReceiveMasterMBCallback receiveCallback_;
	/** Указатель на callback-функцию истечения таймаута ответа слейва. */
	pTimeoutMasterMBCallback timeoutCallback_;
	/** Указатель на callback-функцию окончания пересылки данных мастером MODBUS. */
	pSendMasterMBCallback	 sendCallback_;
	
	/** Добавляет настройки мастера MODBUS в таблицу. */
	void AddSettingsInTable(LPCTSTR portName, PortSettings& settings);
	/** Возвращает настройки мастера MODBUS из таблицы для заданного COM порта. */
	PortSettings GetSettingsFromTable(LPCTSTR portName);
	/** Ищет мастер MODBUS в таблице по имени COM порта. */
	MasterMB* Find(LPCTSTR portName);
};
