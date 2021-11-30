/*
	Author:		R.Gorohov (K1 SOFT)

	TransieverMB.h
	Объявление класса TransieverMB.
*/

#pragma once

#include "CommandQueue.h"
#include "PortRs485.h"


class CmdProcessor;
class BehaviorMB;
class FrameMB;

// прототип класса, осуществляющего обмен в сети MODBUS, общий для мастера и слейва.
class TransieverMB
{
public:
	/** Конструктор. */
	TransieverMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, BehaviorMB& behavior, CmdProcessor& processor);
	/** Деструктор. */
	virtual ~TransieverMB(void);

	/** Устанавливает настройки COM-порта */
	DWORD Settings(DWORD baudrate, BYTE parity);
	/** Создает команду с данными для передачи и добавляет в очередь. */
	DWORD NewSend(BYTE address, const BYTE* data, int dataLen);
	/** Устанавливает таймауты. */
	virtual DWORD SetTimeouts(int charTimeout, int responceTimeout);
	/** Возвращает код системной ошибки. */
	DWORD GetSystemErrorCode(){ return comPort_.GetSystemErrorCode(); }
	/** Возвращает имя порта. */
	const std::wstring& GetPortName(){ return comPort_.GetPortName(); }

	/** Останавливает процесс обмена устройства с сетью MODBUS. */
	void Stop();

protected:
	/** COM порт, по которому идет обмен. */
	PortRs485 comPort_;

	/** Приемный буфер. */
	BYTE recBuffer_[514];
	/** Счетчик принятых байт текущего фрейма. */
	int recCounter_;
	/** Счетчик отправленных байт текущего фрейма. */
	int sendCounter_;

	/** Инициализирует новую передачу. */
	void InitSend();
	/** Инициализирует новый прием. */
	void InitRead();

	/** Устанавливает таймаут приема (передачи) символа. */
	DWORD SetTimeout(int timeout){ return comPort_.SetTimeout(timeout); }


	/** Добавляет команду в приемную очередь. */
	void AddRecCmd(Command* cmd);
	/** Добавляет команду в очередь для пересылки. */
	void AddToSend(DataCommand* cmd);

	/** Запускает процесс обмена устройства с сетью MODBUS. */
	void Start(DWORD baudrate, BYTE parity, int charTimeout);


	/** Обрабатывает принятые команды. */
	void ProcessRecCommands();
	/** Обрабатывает системную ошибку обмена. */
	void ProcessError(DWORD err);

	/** Указатель на фрейм, который передается в данной время. */
	FrameMB* frameForSend_;

	/** Обработчик команд. */
	CmdProcessor& processor_;
	/** Валидатор принятых фреймов. */
	BehaviorMB& behavior_;
	/** Очередь команд для пересылки. */
	CommandQueue sendQueue_;
	/** приемная очередь. */
	CommandQueue recQueue_;

private:

	/** Пересылает очередной символ. */
	void Send();
	/** Принимает очередной символ. */
	DataCommand* Receive();

	HANDLE hStopWorkThreadEvent_;


	/** Идентификатор потока пересылки данных. */
	HANDLE workThread_;

	static DWORD WINAPI WorkThread(LPVOID lpParam);

	DataCommand* ReceiveSymbol(DWORD& errCode);

	bool IsNeedToRead();

};
