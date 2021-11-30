/*
	Author:		R.Gorohov (K1 SOFT)

	TransieverMB.h
	���������� ������ TransieverMB.
*/

#pragma once

#include "CommandQueue.h"
#include "PortRs485.h"


class CmdProcessor;
class BehaviorMB;
class FrameMB;

// �������� ������, ��������������� ����� � ���� MODBUS, ����� ��� ������� � ������.
class TransieverMB
{
public:
	/** �����������. */
	TransieverMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, BehaviorMB& behavior, CmdProcessor& processor);
	/** ����������. */
	virtual ~TransieverMB(void);

	/** ������������� ��������� COM-����� */
	DWORD Settings(DWORD baudrate, BYTE parity);
	/** ������� ������� � ������� ��� �������� � ��������� � �������. */
	DWORD NewSend(BYTE address, const BYTE* data, int dataLen);
	/** ������������� ��������. */
	virtual DWORD SetTimeouts(int charTimeout, int responceTimeout);
	/** ���������� ��� ��������� ������. */
	DWORD GetSystemErrorCode(){ return comPort_.GetSystemErrorCode(); }
	/** ���������� ��� �����. */
	const std::wstring& GetPortName(){ return comPort_.GetPortName(); }

	/** ������������� ������� ������ ���������� � ����� MODBUS. */
	void Stop();

protected:
	/** COM ����, �� �������� ���� �����. */
	PortRs485 comPort_;

	/** �������� �����. */
	BYTE recBuffer_[514];
	/** ������� �������� ���� �������� ������. */
	int recCounter_;
	/** ������� ������������ ���� �������� ������. */
	int sendCounter_;

	/** �������������� ����� ��������. */
	void InitSend();
	/** �������������� ����� �����. */
	void InitRead();

	/** ������������� ������� ������ (��������) �������. */
	DWORD SetTimeout(int timeout){ return comPort_.SetTimeout(timeout); }


	/** ��������� ������� � �������� �������. */
	void AddRecCmd(Command* cmd);
	/** ��������� ������� � ������� ��� ���������. */
	void AddToSend(DataCommand* cmd);

	/** ��������� ������� ������ ���������� � ����� MODBUS. */
	void Start(DWORD baudrate, BYTE parity, int charTimeout);


	/** ������������ �������� �������. */
	void ProcessRecCommands();
	/** ������������ ��������� ������ ������. */
	void ProcessError(DWORD err);

	/** ��������� �� �����, ������� ���������� � ������ �����. */
	FrameMB* frameForSend_;

	/** ���������� ������. */
	CmdProcessor& processor_;
	/** ��������� �������� �������. */
	BehaviorMB& behavior_;
	/** ������� ������ ��� ���������. */
	CommandQueue sendQueue_;
	/** �������� �������. */
	CommandQueue recQueue_;

private:

	/** ���������� ��������� ������. */
	void Send();
	/** ��������� ��������� ������. */
	DataCommand* Receive();

	HANDLE hStopWorkThreadEvent_;


	/** ������������� ������ ��������� ������. */
	HANDLE workThread_;

	static DWORD WINAPI WorkThread(LPVOID lpParam);

	DataCommand* ReceiveSymbol(DWORD& errCode);

	bool IsNeedToRead();

};
