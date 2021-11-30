/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMbProcessor.h
	Объявление класса SlaveMbProcessor.
*/

#pragma once
#include "cmdprocessor.h"

#include "rs485.h"

class SlaveMbProcessor :
	public CmdProcessor
{
public:
	SlaveMbProcessor(void);
	~SlaveMbProcessor(void);

	/** Обрабатывает команду с кодом ошибки. */
	virtual void ProcessError(const std::wstring& portName, BYTE address, DWORD errCode);
	/** Обрабатывает команду с данными. */
	virtual void ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len);
	/** Обрабатывает команду окончания пересылки. */
	virtual void ProcessSend(const std::wstring& portName, BYTE address);
	/** Обрабфтывает команды с признаком таймаута. */
	virtual void ProcessTimeout(const std::wstring& portName, BYTE address);

	/** Инициализирует указатель на callback-функцию получения запороса от мастера. */
	void SetReceiveCallback(pReceiveSlaveMBCallback callback){ receiveCallback_ = callback; }
	/** Инициализирует указатель на callback-функцию окончания пересылки данных слейвом MODBUS. */
	void SetSendCallback(pSendSlaveMBCallback callback){ sendCallback_ = callback; }

private:
	/** Указатель на callback-функцию получения запороса от мастера. */
	pReceiveSlaveMBCallback	receiveCallback_;
	/** Указатель на callback-функцию окончания пересылки данных слейвом MODBUS. */
	pSendSlaveMBCallback		sendCallback_;
};
