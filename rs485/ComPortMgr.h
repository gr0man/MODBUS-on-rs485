/*
	Author:		R.Gorohov (K1 SOFT)

	ComportMgr.h
	Объявление класса ComPortMgr.
*/

#pragma once

#include <string>
#include <map>

class PortRs485;

/** Диспетчер устройств RS485, открытых как COM-порты. */
class ComPortMgr
{
	/** Настройки COM-порта. */
	struct PortSettings
	{
		/** Скорость порта. */
		DWORD baudrate;
		/** Размер байта в битах. */
		BYTE byteSize;
		/** Количество стоповых бит. */
		BYTE stopBits;
		/** настройки проверки четности. */
		BYTE parity;
		/** Таймаут чтения (записи) символа. */
		int charTimeout;
	};

	/** Настройки по-умолчанию. */
	PortSettings defaultSettings_;

public:
	/** Конструктор. */
	ComPortMgr(void);
	/** Деструктор. */
	~ComPortMgr(void);

	/** Закрывает все открытые COM-порты. */
	void CloseAllPorts();

	/** Открывает указанный COM порт. */
	DWORD Open(LPCTSTR portName);
	/** Закрывает указанный COM порт.*/
	DWORD Close(LPCTSTR portName);
	/** Читает данные из COM порта. */
	DWORD Read(LPCTSTR portName, BYTE* data, int len, int* nmbRead);
	/** Записывает данные в COM порт. */
	DWORD Write(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten);
	/** Устанавливает настройки COM порта. */
	DWORD Settings(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity);
	/** Устанавливает таймаут на чтение (запись) символа. */
	DWORD SetTimeout(LPCTSTR portName, int timeout);

	/** Возвращает код последней системной ошибки. */
	DWORD GetSystemError(LPCTSTR portName);
	/** Проверяет, открыт ли заданный COM порт. */
	bool IsExist(LPCTSTR portName){ return (Find(portName)!=0); }
	/** Закрывает COM порт и удаляет его из таблицы. */
	void Remove(LPCTSTR portName);

	/** Устанавливает многопоточную блокировку. */
	void Lock();
	/** Снимает многопоточную блокировку. */
	void Unlock();

private:
	/** Таблица портов RS485, открытых как COM-порты. */
	std::map<std::wstring, PortRs485*> portTable_;
	/** Таблица последних системных ошибок. */
	std::map<std::wstring, DWORD> errorTable_;
	/** Таблица настроек COM портов. */
	std::map<std::wstring, PortSettings> portSettingsTable_;

	/** Элемент многопоточной синхронизации. */
	CRITICAL_SECTION cs_;

	/** Добавляет настройки COM порта в таблицу. */
	void AddSettingsInTable(LPCTSTR portName, PortSettings& settings);
	/** Возвращает настройки из таблицы для заданного COM порта. */
	PortSettings GetSettingsFromTable(LPCTSTR portName);

	/** Добавляет COM порт в таблицу. */
	void Add(LPCTSTR portName, PortRs485* port);
	/** Ищет COM порт в таблице по имени. */
	PortRs485* Find(LPCTSTR portName);
};
