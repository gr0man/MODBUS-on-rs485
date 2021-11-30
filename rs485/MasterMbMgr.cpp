/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMbMgr.cpp
	����������� ������ MasterMbMgr.
*/

#include "StdAfx.h"
#include "MasterMbMgr.h"

#include "MasterMB.h"

#include "ErrorCodes.h"

/**
 *	�����������.
 */
MasterMbMgr::MasterMbMgr(void)
: receiveCallback_(0)
, timeoutCallback_(0)
, sendCallback_(0)
{
	InitializeCriticalSection(&cs_);

	defaultSettings_.baudrate = CBR_4800;
	defaultSettings_.charTimeout = 100;
	defaultSettings_.responceTimeout = 10000;
	defaultSettings_.parity = EVENPARITY;
}

/**
 *	����������.
 */
MasterMbMgr::~MasterMbMgr(void)
{
	CloseAllPorts();
	DeleteCriticalSection(&cs_);
}

/**
 *	������������� ������������� ����������.
 */
void MasterMbMgr::Lock()
{
	EnterCriticalSection(&cs_);
}

/**
 *	������� ������������� ����������.
 */
void MasterMbMgr::Unlock()
{
	LeaveCriticalSection(&cs_);
}

/**
 *	��������� ��� �������� COM-�����.
 */
void MasterMbMgr::CloseAllPorts()
{
	std::map<std::wstring, MasterMB*>::iterator masterIter;
	for(masterIter = masterMbTable_.begin(); masterIter != masterMbTable_.end(); ++masterIter)
	{
		delete masterIter->second;
	}
	masterMbTable_.clear();
}

/**
 *	��������� ������ MODBUS.
 *	\param portName ��� �����.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\param charTimeout ������� ������ (������) �������.
 *	\param responceTimeout ����� �������� ������ �� ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMbMgr::Start(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout)
{
	DWORD ret = OPERATION_COMPLETE;

	try
	{
		MasterMB* master = new MasterMB(portName, baudrate, parity, charTimeout, responceTimeout, processor_);

		masterMbTable_.insert(std::pair<std::wstring, MasterMB*>(std::wstring(portName), master));
		errorTable_.erase(std::wstring(portName));

		PortSettings sett;
		sett.baudrate = baudrate;
		sett.parity = parity;
		sett.charTimeout = charTimeout;
		sett.responceTimeout = responceTimeout;

		AddSettingsInTable(portName, sett);

	}
	catch(DWORD err)
	{
		ret = err;
		errorTable_.insert(std::pair<std::wstring, DWORD>(std::wstring(portName), err));
	}

	return ret;
}

/**
 *	��������� ������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMbMgr::Stop(LPCTSTR portName)
{
	MasterMB* master = Find(portName);

	if( master )
	{

		masterMbTable_.erase(std::wstring(portName));
		LOG_Info("Delete MasterMB on port \"%s\"\n", portName);		
		delete master;

		return OPERATION_COMPLETE;
	}

	return PORT_NOT_OPEN;
}

/**
 *	�������� unicast-������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\param address ����� ����������.
 *	\param data ��������� �� ������ � �������.
 *	\param dataLen ������ ������� � ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMbMgr::SendRequest(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen)
{
	MasterMB* master = Find(portName);

	if( master )
		return master->SendRequest(address, data, dataLen);

	return PORT_NOT_OPEN;
}

/**
*	������������� ��������� ������� MODBUS.
 *	\param portName ��� �����.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMbMgr::Settings(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.baudrate = baudrate;
	sett.parity = parity;
	AddSettingsInTable(portName, sett);

	MasterMB* master = Find(portName);

	if( master )
		return master->Settings(baudrate, parity);

	return PORT_NOT_OPEN;
}

/**
 *	������������� �������� ������� MODBUS.
 *	\param portName ��� �����.\
 *	\param charTimeout ������� ������ (������) �������.
 *	\param responceTimeout ����� �������� ������ �� ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMbMgr::SetTimeouts(LPCTSTR portName, int charTimeout, int responceTimeout)
{
	PortSettings sett = GetSettingsFromTable(portName);
	sett.charTimeout = charTimeout;
	sett.responceTimeout = responceTimeout;
	AddSettingsInTable(portName, sett);

	MasterMB* master = Find(portName);

	if( master )
		return master->SetTimeouts(charTimeout, responceTimeout);

	return PORT_NOT_OPEN;
}

/**
 *	�������������� ��������� �� callback-������� ��������� ������ �� ������.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMbMgr::SetReceiveCallback(pReceiveMasterMBCallback callback)
{
	receiveCallback_ = callback;
	processor_.SetReceiveCallback(callback);
	return OPERATION_COMPLETE;
}

/**
 *	�������������� ��������� �� callback-������� ��������� �������� ������ ������.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMbMgr::SetTimeoutCallback(pTimeoutMasterMBCallback callback)
{
	timeoutCallback_ = callback;
	processor_.SetTimeoutCallback(callback);
	return OPERATION_COMPLETE;
}

/**
 *	�������������� ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMbMgr::SetSendCallback(pSendMasterMBCallback callback)
{
	sendCallback_ = callback;
	processor_.SetSendCallback(callback);
	return OPERATION_COMPLETE;
}

/**
 *	���������� ��� ��������� ��������� ������.
 *	\param portName ��� �����.
 *	\return ��� ��������� ������.
 */
DWORD MasterMbMgr::GetSystemError(LPCTSTR portName)
{
	MasterMB* master = Find(portName);
	if( master )
		return master->GetSystemErrorCode();

	std::map<std::wstring, DWORD>::iterator iter = errorTable_.find(portName);
	if( iter != errorTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	���� ������ MODBUS � ������� �� ����� COM �����.
 *	\param portName ��� �����.
 *	\return ��������� �� ��������� ������ MasterMB, ��� 0.
 */
MasterMB* MasterMbMgr::Find(LPCTSTR portName)
{
	std::map<std::wstring, MasterMB*>::iterator iter = masterMbTable_.find(std::wstring(portName));
	if( iter != masterMbTable_.end() )
		return iter->second;

	return 0;
}

/**
 *	��������� ��������� ������� MODBUS � �������.
 *	\param portName ��� �����.
 *	\param settings ��������� ������� MODBUS.
 */
void MasterMbMgr::AddSettingsInTable(LPCTSTR portName, PortSettings& settings)
{
	settingsTable_.insert(std::pair<std::wstring, PortSettings>(std::wstring(portName), settings));
}

/**
 *	���������� ��������� ������� MODBUS �� �������.
 *	\param portName ��� �����.
 *	\return ��������� ������� MODBUS.
 */
MasterMbMgr::PortSettings MasterMbMgr::GetSettingsFromTable(LPCTSTR portName)
{
	std::map<std::wstring, PortSettings>::iterator iter = settingsTable_.find(portName);
	if( iter == settingsTable_.end() )
		return defaultSettings_;
	else
		return iter->second;
}