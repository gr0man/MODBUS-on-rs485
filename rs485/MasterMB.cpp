/*
	Author:		R.Gorohov (K1 SOFT)

	MasterMB.cpp
	����������� ������� MasterBehaviorMB � MasterMB.
*/

#include "StdAfx.h"
#include "MasterMB.h"

#include "errorCodes.h"

#include "FrameMB.h"
#include "Command.h"

#include "time.h"

#include "rs485.h"

/**
 *	�����������.
 */
MasterBehaviorMB::MasterBehaviorMB(void)
: responceTimeout_(0)
, slaveAddress_(0)
, requestTime_(0)
, isNeedToRead_(false)
{
}

/**
 *	��������� ��������� �������� ������.
 *	\param address ����� ����������.
 */
void MasterBehaviorMB::FinishSend(BYTE address)
{ /*fix time and address*/
	requestTime_ = GetCurrentTime();
	slaveAddress_ = address;

	isNeedToRead_ = (address != 0);

	LOG_Error("MasterBehaviorMB::FinishSend: set addres %d\n", slaveAddress_);
}

void MasterBehaviorMB::FinishReceive(BYTE address)
{
	isNeedToRead_ = (address != slaveAddress_);
}

/**
 *	��������� ����������� ������ ��������� ������.
 *	\param addres ����� ��������� ������.
 *	\return true, ���� ����� ��������� � ���������.
 */
bool MasterBehaviorMB::IsAddressValid(BYTE address)
{ 
	LOG_Error("MasterBehaviorMB addres %d\n", slaveAddress_);
	LOG_Error("MasterBehaviorMB addres in frame %d\n", address);

	return (slaveAddress_ == address);
}

/**
 *	���������, �� ������� �� ����� �������� ������.
 *	\return true, ���� ������� �������� ��������.
 */
bool MasterBehaviorMB::IsTimeoutExpiration()
{ 
	if( requestTime_ == 0 || responceTimeout_ == 0 )
		return false;

	ULONGLONG crTime = GetCurrentTime();

	return ( (crTime-requestTime_) > responceTimeout_ );
}

/**
 *	���������� ��������� ��������.
 */
void MasterBehaviorMB::ResetSettings()
{
	slaveAddress_ = 0;
	requestTime_ = 0;
	isNeedToRead_ = false;
}


///////////////////////////////////////////////////////////////////////////////////////

/**
 *	�����������.
 *	\param portName ��� �����.
 *	\param baudrate ������� ������.
 *	\param parity �������� ��������.
 *	\param charTimeout ������� ������ (������) �������.
 *	\param responceTimeout ������� �������� ������ �� ������.
 */
MasterMB::MasterMB(LPCTSTR portName, DWORD baudrate, BYTE parity, int charTimeout, int responceTimeout, CmdProcessor& processor)
: trans_(portName, baudrate, parity, charTimeout, behavior_, processor)
{
	LOG_Info("MasterMB on \"%s\" constructor\n", portName);
}

/**
 *	����������.
 */
MasterMB::~MasterMB(void)
{
	//LOG_Info("MasterMB on \"%s\" destructor\n", trans_.GetPortName().c_str());
	trans_.Stop();
}

/**
 *	�������� ��������� RS485.
 *	\param baudrate �������� ������.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMB::Settings(DWORD baudrate, BYTE parity)
{
	return trans_.Settings(baudrate, parity);
}

/**
 *	�������������� ������� ������� ������� MODBUS.
 *	\param address ����� ����������.
 *	\param data ��������� �� ����� � �������.
 *	\param dataLen ������ ������ � ������.
 *	\return ��� ������ (������ OPERATION_COMPLETE).
 */
DWORD MasterMB::SendRequest(BYTE address, const BYTE* data, int dataLen)
{
	return trans_.NewSend(address, data, dataLen);
}

/**
 *	������������� ������� �������� ������ �� ������
 *	\param charTimeout ������� ������ (������) �������.
 *	\param responceTimeout ������� �������� ������ �� ������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD MasterMB::SetTimeouts(int charTimeout, int responceTimeout)
{
	return trans_.SetTimeouts(charTimeout, responceTimeout);
}
