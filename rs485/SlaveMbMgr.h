/*
	Author:		R.Gorohov (K1 SOFT)

	SlaveMbMgr.h
	���������� ������ SlaveMbMgr.
*/

#pragma once

#include <string>
#include <map>

#include "rs485.h"
#include "SlaveMbProcessor.h"

class SlaveMB;

class SlaveMbMgr
{
	/** ��������� ������ MODBUS. */
	struct PortSettings
	{
		DWORD baudrate;
		BYTE parity;
		int charTimeout;
	};

	PortSettings defaultSettings_;

public:
	/** �����������. */
	SlaveMbMgr(void);
	/** ����������. */
	~SlaveMbMgr(void);

	/** ��������� ��� �������� COM-�����. */
	void CloseAllPorts();

	/** ��������� ����� MODBUS. */
	DWORD Start(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout);
	/** ��������� ������ ������ MODBUS. */
	DWORD Stop(LPCTSTR portName);
	/** �������� ����� �� ������ ������� MODBUS. */
	DWORD SendReply(LPCTSTR portName, const BYTE* data, int dataLen);
	/** ������������� ��������� ������ MODBUS. */
	DWORD Settings(LPCTSTR portName, DWORD baudrate, BYTE parity);
	/** ������������� ������� �� ������/������ �������. */
	DWORD SetTimeout(LPCTSTR portName, int charTimeout);

	/** �������������� ��������� �� callback-������� ��������� ������� �� ������� MODBUS. */
	DWORD SetReceiveCallback(pReceiveSlaveMBCallback callback);
	/** �������������� ��������� �� callback-������� ��������� ��������� ������ ������� MODBUS. */
	DWORD SetSendCallback(pSendSlaveMBCallback callback);

	/** ���������� ��� ��������� ������. */
	DWORD GetSystemError(LPCTSTR portName);

	/** ���������, ������ �� ����� MODBUS �� �������� COM �����. */
	bool IsExist(LPCTSTR portName);//{ return (Find(portName)!=0); }
	/** ��������� ������ ������ �� �������� COM ����� � ������� ��� �� �������. */
	void Remove(LPCTSTR portName);

	/** ������������� ������������� ����������. */
	void Lock();
	/** ������� ������������� ����������. */
	void Unlock();

private:
	/** ���������� ������ ������� MODBUS. */
	SlaveMbProcessor processor_;

	/** ������� ������ RS485, �� ������� ������� MODBUS �����'� */
	std::map<std::wstring, SlaveMB*> slaveMbTable_;
	/** ������� ��������� ��������� ������. */
	std::map<std::wstring, DWORD> errorTable_;
	/** ������� �������� ������� MODBUS. */
	std::map<std::wstring, PortSettings> settingsTable_;

	/** ������� ������������� �������������. */
	CRITICAL_SECTION cs_;

	/** ��������� �� callback-������� ��������� ������� �� �������. */
	pReceiveSlaveMBCallback receiveCallback_;
	/** ��������� �� callback-������� ��������� ��������� ������ ������� MODBUS. */
	pSendSlaveMBCallback sendCallback_;

	/** ��������� ��������� ������ MODBUS � �������. */
	void AddSettingsInTable(LPCTSTR portName, PortSettings& settings);
	/** ���������� ��������� ������ MODBUS �� ������� ��� ��������� COM �����. */
	PortSettings GetSettingsFromTable(LPCTSTR portName);
	/** ���� ����� MODBUS � ������� �� ����� COM �����. */
	SlaveMB* Find(LPCTSTR portName);
};
