/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMB.h
	���������� ������� MasterBehaviorMB � MasterMB.
*/

#pragma once

#include "PortRs485.h"
#include "CommandQueue.h"
#include <vector>

#include "BehaviorMB.h"

#include "TransieverMB.h"

/** ������������� ���������� �� �������� ����������� ������� �������� MODBUS. */
class MasterBehaviorMB : public BehaviorMB
{
public:
	/** �����������. */
	MasterBehaviorMB(void);
	/** ����������. */
	virtual ~MasterBehaviorMB(void){};

	/** ������������� ������� �������� ������ �� ������. */
	void SetResponceTimeout(int responceTimeout){ responceTimeout_ = responceTimeout; }
	/** ��������� ��������� �������� ������. */
	void FinishSend(BYTE address);
	
	/** ��������� ����������� ������ ��������� ������. */
	bool IsAddressValid(BYTE address);
	/** ���������, �� ������� �� ����� �������� ������. */
	bool IsTimeoutExpiration();

	/** ���������� ��������� ��������. */
	void ResetSettings();

	bool IsNeedToRead(){ return isNeedToRead_; }

	void FinishReceive(BYTE address);
	void ReceiveTimeout(){ isNeedToRead_ = false; }
private:
	/** ������� �������� ������. */
	int responceTimeout_;
	/** ��������� �����. */
	BYTE slaveAddress_;
	/** ��������������� ����� ��������� ������� �������. */
	ULONGLONG requestTime_;

	bool isNeedToRead_;
};

/** ������������� ���������� ������� MODBUS �� RS485. */
class MasterMB
{
public:
	/** �����������. */
	MasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout, CmdProcessor& processor);
	/** ����������. */
	virtual ~MasterMB(void);

	/** �������� ��������� RS485. */
	DWORD Settings(DWORD baudrate, BYTE parity);

	/** �������������� ������� ������� ������� MODBUS. */
	DWORD SendRequest(BYTE address, const BYTE* data, int dataLen);
	/** ������������� �������� ������� MODBUS. */
	DWORD SetTimeouts(int charTimeout, int responceTimeout);

	/** ���������� ��� ��������� ��������� ������. */
	DWORD GetSystemErrorCode(){ return trans_.GetSystemErrorCode(); }

private:
	/** ��������� �������� �������. */
	MasterBehaviorMB behavior_;
	/** ���������������� MODBUS. */
	TransieverMB trans_;

};
