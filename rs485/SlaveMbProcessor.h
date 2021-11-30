/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMbProcessor.h
	���������� ������ SlaveMbProcessor.
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

	/** ������������ ������� � ����� ������. */
	virtual void ProcessError(const std::wstring& portName, BYTE address, DWORD errCode);
	/** ������������ ������� � �������. */
	virtual void ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len);
	/** ������������ ������� ��������� ���������. */
	virtual void ProcessSend(const std::wstring& portName, BYTE address);
	/** ������������ ������� � ��������� ��������. */
	virtual void ProcessTimeout(const std::wstring& portName, BYTE address);

	/** �������������� ��������� �� callback-������� ��������� �������� �� �������. */
	void SetReceiveCallback(pReceiveSlaveMBCallback callback){ receiveCallback_ = callback; }
	/** �������������� ��������� �� callback-������� ��������� ��������� ������ ������� MODBUS. */
	void SetSendCallback(pSendSlaveMBCallback callback){ sendCallback_ = callback; }

private:
	/** ��������� �� callback-������� ��������� �������� �� �������. */
	pReceiveSlaveMBCallback	receiveCallback_;
	/** ��������� �� callback-������� ��������� ��������� ������ ������� MODBUS. */
	pSendSlaveMBCallback		sendCallback_;
};
