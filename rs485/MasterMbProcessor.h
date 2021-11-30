/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMbProcessor.h
	Объявление класса MasterMbProcessor.
*/

#pragma once
#include "cmdprocessor.h"
#include "rs485.h"

class MasterMbProcessor :
	public CmdProcessor
{
public:
	/** Конструктор. */
	MasterMbProcessor(void);
	/** Деструктор. */
	~MasterMbProcessor(void);

	/** Обрабатывает команду с кодом ошибки. */
	virtual void ProcessError(const std::wstring& portName, BYTE address, DWORD errCode);
	/** Обрабатывает команду с данными. */
	virtual void ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len);
	/** Обрабатывает команду окончания пересылки. */
	virtual void ProcessSend(const std::wstring& portName, BYTE address);
	/** Обрабфтывает команды с признаком таймаута. */
	virtual void ProcessTimeout(const std::wstring& portName, BYTE address);

	/** Инициализирует указатель на callback-функцию получения ответа от слейва. */
	void SetReceiveCallback(pReceiveMasterMBCallback callback){ receiveCallback_ = callback; }
	/** Инициализирует указатель на callback-функцию истечения таймаута ответа слейва. */
	void SetTimeoutCallback(pTimeoutMasterMBCallback callback){ timeoutCallback_ = callback; }
	/** Инициализирует указатель на callback-функцию окончания пересылки данных мастером MODBUS. */
	void SetSendCallback(pSendMasterMBCallback callback){ sendCallback_ = callback; }

private:
	/** Указатель на callback-функцию получения ответа от слейва. */
	pReceiveMasterMBCallback	receiveCallback_;
	/** Указатель на callback-функцию истечения таймаута ответа слейва. */
	pTimeoutMasterMBCallback	timeoutCallback_;
	/** Указатель на callback-функцию окончания пересылки данных мастером MODBUS. */
	pSendMasterMBCallback		sendCallback_;
};
