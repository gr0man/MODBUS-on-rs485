/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMB.h
	���������� ������� SlaveBehaviorMB � SlaveMB.
*/

#pragma once

#include "PortRs485.h"
#include <vector>

#include "CommandQueue.h"

#include "TransieverMB.h"
#include "BehaviorMB.h"
#include "CmdProcessor.h"

/** ������������� ���������� �� �������� ����������� ������� ������� MODBUS. */
class SlaveBehaviorMB : public BehaviorMB
{
public:
	/** �����������. */
	SlaveBehaviorMB(BYTE address):address_(address){};
	/** ����������. */
	virtual ~SlaveBehaviorMB(void){};

	/** ������������� ������� �������� ������. */
	void SetResponceTimeout(int responceTimeout){};
	/** ��������� ��������� �������� ������. */
	void FinishSend(BYTE address){};

	void SetAddress(BYTE address){ address_ = address; }
	
	/** ��������� ����������� ������ ��������� ������. */
	virtual bool IsAddressValid(BYTE address);
	/** ���������, �� ������� �� ����� �������� ������. */
	virtual bool IsTimeoutExpiration(){ return false; }
	/** ���������� ��������� ��������. */
	void ResetSettings(){};

	virtual bool IsNeedToRead(){ return true; }

	virtual void FinishReceive(BYTE address){};
	virtual void ReceiveTimeout(){};
private:
	BYTE address_;
};

/** ������������� ���������� ������ MODBUS �� RS485. */
class SlaveMB
{
public:
	/** �����������. */
	SlaveMB(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout, CmdProcessor& processor);
	/** ����������. */
	virtual ~SlaveMB(void);

	/** ������������� ����� ������ MODBUS. */
	DWORD SetAddress(BYTE address);
	/** �������������� ������� ������ ������� MODBUS. */
	DWORD SendReply(const BYTE* data, int dataLen);
	/** �������� ��������� RS485. */
	DWORD Settings(DWORD baudrate, BYTE parity);
	/** ������������� ������� ������ �������. */
	DWORD SetTimeout(int charTimeout){ return trans_.SetTimeouts(charTimeout, 0); }
	/** ���������� ��� ��������� ��������� ������. */
	DWORD GetSystemErrorCode(){ return trans_.GetSystemErrorCode(); }

private:
	/** ��������� �������� �������. */
	SlaveBehaviorMB behavior_;
	/** ���������������� MODBUS. */
	TransieverMB trans_;
	/** ����� ������ MODBUS. */
	BYTE address_;
};
