/*
	Author:		R.Gorohov (K1 SOFT)

	PortRs485.h
	Объявление класса portRs485.
*/

#pragma once

#include <string>

/** Релизует функционал по работе с Rs485. */
class PortRs485
{
public:
	/** Конструктор. */
	PortRs485(LPCTSTR portName);
	/** Деструктор. */
	virtual ~PortRs485(void);

	/** Возвращает код последней системной ошибки. */
	inline DWORD GetSystemErrorCode(){ return sysErrorCode_; }

	/** Читает данные из rs485. */
	DWORD Read(BYTE* data, int len, int* nmbRead);
	/** Записывает данные в rs485. */
	DWORD Write(const BYTE* data, int len, int* nmbWritten);
	/** Устанавливает настройки порта. */
	DWORD Settings(DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity);
	/** Устанавливает таймаут чтения (записис) порта. */
	DWORD SetTimeout(int timeout);
	/** Возвращает имя порта. */
	const std::wstring& GetPortName(){ return portName_; }

	/** Закрывает порт. */
	void Close();

	/**  */
	bool IsClosed(int timeout=0);

private:

	/** Открывает порт. */
	void Open();
	/** Код последней системной ошибки. */
	DWORD sysErrorCode_;

	/** Идентификатор открытого порта. */
	HANDLE hPort_;
	/** Событие окончания записи в COM-порт. */
	HANDLE hWriteEvent_;
	/** Событие окончания чтения из COM-порта. */
	HANDLE hReadEvent_;
	/** Событие завершения операций. */
	HANDLE hCloseEvent_;
	/** Структура OVERLAPPED для чтения. */
	OVERLAPPED ovRead_;
	/** Структура OVERLAPPED для записи. */
	OVERLAPPED ovWrite_;
	/** Имя порта. */	
	std::wstring portName_;
	/** Таймаут на 1 символ */
	int timeout_;
};
