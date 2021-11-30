/*
	Author:		R.Gorohov (K1 SOFT)

	Command.h
	Объявление классов Command, DataCommand, SendCommand, ErrorCommand, TimeoutCommand.
*/

#pragma once

#include <string>
#include "FrameMB.h"

class CmdProcessor;

/** Базовый абстрактный класс команды. */
class Command
{
public:
	/** Конструктор. */
	Command(const std::wstring& portName, BYTE address);
	/** Деструктор. */
	virtual ~Command(void);

	/** Возвращает имя открытого порта RS485. */
	const std::wstring GetPortName(){ return portName_; }
	/** Возвращает адрес MODBUS устройства. */
	BYTE GetAddress(){ return address_; }

	/** Обработчик команды (реализуется в наследнике).*/
	virtual bool Process(CmdProcessor* processor) = 0;

#ifdef USE_LOG
	virtual const char* GetName() = 0;
#endif

private:
	/** Имя порта RS485. */
	std::wstring portName_;
	/** Адрес MODBUS устройства. */
	BYTE address_;
};

////////////////////////////////////////////////////////////////////////////

/** Класс команды, инкапсулирующей буфер с данными. */
class DataCommand : public Command
{
public:
	/** Конструктор. */
	DataCommand(const std::wstring& portName, BYTE address, const BYTE* data, int len);
	/** Деструктор. */
	~DataCommand();

	/** Вызывает метод обработчика команд. */
	virtual bool Process(CmdProcessor* processor);

	/** Возвращает указатель на буфер с данными команды. */
	const BYTE *GetBuffer()const { return data_; }
	/** Возвращает размер буфера с данными команды. */
	int GetLen()const { return len_; }

#ifdef USE_LOG
	virtual const char* GetName(){return "DataCommand";}
#endif

private:
	/** Буфер с данными. */
	BYTE* data_;
	/** Размер буфера. */
	int len_;
};

/////////////////////////////////////////////////////////////////////////////

/** Команда, инкапсулирующая событие завершения отправки данных. */
class SendCommand : public Command
{
public:
	/** Конструктор. */
	SendCommand(const std::wstring& portName, BYTE address);
	/** Деструктор. */
	~SendCommand();

	/** Вызывает метод обработчика команд. */
	virtual bool Process(CmdProcessor* processor);

#ifdef USE_LOG
	virtual const char* GetName(){return "SendCommand";}
#endif
};

/////////////////////////////////////////////////////////////////////////////

/** Класс команды, инкапсулирующей событие ошибки. */
class ErrorCommand : public Command
{
public:
	/** Конструктор. */
	ErrorCommand(const std::wstring& portName, BYTE address, DWORD errorCode);
	/** ДДеструктор. */
	~ErrorCommand();

	/** Вызывает метод обработчика команд. */
	virtual bool Process(CmdProcessor* processor);

#ifdef USE_LOG
	virtual const char* GetName(){return "ErrorCommand";}
#endif

private:
	/** Кол ошибки. */
	DWORD errorCode_;
};

//////////////////////////////////////////////////////////////////////////////

/** Класс команды, инкапсулирующей событие превышения времени ожидания ответа. */
class TimeoutCommand : public Command
{
public:
	/** Конструктор. */
	TimeoutCommand(const std::wstring& portName, BYTE address);
	/** Деструктор. */
	~TimeoutCommand();

	/** Вызывает метод обработчика коман. */
	virtual bool Process(CmdProcessor* processor);

	#ifdef USE_LOG
	virtual const char* GetName(){return "TimeoutCommand";}
#endif
};