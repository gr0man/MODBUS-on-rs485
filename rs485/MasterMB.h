/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMB.h
	Объявления классов MasterBehaviorMB и MasterMB.
*/

#pragma once

#include "PortRs485.h"
#include "CommandQueue.h"
#include <vector>

#include "BehaviorMB.h"

#include "TransieverMB.h"

/** Инкапсулирует функционал по проверке принимаемых фреймов мастером MODBUS. */
class MasterBehaviorMB : public BehaviorMB
{
public:
	/** Конструктор. */
	MasterBehaviorMB(void);
	/** Деструктор. */
	virtual ~MasterBehaviorMB(void){};

	/** Устанавливает таймаут ожидания ответа от слейва. */
	void SetResponceTimeout(int responceTimeout){ responceTimeout_ = responceTimeout; }
	/** Фиксирует окончание отправки фрейма. */
	void FinishSend(BYTE address);
	
	/** Проверяет праильность адреса принятого фрейма. */
	bool IsAddressValid(BYTE address);
	/** Проверяет, не истекло ли время ожидания ответа. */
	bool IsTimeoutExpiration();

	/** Сбрасывает параметры ожидания. */
	void ResetSettings();

	bool IsNeedToRead(){ return isNeedToRead_; }

	void FinishReceive(BYTE address);
	void ReceiveTimeout(){ isNeedToRead_ = false; }
private:
	/** Таймаут ожидания ответа. */
	int responceTimeout_;
	/** Ожидаемый адрес. */
	BYTE slaveAddress_;
	/** Завиксированное время окончания посылки запроса. */
	ULONGLONG requestTime_;

	bool isNeedToRead_;
};

/** Инкапсулирует функционал мастера MODBUS на RS485. */
class MasterMB
{
public:
	/** Конструктор. */
	MasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout, CmdProcessor& processor);
	/** Деструктор. */
	virtual ~MasterMB(void);

	/** Изменяет настройки RS485. */
	DWORD Settings(DWORD baudrate, BYTE parity);

	/** Инициализирует посылку запроса мастера MODBUS. */
	DWORD SendRequest(BYTE address, const BYTE* data, int dataLen);
	/** Устанавливает таймауты мастера MODBUS. */
	DWORD SetTimeouts(int charTimeout, int responceTimeout);

	/** Возвращает код последней системной ошибки. */
	DWORD GetSystemErrorCode(){ return trans_.GetSystemErrorCode(); }

private:
	/** Валидатор принятых фреймов. */
	MasterBehaviorMB behavior_;
	/** Приемопередатчик MODBUS. */
	TransieverMB trans_;

};
