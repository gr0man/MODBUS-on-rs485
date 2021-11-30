/*
	Author:		R.Gorohov (K1 SOFT)

	CmdProcessor.h
	Объявление класса CmdProcessor.
*/

#pragma once

#include <string>

/** Абстрактный базовый класс обработчика команд. */
class CmdProcessor
{
public:
	/** Конструктор. */
	CmdProcessor(void);
	/** Деструктор. */
	virtual ~CmdProcessor(void);

	/** Обрабатывает команду с кодом ошибки. */
	virtual void ProcessError(const std::wstring& portName, BYTE address, DWORD errCode)=0;
	/** Обрабатывает команду с данными. */
	virtual void ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len)=0;
	/** Обрабатывает команду окончания пересылки. */
	virtual void ProcessSend(const std::wstring& portName, BYTE address)=0;
	/** Обрабфтывает команды с признаком таймаута. */
	virtual void ProcessTimeout(const std::wstring& portName, BYTE address)=0;
};
