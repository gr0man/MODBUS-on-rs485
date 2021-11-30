/*
	Author:		R.Gorohov (K1 SOFT)

	rs485.h
	���������� �������������� ������� ����������.
*/

#pragma once

/** ��������� �������� COM-����. */
DWORD WINAPI OpenRS485(LPCTSTR portName);
/** ��������� �������� COM-����. */
DWORD WINAPI CloseRS485(LPCTSTR portName);
/** ������ �� COM-����� ������. */
DWORD WINAPI ReadRS485(LPCTSTR portName, BYTE* data, int len, int* nmbRead);
/** ���������� � COM-���� ������. */
DWORD WINAPI WriteRS485(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten);
/** ������������� ��������� COM-�����. */
DWORD WINAPI SettingsRS485(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity);
/** ������������� ������� ������ (������) COM-�����. */
DWORD WINAPI SetTimeoutRS485(LPCTSTR portName, int timeout);

// MODBUS �������

/** ��� callback-�������, ���������� ��� ��������� ������ �� ������ MODBUS */
typedef void (WINAPI *pReceiveMasterMBCallback)(LPCTSTR portName, BYTE slaveAddress, const BYTE* data, int len);
/** ��� callback-�������, ���������� ��� ��������� �������� �������� ������ �� ������ MODBUS */
typedef void (WINAPI *pTimeoutMasterMBCallback)(LPCTSTR portName, BYTE address);
/** ��� callback-�������, ���������� �� ��������� ��������� ������ �������� MODBUS */
typedef void (WINAPI *pSendMasterMBCallback)(LPCTSTR portName);

/** ��������� ������ MODBUS. */
DWORD WINAPI StartMasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout);
/** �������� unicast-������ ������� MODBUS. */
DWORD WINAPI SendRequestMasterMB(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen);
/** ��������� ������ ������� MODBUS. */
DWORD WINAPI StopMasterMB(LPCTSTR portName);
/** ������������� �������� ������� MODBUS. */
DWORD WINAPI SetTimeoutsMasterMB(LPCTSTR portName, int charTimeout, int responceTimeout);
/** ������������� ��������� ������� MODBUS. */
DWORD WINAPI SettingsMasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity);
/** �������������� ��������� �� callback-������� ��������� ������ �� ������ */
DWORD WINAPI SetReceiveMasterMBCallback(pReceiveMasterMBCallback callback);
/** �������������� ��������� �� callback-������� ��������� �������� ������ ������. */
DWORD WINAPI SetTimeoutMasterMBCallback(pTimeoutMasterMBCallback callback);
/** �������������� ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS. */
DWORD WINAPI SetSendMasterMBCallback(pSendMasterMBCallback callback);

/** ��� callback-�������, ���������� ��� ��������� ������� �� ������� MODBUS. */
typedef void (WINAPI *pReceiveSlaveMBCallback)(LPCTSTR portName, BYTE slaveAddress, const BYTE* data, int len);
/** ��� callback-�������, ���������� �� ��������� ��������� ������ ������� MODBUS. */
typedef void (WINAPI *pSendSlaveMBCallback)(LPCTSTR portName);

/** ��������� ����� MODBUS. */
DWORD WINAPI StartSlaveMB(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout);
/** �������� ����� �� ������ ������� MODBUS. */
DWORD WINAPI SendReplySlaveMB(LPCTSTR portName, const BYTE* data, int dataLen);
/** ��������� ������ ������ MODBUS. */
DWORD WINAPI StopSlaveMB(LPCTSTR portName);
/** ������������� ��������� ������ MODBUS. */
DWORD WINAPI SettingsSlaveMB(LPCTSTR portName, DWORD baudrate, BYTE parity);
/** ������������� ������� �� ������/������ �������. */
DWORD WINAPI SetTimeoutSlaveMB(LPCTSTR portName, int charTimeout);
/** �������������� ��������� �� callback-������� ��������� ������� �� ������� MODBUS. */
DWORD WINAPI SetReceiveSlaveMBCallback(pReceiveSlaveMBCallback callback);
/** �������������� ��������� �� callback-������� ��������� ��������� ������ ������� MODBUS. */
DWORD WINAPI SetSendSlaveMBCallback(pSendSlaveMBCallback callback);

/** ���������� ��� ��������� ������ RS485. */
DWORD WINAPI GetSystemErrorRS485(LPCTSTR portName);
/** ���������� ��� ��������� ������ ������� MODBUS. */
DWORD WINAPI GetSystemErrorMasterMB(LPCTSTR portName);
/** ���������� ��� ��������� ������ ������ MODBUS. */
DWORD WINAPI GetSystemErrorSlaveMB(LPCTSTR portName);


DWORD StartFrameTests();


// ������� ��� �������
typedef void (*pLogCallback)(const char*);
DWORD SetLogCallback(pLogCallback callback);

void LOG_Info(const char* info);
void LOG_Info(const char* info, const BYTE* data, int len);
void LOG_Info(const char* info, const char* param);
void LOG_Info(const char* info, const wchar_t* param);
void LOG_Error(const char* info, DWORD param1);


#include <string>

void ConvertWS2S(LPCTSTR src, std::string& dst);

