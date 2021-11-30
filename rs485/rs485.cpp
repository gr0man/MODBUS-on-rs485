/*
	Author:		R.Gorohov (K1 SOFT)

	rs485.cpp
	����������� �������������� ������� ����������.
*/

#include "stdafx.h"

#include <map>

#include "rs485.h"
#include "PortRs485.h"
#include "MasterMB.h"
#include "SlaveMB.h"

#include "errorCodes.h"

#include "Rs485Manager.h"

#include "FrameMB.h"


Rs485Manager manager;

/**
 *	��������� �������� COM-����.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI OpenRS485(LPCTSTR portName)
{
	return manager.OpenRs485(portName);
}

/**
 *	��������� �������� COM-����.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI CloseRS485(LPCTSTR portName)
{
	return manager.CloseRs485(portName);
}

/**
 *	������ �� COM-����� ������.
 *	\param portName ��� �����.
 *	\param data ��������� �� ����� ��� ������ ������.
 *	\param len ������ ��������� ������.
 *	\param nmbRead ��������� �� ����������, � �������� ����������� ���������� ����������� ����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI ReadRS485(LPCTSTR portName, BYTE* data, int len, int* nmbRead)
{
	return manager.ReadRs485(portName, data, len, nmbRead);
}

/**
 *	���������� � COM-���� ������. 
 *	\param portName ��� �����.
 *	\param data ��������� �� ������ ��� ������.
 *	\param len ������ ������.
 *	\param nmbRead ��������� �� ����������, � �������� ����������� ���������� ���������� ����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI WriteRS485(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten)
{
	return manager.WriteRs485(portName, data, len, nmbWritten);
}

/**
 *	������������� ��������� COM-�����.
 *	\param portName ��� �����.
 *	\param baudrate �������� ������.
 *	\param byteSize ������ ����� � �����.
 *	\param stopBits ���������� �������� ���.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SettingsRS485(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity)
{
	return manager.SettingsRs485(portName, baudrate, byteSize, stopBits, parity);
}

/**
 *	������������� ������� ������ (������) COM-�����.
 *	\param portName ��� �����.
 *	\param timeout ������� ������ (�������) �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SetTimeoutRS485(LPCTSTR portName, int timeout)
{
	return manager.SetTimeoutRs485(portName, timeout);
}


// MODBUS �������

/**
 *	��������� ������ MODBUS.
 *	\param portName ��� �����.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\param charTimeout ������� ������ (������) �������.
 *	\param responceTimeout ����� �������� ������ �� ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI StartMasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout)
{
	return manager.StartMaster(portName, baudrate, parity, charTimeout, responceTimeout);
}

/**
 *	�������� unicast-������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\param address ����� ����������.
 *	\param data ��������� �� ������ � �������.
 *	\param dataLen ������ ������� � ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SendRequestMasterMB(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen)
{
	return manager.SendRequestMaster(portName, address, data, dataLen);
}

/**
 *	������������� �������� ������� MODBUS.
 *	\param portName ��� �����.
 *	\param charTimeout ������� ������ (������) �������.
 *	\param responceTimeout ����� �������� ������ �� ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SetTimeoutsMasterMB(LPCTSTR portName, int charTimeout, int responceTimeout)
{
	return manager.SetTimeoutsMaster(portName, charTimeout, responceTimeout);
}

/**
 *	��������� ������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI StopMasterMB(LPCTSTR portName)
{
	return manager.StopMaster(portName);
}

/**
 *	������������� ��������� ������� MODBUS.
 *	\param portName ��� �����.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SettingsMasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	return manager.SettingsMaster(portName, baudrate, parity);
}

/**
 *	�������������� ��������� �� callback-������� ��������� ������ �� ������.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SetReceiveMasterMBCallback(pReceiveMasterMBCallback callback)
{
	return manager.SetReceiveMasterCallback(callback);
}

/**
 *	�������������� ��������� �� callback-������� ��������� �������� ������ ������.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SetTimeoutMasterMBCallback(pTimeoutMasterMBCallback callback)
{
	return manager.SetTimeoutMasterCallback(callback);
}

/**
 *	�������������� ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SetSendMasterMBCallback(pSendMasterMBCallback callback)
{
	return manager.SetSendMasterCallback(callback);
}


/**
 *	��������� ����� MODBUS.
 *	\param portName ��� �����.
 *	\param adddress ����� ������.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\param charTimeout ������� ������ (������) �������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI StartSlaveMB(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout)
{
	return manager.StartSlave(portName, address, baudrate, parity, charTimeout);
}

/**
 *	�������� ����� �� ������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\param data ��������� �� ������ � �������.
 *	\param dataLen ������ ������� � ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SendReplySlaveMB(LPCTSTR portName, const BYTE* data, int dataLen)
{
	return manager.SendReplySlave(portName, data, dataLen);
}

/**
 *	��������� ������ ������ MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI StopSlaveMB(LPCTSTR portName)
{
	return manager.StopSlave(portName);
}

/**
 *	������������� ��������� ������ MODBUS.
 *	\param portName ��� �����.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SettingsSlaveMB(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	return manager.SettingsSlave(portName, baudrate, parity);
}

/**
 *	������������� ������� �� ������/������ �������.
 *	\param portName ��� �����.
 *	\param charTimeout ������� ������ (������) �������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SetTimeoutSlaveMB(LPCTSTR portName, int charTimeout)
{
	return manager.SetTimeoutSlave(portName, charTimeout);
}

/**
 *	�������������� ��������� �� callback-������� ��������� ������� �� ������� MODBUS.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SetReceiveSlaveMBCallback(pReceiveSlaveMBCallback callback)
{
	return manager.SetReceiveSlaveCallback(callback);
}

/**
 *	�������������� ��������� �� callback-������� ��������� ��������� ������ ������� MODBUS.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD WINAPI SetSendSlaveMBCallback(pSendSlaveMBCallback callback)
{
	return manager.SetSendSlaveCallback(callback);
}


/**
 *	���������� ��� ��������� ������ RS485.
 *	\param portName ��� �����.
 *	\return ��� ��������� ������.
 */
