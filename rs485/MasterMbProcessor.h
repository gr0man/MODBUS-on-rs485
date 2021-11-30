/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMbProcessor.h
	���������� ������ MasterMbProcessor.
*/

#pragma once
#include "cmdprocessor.h"
#include "rs485.h"

class MasterMbProcessor :
	public CmdProcessor
{
public:
	/** �����������. */
	MasterMbProcessor(void);
	/** ����������. */
	~MasterMbProcessor(void);

	/** ������������ ������� � ����� ������. */
	virtual void ProcessError(const std::wstring& portName, BYTE address, DWORD errCode);
	/** ������������ ������� � �������. */
	virtual void ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len);
	/** ������������ ������� ��������� ���������. */
	virtual void ProcessSend(const std::wstring& portName, BYTE address);
	/** ������������ ������� � ��������� ��������. */
	virtual void ProcessTimeout(const std::wstring& portName, BYTE address);

	/** �������������� ��������� �� callback-������� ��������� ������ �� ������. */
	void SetReceiveCallback(pReceiveMasterMBCallback callback){ receiveCallback_ = callback; }
	/** �������������� ��������� �� callback-������� ��������� �������� ������ ������. */
	void SetTimeoutCallback(pTimeoutMasterMBCallback callback){ timeoutCallback_ = callback; }
	/** �������������� ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS. */
	void SetSendCallback(pSendMasterMBCallback callback){ sendCallback_ = callback; }

private:
	/** ��������� �� callback-������� ��������� ������ �� ������. */
	pReceiveMasterMBCallback	receiveCallback_;
	/** ��������� �� callback-������� ��������� �������� ������ ������. */
	pTimeoutMasterMBCallback	timeoutCallback_;
	/** ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS. */
	pSendMasterMBCallback		sendCallback_;
};
