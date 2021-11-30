/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMbMgr.h
	Объявление класса SlaveMbMgr.
*/

#pragma once

#include <string>
#include <map>

#include "rs485.h"
#include "SlaveMbProcessor.h"

class SlaveMB;

class SlaveMbMgr
{
	/** Настройки слейва MODBUS. */
	struct PortSettings
	{
		DWORD baudrate;
		BYTE parity;
		int charTimeout;
	};

	PortSettings defaultSettings_;

public:
	/** Конструктор. */
	SlaveMbMgr(void);
	/** Деструктор. */
	~SlaveMbMgr(void);

	/** Закрывает все открытые COM-порты. */
	void CloseAllPorts();

	/** Запускает слейв MODBUS. */
	DWORD Start(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout);
	/** Завершает работу слейва MODBUS. */
	DWORD Stop(LPCTSTR portName);
	/** Посылает ответ на запрос мастера MODBUS. */
	DWORD SendReply(LPCTSTR portName, const BYTE* data, int dataLen);
	/** Устанавливает настройки слейва MODBUS. */
	DWORD Settings(LPCTSTR portName, DWORD baudrate, BYTE parity);
	/** Устанавливает таймаут на чтение/запись символа. */
	DWORD SetTimeout(LPCTSTR portName, int charTimeout);

	/** Инициализирует указатель на callback-функцию получения запроса от мастера MODBUS. */
	DWORD SetReceiveCallback(pReceiveSlaveMBCallback callback);
	/** Инициализирует указатель на callback-функцию окончания пересылки данных слейвом MODBUS. */
	DWORD SetSendCallback(pSendSlaveMBCallback callback);

	/** Возвращает код системной ошибки. */
	DWORD GetSystemError(LPCTSTR portName);

	/** Проверяет, открыт ли слейв MODBUS на заданном COM порту. */
	bool IsExist(LPCTSTR portName);//{ return (Find(portName)!=0); }
	/** Завершает работу слейва на заданном COM порту и удаляет его из таблицы. */
	void Remove(LPCTSTR portName);

	/** Устанавливает многопоточную блокировку. */
	void Lock();
	/** Снимает многопоточную блокировку. */
	void Unlock();

private:
	/** Обработчик команд мастера MODBUS. */
	SlaveMbProcessor processor_;

	/** Таблица портов RS485, на которых открыты MODBUS слейв'ы */
	std::map<std::wstring, SlaveMB*> slaveMbTable_;
	/** Таблица последних системных ошибок. */
	std::map<std::wstring, DWORD> errorTable_;
	/** Таблица настроек слейвов MODBUS. */
	std::map<std::wstring, PortSettings> settingsTable_;

	/** Элемент многопоточной синхронизации. */
	CRITICAL_SECTION cs_;

	/** Указатель на callback-функцию получения запроса от мастера. */
	pReceiveSlaveMBCallback receiveCallback_;
	/** Указатель на callback-функцию окончания пересылки данных слейвом MODBUS. */
	pSendSlaveMBCallback sendCallback_;

	/** Добавляет настройки слейва MODBUS в таблицу. */
	void AddSettingsInTable(LPCTSTR portName, PortSettings& settings);
	/** Возвращает настройки слейва MODBUS из таблицы для заданного COM порта. */
	PortSettings GetSettingsFromTable(LPCTSTR portName);
	/** Ищет слейв MODBUS в таблице по имени COM порта. */
	SlaveMB* Find(LPCTSTR portName);
};