DWORD WINAPI GetSystemErrorRS485(LPCTSTR portName)
{
	return manager.GetSystemErrorRs485(portName);
}

/**
 *	���������� ��� ��������� ������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ��������� ������.
 */
DWORD WINAPI GetSystemErrorMasterMB(LPCTSTR portName)
{
	return manager.GetSystemErrorMaster(portName);
}

/**
 *	���������� ��� ��������� ������ ������ MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ��������� ������.
 */
DWORD WINAPI GetSystemErrorSlaveMB(LPCTSTR portName)
{
	return manager.GetSystemErrorSlave(portName);
}



//////////////////////////////////////////////////////////////
// ����������
pLogCallback logCallback(0);

DWORD SetLogCallback(pLogCallback callback)
{
	logCallback = callback;
	//if(logCallback)
	//	MessageBox(NULL, L"SetLogCallback", L"Info", MB_OK);
	return 0;
}

void Log(const char* info)
{
	if(logCallback)
		logCallback(info);
}


void LOG_Info(const char* info)
{
#ifdef USE_LOG	
	Log(info);
#endif
}

void LOG_Info(const char* info, const BYTE* data, int len)
{
#ifdef USE_LOG
	char buffer[20];
	std::string str(info);
	int i;
	for(i=0; i<len; ++i)
	{
		sprintf_s(buffer, 20, " %02X", data[i]);
		str += std::string(buffer);
	}
	str += "\n";
	Log(str.c_str());
#endif
}

void LOG_Info(const char* info, const char* param)
{
#ifdef USE_LOG
	char buffer[2000];
	sprintf_s(buffer, 2000, info, param);
	Log(buffer);
#endif
}

void LOG_Info(const char* info, const wchar_t* param)
{
#ifdef USE_LOG
	std::string str;
	ConvertWS2S(param, str);
	char buffer[2000];
	sprintf_s(buffer, 2000, info, str.c_str());
	Log(buffer);
#endif
}

void LOG_Error(const char* info, DWORD param1)
{
#ifdef USE_LOG
	char buffer[2000];
	sprintf_s(buffer, 2000, info, param1);
	Log(buffer);
#endif
}

void ConvertWS2S(LPCTSTR src, std::string& dst)
{
	char buf[1000];	
	std::wstring srcStr(src);
	wcstombs(buf, srcStr.c_str(), srcStr.size()+1);
	dst = std::string(buf);
}

#include "FrameMB_Tests.h"

DWORD StartFrameTests()
{
	FrameMB_Tests tests;

	tests.FrameFromBinary();
	tests.FrameFromAscii();

	return 0;
}