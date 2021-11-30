/*
	Author:		R.Gorohov (K1 SOFT)

	Rs485Manager.h
	���������� ������ Rs485Manager.
*/

#pragma once
#include "stdAfx.h"

#include <map>
#include <string>

#include "rs485.h"

#include "ComPortMgr.h"
#include "MasterMbMgr.h"
#include "SlaveMbMgr.h"

class PortRs485;
class MasterMB;
class SlaveMB;


/** �������� ���� ��������� �� RS485. */
class Rs485Manager
{
public:
	/** �����������. */
	Rs485Manager(void);
	/** ����������. */
	~Rs485Manager(void);

	/** ��������� �������� COM-����.  */
	DWORD OpenRs485(LPCTSTR portName);
	/** ��������� �������� COM-����. */
	DWORD CloseRs485(LPCTSTR portName);
	/** ������ �� COM-����� ������. */
	DWORD ReadRs485(LPCTSTR portName, BYTE* data, int len, int* nmbRead);
	/** ���������� � COM-���� ������. */
	DWORD WriteRs485(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten);
	/** ��������� COM-�����. */
	DWORD SettingsRs485(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity);
	/** ��������� �������� COM-�����. */
	DWORD SetTimeoutRs485(LPCTSTR portName, int timeout);

	
	/** ��������� ������ MODBUS. */
	DWORD StartMaster(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout);
	/** �������� unicast-������ ������� MODBUS. */
	DWORD SendRequestMaster(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen);
	/** ������������� ��������� ������� MODBUS. */
	DWORD SettingsMaster(LPCTSTR portName, DWORD baudrate, BYTE parity);
	/** ��������� ������ ������� MODBUS. */
	DWORD StopMaster(LPCTSTR portName);
	/** ������������� �������� ������� MODBUS. */
	DWORD SetTimeoutsMaster(LPCTSTR portName, int charTimeout, int responceTimeout);

	/** �������������� ��������� �� callback-������� ��������� ������ �� ������ */
	DWORD SetReceiveMasterCallback(pReceiveMasterMBCallback callback);
	/** �������������� ��������� �� callback-������� ��������� �������� ������ ������. */
	DWORD SetTimeoutMasterCallback(pTimeoutMasterMBCallback callback);
	/** �������������� ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS. */
	DWORD SetSendMasterCallback(pSendMasterMBCallback callback);

	
	/** ��������� ����� MODBUS. */
	DWORD StartSlave(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout);
	/** �������� ����� �� ������ ������� MODBUS. */
	DWORD SendReplySlave(LPCTSTR portName, const BYTE* data, int dataLen);
	/** ������������� ��������� ������ MODBUS. */
	DWORD SettingsSlave(LPCTSTR portName, DWORD baudrate, BYTE parity);
	/** ��������� ������ ������ MODBUS. */
	DWORD StopSlave(LPCTSTR portName);
	/** ������������� ������� �� ������/������ �������. */
	DWORD SetTimeoutSlave(LPCTSTR portName, int charTimeout);

	/** �������������� ��������� �� callback-������� ��������� ������� �� ������� MODBUS. */
	DWORD SetReceiveSlaveCallback(pReceiveSlaveMBCallback callback);
	/** �������������� ��������� �� callback-������� ��������� ��������� ������ ������� MODBUS. */
	DWORD SetSendSlaveCallback(pSendSlaveMBCallback callback);


	/** ���������� ��� ��������� ������ RS485. */
	DWORD GetSystemErrorRs485(LPCTSTR portName);
	/** ���������� ��� ��������� ������ ������� MODBUS. */
	DWORD GetSystemErrorMaster(LPCTSTR portName);
	/** ���������� ��� ��������� ������ ������ MODBUS. */
	DWORD GetSystemErrorSlave(LPCTSTR portName);

private:
	/** �������� COM ������ RS485. */
	ComPortMgr	comPortManager_;
	/** �������� ��������� MODBUS. */
	MasterMbMgr masterMbManager_;
	/** �������� ������� MODBUS. */
	SlaveMbMgr	slaveMbManager_;

};
