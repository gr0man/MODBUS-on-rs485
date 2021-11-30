/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMbMgr.h
	���������� ������ MasterMbMgr.
*/

#pragma once

#include <map>
#include <string>

#include "rs485.h"
#include "MasterMbProcessor.h"

class MasterMB;

class MasterMbMgr
{
	/** ��������� ������� MODBUS. */
	struct PortSettings
	{
		DWORD baudrate;
		BYTE parity;
		int charTimeout;
		int responceTimeout;
	};

	PortSettings defaultSettings_;

public:
	/** �����������. */
	MasterMbMgr(void);
	/** ����������. */
	~MasterMbMgr(void);

	/** ��������� ��� �������� COM-�����. */
	void CloseAllPorts();

	/** ��������� ������ MODBUS. */
	DWORD Start(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout);
	/** ��������� ������ ������� MODBUS. */
	DWORD Stop(LPCTSTR portName);
	/** ������������� ��������� ������� MODBUS. */
	DWORD Settings(LPCTSTR portName, DWORD baudrate, BYTE parity);
	/** �������� unicast-������ ������� MODBUS. */
	DWORD SendRequest(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen);
	/** ������������� �������� ������� MODBUS. */
	DWORD SetTimeouts(LPCTSTR portName, int charTimeout, int responceTimeout);

	/** �������������� ��������� �� callback-������� ��������� ������ �� ������. */
	DWORD SetReceiveCallback(pReceiveMasterMBCallback callback);
	/** �������������� ��������� �� callback-������� ��������� �������� ������ ������. */
	DWORD SetTimeoutCallback(pTimeoutMasterMBCallback callback);
	/** �������������� ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS. */
	DWORD SetSendCallback(pSendMasterMBCallback callback);

	//! ���������� ��� ��������� ������.
	DWORD GetSystemError(LPCTSTR portName);

	/** ���������, ������ �� ������ MODBUS �� �������� COM �����. */
	bool IsExist(LPCTSTR portName){ return (Find(portName)!=0); }
	/** ��������� ������ ������� �� �������� COM ����� � ������� ��� �� �������. */
	void Remove(LPCTSTR portName);

	/** ������������� ������������� ����������. */
	void Lock();
	/** ������� ������������� ����������. */
	void Unlock();

private:
	/** ���������� ������ ������� MODBUS. */
	MasterMbProcessor processor_;

	/** ������� ������ RS485, �� ������� ������� MODBUS ������'� */
	std::map<std::wstring, MasterMB*> masterMbTable_;
	/** ������� ��������� ��������� ������. */
	std::map<std::wstring, DWORD> errorTable_;
	/** ������� �������� �������� MODBUS. */
	std::map<std::wstring, PortSettings> settingsTable_;

	/** ������� ������������� �������������. */
	CRITICAL_SECTION cs_;
	
	/** ��������� �� callback-������� ��������� ������ �� ������. */
	pReceiveMasterMBCallback receiveCallback_;
	/** ��������� �� callback-������� ��������� �������� ������ ������. */
	pTimeoutMasterMBCallback timeoutCallback_;
	/** ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS. */
	pSendMasterMBCallback	 sendCallback_;
	
	/** ��������� ��������� ������� MODBUS � �������. */
	void AddSettingsInTable(LPCTSTR portName, PortSettings& settings);
	/** ���������� ��������� ������� MODBUS �� ������� ��� ��������� COM �����. */
	PortSettings GetSettingsFromTable(LPCTSTR portName);
	/** ���� ������ MODBUS � ������� �� ����� COM �����. */
	MasterMB* Find(LPCTSTR portName);
};
