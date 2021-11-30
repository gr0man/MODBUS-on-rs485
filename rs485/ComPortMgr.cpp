/*
	Author:		R.Gorohov (K1 SOFT)

	ComportMgr.cpp
	����������� ������ ComPortMgr.
*/

#include "StdAfx.h"
#include "ComPortMgr.h"

#include "ErrorCodes.h"
#include "PortRs485.h"

/**
 *	�����������.
 */
ComPortMgr::ComPortMgr(void)
{
	InitializeCriticalSection(&cs_);

	defaultSettings_.baudrate = CBR_4800;
	defaultSettings_.byteSize = 8;
	defaultSettings_.charTimeout = 100;
	defaultSettings_.stopBits = ONESTOPBIT;
	defaultSettings_.parity = EVENPARITY;
}

/**
 *	����������.
 */
ComPortMgr::~ComPortMgr(void)
{
	CloseAllPorts();
	DeleteCriticalSection(&cs_);
}

/**
 *	������������� ������������� ����������.
 */
void ComPortMgr::Lock()
{
	EnterCriticalSection(&cs_);
}

/**
 *	������� ������������� ����������.
 */
void ComPortMgr::Unlock()
{
	LeaveCriticalSection(&cs_);
}

/**
 *	��������� ��� COM �����, ������������� � �������.
 */
void ComPortMgr::CloseAllPorts()
{
	std::map<std::wstring, PortRs485*>::iterator portIter;
	for(portIter = portTable_.begin(); portIter != portTable_.end(); ++portIter)
	{
		delete portIter->second;
	}
	portTable_.clear();
}

/**
 *	��������� COM ����.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD ComPortMgr::Open(LPCTSTR portName)
{
	DWORD ret = OPERATION_COMPLETE;

	try
	{
		PortRs485* port = new PortRs485(portName);

		portTable_.insert(std::pair<std::wstring, PortRs485*>(std::wstring(portName), port));
		errorTable_.erase(std::wstring(portName));
	}
	catch(DWORD err)
	{
		ret = err;
		errorTable_.insert(std::pair<std::wstring, DWORD>(std::wstring(portName), err));
	}

	return ret;
}

/**
 *	��������� COM ����.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD ComPortMgr::Close(LPCTSTR portName)
{
	PortRs485* port = Find(portName);
	if( port )
	{
		portTable_.erase(std::wstring(portName));
		delete port;

		return OPERATION_COMPLETE;
	}

	return PORT_NOT_OPEN;
}

/**
 *	������ �� COM ����� ������.
 *	\param portName ��� �����.
 *	\param data ��������� �� ����� ��� ������ ������.
 *	\param len ������ ������ � ������.
 *	\param nmbRead ��������� �� ���������� �������� ����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD ComPortMgr::Read(LPCTSTR portName, BYTE* data, int len, int* nmbRead)
{
	PortRs485* port = Find(portName);
	if( port )
	{
		return port->Read(data, len, nmbRead);
	}

	return PORT_NOT_OPEN;
}

/**
 *	���������� ������ � COM ����.
 *	\param portName ��� �����.
 *	\param data ��������� �� ����� � �������.
 *	\param len ���������� ���� � ������.
 *	\param nmbWritten ��������� �� ���������� ���������� ����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD ComPortMgr::Write(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten)
{
	PortRs485* port = Find(portName);
	if( port )
	{
		return port->Write(data, len, nmbWritten);
	}

	return PORT_NOT_OPEN;
}

/**
 *	������������� ��������� COM �����.
 *	\param portName ��� �����.
 *	\param baudrate �������� �����.
 *	\param byteSize ������ ����� � �����.
 *	\param stopBits ���������� �������� ���.
 *	\param parity ��������� �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD ComPortMgr::Settings(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.baudrate = baudrate;
	sett.byteSize = byteSize;
	sett.stopBits = stopBits;
	sett.parity = parity;

	AddSettingsInTable(portName, sett);
	
	PortRs485* port = Find(portName);
	if( port )
	{
		
		return port->Settings(baudrate, byteSize, stopBits, parity);
	}

	return PORT_NOT_OPEN;
}

/**
 *	������������� ������� ������ (������) �������.
 *	\param portName ��� �����.
 *	\param timeout �������� �������� � ��.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD ComPortMgr::SetTimeout(LPCTSTR portName, int timeout)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.charTimeout = timeout;

	AddSettingsInTable(portName, sett);
	
	PortRs485* port = Find(portName);
	if( port )
	{
		return port->SetTimeout(timeout);
	}

	return PORT_NOT_OPEN;
}

/**
 *	���������� ��� ��������� ��������� ������.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD ComPortMgr::GetSystemError(LPCTSTR portName)
{
	PortRs485* port = Find(portName);
	if( port )
		return port->GetSystemErrorCode();

	std::map<std::wstring, DWORD>::iterator iter = errorTable_.find(portName);
	if( iter != errorTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	������������ ����� ��������� COM ����� � �������.
 *	\param portName ��� �����.
 *	\return ��������� �� ����� PortRs485, ���� ���� ������ � �������,
 *			0 � ��������� ������.
 */
PortRs485* ComPortMgr::Find(LPCTSTR portName)
{
	std::map<std::wstring, PortRs485*>::iterator iter = portTable_.find(std::wstring(portName));
	if( iter != portTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	��������� � ������� ��������� ��� ��������� COM �����.
 *	\param portName ��� �����.
 *	\param settings ������ �� ���������.
 */

void ComPortMgr::AddSettingsInTable(LPCTSTR portName, PortSettings& settings)
{
	portSettingsTable_.insert(std::pair<std::wstring, PortSettings>(std::wstring(portName), settings));
}

/**
 *	���������� ��������� ��� ��������� COM �����.
 *	\param portName ��� �����.
 *	\return ��������� ����� �� �������, ���� ��� ���� � ������� ���
  *			��������� ��-��������� � ��������� ������.
 */
ComPortMgr::PortSettings ComPortMgr::GetSettingsFromTable(LPCTSTR portName)
{
	std::map<std::wstring, PortSettings>::iterator iter = portSettingsTable_.find(portName);
	if( iter == portSettingsTable_.end() )
		return defaultSettings_;
	else
		return iter->second;
}