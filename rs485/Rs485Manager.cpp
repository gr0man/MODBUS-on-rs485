/*
	Author:		R.Gorohov (K1 SOFT)

	Rs485Manager.cpp
	����������� ������ Rs485Manager.
*/

#include "StdAfx.h"
#include "Rs485Manager.h"

#include "PortRS485.h"
#include "MasterMB.h"
#include "SlaveMB.h"

#include "errorCodes.h"

/**
 *	�����������.
 */
Rs485Manager::Rs485Manager(void)
{

}

/**
 *	����������.
 */
Rs485Manager::~Rs485Manager(void)
{
}

/**
 *	��������� �������� COM-����.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::OpenRs485(LPCTSTR portName)
{
	DWORD ret;

	comPortManager_.Lock();

	if( comPortManager_.IsExist(portName) )
	{
		ret = PORT_ALREADY_OPEN;
	}
	else
	{
		if(masterMbManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
		else
			ret = comPortManager_.Open(portName);
	}

	comPortManager_.Unlock();

	return ret;
}


/**
 *	��������� �������� COM-����.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::CloseRs485(LPCTSTR portName)
{
	DWORD ret;

	comPortManager_.Lock();

	ret = comPortManager_.Close(portName);
	if( ret == PORT_NOT_OPEN )
	{
		if( masterMbManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
	}

	comPortManager_.Unlock();

	return ret;

}

/**
 *	������ �� COM-����� ������.
 *	\param portName ��� �����.
 *	\param data ��������� �� ����� ��� ������ ������.
 *	\param len ������ ��������� ������.
 *	\param nmbRead ��������� �� ����������, � �������� ����������� ���������� ����������� ����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::ReadRs485(LPCTSTR portName, BYTE* data, int len, int* nmbRead)
{
	DWORD ret = comPortManager_.Read(portName, data, len, nmbRead);
	if( ret == PORT_NOT_OPEN )
	{
		if( masterMbManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
	}

	return ret;
}

/**
 *	���������� � COM-���� ������. 
 *	\param portName ��� �����.
 *	\param data ��������� �� ������ ��� ������.
 *	\param len ������ ������.
 *	\param nmbRead ��������� �� ����������, � �������� ����������� ���������� ���������� ����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::WriteRs485(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten)
{
	DWORD ret = comPortManager_.Write(portName, data, len, nmbWritten);
	if( ret == PORT_NOT_OPEN )
	{
		if( masterMbManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	return ret; 
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
DWORD Rs485Manager::SettingsRs485(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity)
{
	return comPortManager_.Settings(portName, baudrate, byteSize, stopBits, parity);
}

/**
 *	������������� ������� ������ (������) COM-�����.
 *	\param portName ��� �����.
 *	\param timeout ������� ������ (�������) �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SetTimeoutRs485(LPCTSTR portName, int timeout)
{
	return comPortManager_.SetTimeout(portName, timeout);
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
DWORD Rs485Manager::StartMaster(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout)
{
	DWORD ret;

	masterMbManager_.Lock();

	if( masterMbManager_.IsExist(portName) )
	{
		ret = PORT_ALREADY_OPEN;
	}
	else
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
		else
			ret = masterMbManager_.Start(portName, baudrate, parity, charTimeout, responceTimeout);
	}

	masterMbManager_.Unlock();

	return ret;
}

/**
 *	�������� unicast-������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\param address ����� ����������.
 *	\param data ��������� �� ������ � �������.
 *	\param dataLen ������ ������� � ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SendRequestMaster(LPCTSTR portName, BYTE address, const BYTE* data, int dataLen)
{
	LOG_Info("SendRequestMasterMB on port \"%s\"\n", portName);

	DWORD ret = masterMbManager_.SendRequest(portName, address, data, dataLen);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	LOG_Info("SendRequestMasterMB complete\n");
	return ret; 
}

/**
 *	������������� ��������� ������� MODBUS.
 *	\param portName ��� �����.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SettingsMaster(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	DWORD ret = masterMbManager_.Settings(portName, baudrate, parity);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	return ret;
}

/**
 *	������������� �������� ������� MODBUS.
 *	\param portName ��� �����.
 *	\param charTimeout ������� ������ (������) �������.
 *	\param responceTimeout ����� �������� ������ �� ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SetTimeoutsMaster(LPCTSTR portName, int charTimeout, int responceTimeout)
{
	DWORD ret = masterMbManager_.SetTimeouts(portName, charTimeout, responceTimeout);

	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	return ret;
}

/**
 *	��������� ������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::StopMaster(LPCTSTR portName)
{
	DWORD ret;
	
	masterMbManager_.Lock();

	ret = masterMbManager_.Stop(portName);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || slaveMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
	}

	masterMbManager_.Unlock();

	return ret;
}


/**
 *	�������������� ��������� �� callback-������� ��������� ������ �� ������.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SetReceiveMasterCallback(pReceiveMasterMBCallback callback)
{
	return masterMbManager_.SetReceiveCallback(callback);
}

/**
 *	�������������� ��������� �� callback-������� ��������� �������� ������ ������.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SetTimeoutMasterCallback(pTimeoutMasterMBCallback callback)
{
	return masterMbManager_.SetTimeoutCallback(callback);
}

/**
 *	�������������� ��������� �� callback-������� ��������� ��������� ������ �������� MODBUS.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SetSendMasterCallback(pSendMasterMBCallback callback)
{
	return masterMbManager_.SetSendCallback(callback);
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
DWORD Rs485Manager::StartSlave(LPCTSTR portName, BYTE address, DWORD baudrate, BYTE parity, int charTimeout)
{
	DWORD ret;

	slaveMbManager_.Lock();

	if( slaveMbManager_.IsExist(portName) )
	{
		ret = PORT_ALREADY_OPEN;
	}
	else
	{
		if( comPortManager_.IsExist(portName) || masterMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
		else
			ret = slaveMbManager_.Start(portName, address, baudrate, parity, charTimeout);
	}

	slaveMbManager_.Unlock();

	return ret;
}

/**
 *	�������� ����� �� ������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\param data ��������� �� ������ � �������.
 *	\param dataLen ������ ������� � ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SendReplySlave(LPCTSTR portName, const BYTE* data, int dataLen)
{
	DWORD ret = slaveMbManager_.SendReply(portName, data, dataLen);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || masterMbManager_.IsExist(portName) )
			return PORT_IS_BUSY;
	}

	return ret;
}

/**
 *	��������� ������ ������ MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::StopSlave(LPCTSTR portName)
{
	DWORD ret;

	slaveMbManager_.Lock();

	ret = slaveMbManager_.Stop(portName);
	if( ret == PORT_NOT_OPEN )
	{
		if( comPortManager_.IsExist(portName) || masterMbManager_.IsExist(portName) )
			ret = PORT_IS_BUSY;
	}

	slaveMbManager_.Unlock();

	return ret;
}

/**
 *	������������� ��������� ������ MODBUS.
 *	\param portName ��� �����.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SettingsSlave(LPCTSTR portName, DWORD baudrate, BYTE parity)
{
	return slaveMbManager_.Settings(portName, baudrate, parity);
}

/**
 *	������������� ������� �� ������/������ �������.
 *	\param portName ��� �����.
 *	\param charTimeout ������� ������ (������) �������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SetTimeoutSlave(LPCTSTR portName, int charTimeout)
{
	return slaveMbManager_.SetTimeout(portName, charTimeout);
}

/**
 *	�������������� ��������� �� callback-������� ��������� ������� �� ������� MODBUS.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SetReceiveSlaveCallback(pReceiveSlaveMBCallback callback)
{
	return slaveMbManager_.SetReceiveCallback(callback);
}

/**
 *	�������������� ��������� �� callback-������� ��������� ��������� ������ ������� MODBUS.
 *	\param ����� callback-�������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD Rs485Manager::SetSendSlaveCallback(pSendSlaveMBCallback callback)
{
	return slaveMbManager_.SetSendCallback(callback);
}


/**
 *	���������� ��� ��������� ������ RS485.
 *	\param portName ��� �����.
 *	\return ��� ��������� ������.
 */
DWORD Rs485Manager::GetSystemErrorRs485(LPCTSTR portName)
{
	return comPortManager_.GetSystemError(portName);
}

/**
 *	���������� ��� ��������� ������ ������� MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ��������� ������.
 */
DWORD Rs485Manager::GetSystemErrorMaster(LPCTSTR portName)
{
	return masterMbManager_.GetSystemError(portName);
}

/**
 *	���������� ��� ��������� ������ ������ MODBUS.
 *	\param portName ��� �����.
 *	\return ��� ��������� ������.
 */
DWORD Rs485Manager::GetSystemErrorSlave(LPCTSTR portName)
{
	return slaveMbManager_.GetSystemError(portName);
}

