/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMB.h
	Объявления классов SlaveBehaviorMB и SlaveMB.
*/

#pragma once

#include "PortRs485.h"
#include <vector>

#include "CommandQueue.h"

#include "TransieverMB.h"
#include "BehaviorMB.h"
#include "CmdProcessor.h"

/** Инкапсулирует функционал по проверке принимаемых фреймов слейвом MODBUS. */
class SlaveBehaviorMB : public BehaviorMB
{
public:
	/** Конструктор. */
	SlaveBehaviorMB(BYTE address):address_(address){};
	/** Деструктор. */
	virtual ~SlaveBehaviorMB(void){};

	/** Устанавливает таймаут ожидания ответа. */
	void SetResponceTimeout(int responceTimeout){};
	/** Фиксирует окончание отправки фрейма. */
	void FinishSend(BYTE address){};

	void SetAddress(BYTE address){ address_ = address; }
	
	/** Проверяет праильность адреса принятого фрейма. */
	virtual bool IsAddressValid(BYTE address);
	/** Проверяет, не истекло ли время ожидания ответа. */
	virtual bool IsTimeoutExpiration(){ return false; }
	/** Сбрасывает параметры ожидания. */
	void ResetSettings(){};

	virtual bool IsNeedToRead(){ return true; }

	virtual void FinishReceive(BYTE address){};
	virtual void ReceiveTimeout(){};
private:
	BYTE address_;
};

/** Инкапсулирует функционал слейва MODBUS на RS485. */
class SlaveMB
{
public:
	/** Конструктор. */
	SlaveMB(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout, CmdProcessor& processor);
	/** Деструктор. */
	virtual ~SlaveMB(void);

	/** Устанавливает адрес слейва MODBUS. */
	DWORD SetAddress(BYTE address);
	/** Инициализирует посылку ответа слейвом MODBUS. */
	DWORD SendReply(const BYTE* data, int dataLen);
	/** Изменяет настройки RS485. */
	DWORD Settings(DWORD baudrate, BYTE parity);
	/** Устанавливает таймаут чтения символа. */
	DWORD SetTimeout(int charTimeout){ return trans_.SetTimeouts(charTimeout, 0); }
	/** Возвращает код последней системной ошибки. */
	DWORD GetSystemErrorCode(){ return trans_.GetSystemErrorCode(); }

private:
	/** Валидатор принятых фреймов. */
	SlaveBehaviorMB behavior_;
	/** Приемопередатчик MODBUS. */
	TransieverMB trans_;
	/** Адрес слейва MODBUS. */
	BYTE address_;
};
