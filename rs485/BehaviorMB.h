/*
	Author:		R.Gorohov (K1 SOFT)

	BehaviorMB.h
	���������� ������ BehaviorMB.
*/

#pragma once

/** ������� ����������� ����� ��������� ���������� MODBUS */
class BehaviorMB
{
public:
	/** �����������. */
	BehaviorMB(void);
	/** ����������. */
	virtual ~BehaviorMB(void);

	/** ������������� ������� �������� ������. */
	virtual void SetResponceTimeout(int responceTimeout)=0;
	/** ��������� ������ ��������� ��������� ������. */
	virtual void FinishSend(BYTE address)=0;
	/** ���������� ����� ����������� ������ � ������� �������������� ����������. */
	virtual bool IsAddressValid(BYTE address)=0;
	/** ��������� �� ��������� �� ����� �������� ������. */
	virtual bool IsTimeoutExpiration()=0;
	/** ���������� ���������. */
	virtual void ResetSettings()=0;
	/** ���������� ������� ��������� �����. */
	ULONGLONG GetCurrentTime();
	/** ���������, ��������� �� ����� � ������ �����. */
	virtual bool IsNeedToRead()=0;

	virtual void FinishReceive(BYTE address)=0;
	virtual void ReceiveTimeout()=0;

};
