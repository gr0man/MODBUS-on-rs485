/*
	Author:		R.Gorohov (K1 SOFT)

	PortRs485.cpp
	����������� ������ portRs485.
*/

#include "StdAfx.h"
#include "PortRs485.h"
#include "errorCodes.h"

#include "rs485.h"

/**
 *	�����������.
 *	\param portName ��� �����.
 */
PortRs485::PortRs485(LPCTSTR portName)
: portName_(portName)
, hPort_(INVALID_HANDLE_VALUE)
, sysErrorCode_(0)
, hWriteEvent_(CreateEvent(NULL, TRUE, FALSE, NULL))
, hReadEvent_(CreateEvent(NULL, TRUE, FALSE, NULL))
, hCloseEvent_(CreateEvent(NULL, TRUE, FALSE, NULL))
, timeout_(100)
{
	Open();
}

/**
 *	����������.
 */
PortRs485::~PortRs485(void)
{
	Close();

	if( hPort_ != INVALID_HANDLE_VALUE )
	{
		//LOG_Info("Close HANDLE COM port \"%s\"\n", portName_.c_str());		
		
		CloseHandle(hPort_);
		hPort_ = INVALID_HANDLE_VALUE;
	}	
	
	CloseHandle(hWriteEvent_);
	CloseHandle(hReadEvent_);
}

/**
 *	������ ������ �� rs485.\
 *	\param data ��������� �� ����� ��� ������ ������.
 *	\param len ������ ��������� ������.
 *	\param nmbRead ��������� �� ����������, � ������� ����������� ���������� ����������� ����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD PortRs485::Read(BYTE* data, int len, int* nmbRead)
{
	DWORD dwRead = 0;
	DWORD err = OPERATION_COMPLETE;

	ResetEvent(hReadEvent_);
	ZeroMemory(&ovRead_,sizeof(OVERLAPPED));
	ovRead_.hEvent = hReadEvent_;

	//LOG_Error("COM port start read %d bytes\n",len);

	BOOL ret = ReadFile(hPort_, data, len, &dwRead, &ovRead_);
	if(!ret)
	{
		sysErrorCode_ = GetLastError();
		if( sysErrorCode_ == ERROR_IO_PENDING )
		{
			HANDLE hEv[] = {hCloseEvent_, hReadEvent_};
			if( WaitForMultipleObjects(2, hEv, FALSE, timeout_*len) == WAIT_OBJECT_0 )
			{
				err = PORT_IS_CLOSED;
				return err;
			}
			
			DWORD dwReading = 0;
			GetOverlappedResult(hPort_,&ovRead_,&dwReading,FALSE);
			*nmbRead = dwReading;

			if(dwReading)
				LOG_Info("(1) COM port readed data: ", data, dwReading);
		}
		else
		{
			err = SYSTEM_ERROR;
			//LOG_Info("Read COM port \"%s\" ", portName_.c_str());
			//LOG_Error("error = %u\n", sysErrorCode_);

			if( WaitForSingleObject(hCloseEvent_, 0) == WAIT_OBJECT_0 )
			{
				err = PORT_IS_CLOSED;
				return err;
			}
		}
	}
	else
	{
		*nmbRead = dwRead;

		if( dwRead )
			LOG_Info("(2) COM port readed data: ", data, dwRead);
	}

	return err;
}

/**
 *	���������� ������ � rs485.
 *	\param data ��������� �� ������ ��� ������.
 *	\param len ������ ������.
 *	\param nmbRead ��������� �� ����������, � ������� ����������� ���������� ���������� ����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD PortRs485::Write(const BYTE* data, int len, int* nmbWritten)
{
	DWORD dwWritten = 0;
	DWORD err = OPERATION_COMPLETE;

	ResetEvent(hWriteEvent_);
	ZeroMemory(&ovWrite_,sizeof(OVERLAPPED));
	ovWrite_.hEvent = hWriteEvent_;

	LOG_Info("COM port start write data: ",data, len);

	BOOL ret = WriteFile(hPort_, data, len, &dwWritten, &ovWrite_);
	if(!ret)
	{
		sysErrorCode_ = GetLastError();
		if( sysErrorCode_ == ERROR_IO_PENDING )
		{
			HANDLE hEv[] = {hCloseEvent_, hWriteEvent_};
			//if( WaitForMultipleObjects(2, hEv, FALSE, timeout_*len) == WAIT_OBJECT_0 )
			if( WaitForSingleObject(hCloseEvent_, timeout_*len) == WAIT_OBJECT_0 )
			{
				err = PORT_IS_CLOSED;
				//return err;
			}
			
			DWORD dwWrtitten = 0;
			GetOverlappedResult(hPort_,&ovWrite_,&dwWritten,FALSE);
			*nmbWritten = dwWritten;

			LOG_Error("(1) COM port write %d bytes\n",dwWritten);
		}
		else
		{
			err = SYSTEM_ERROR;
			//LOG_Info("Write COM port \"%s\" ", portName_.c_str());
			LOG_Error("error = %u\n", sysErrorCode_);

			if( WaitForSingleObject(hCloseEvent_, 0) == WAIT_OBJECT_0 )
				err = PORT_IS_CLOSED;
		}
	}
	else
	{
		*nmbWritten = dwWritten;

		LOG_Error("(2) COM port write %d bytes\n",dwWritten);
	}

	return err;
}

/**
 *	������������� ��������� �����.
 *	\param baudrate �������� ������.
 *	\param byteSize ������ ����� � �����.
 *	\param stopBits ���������� �������� ���.
 *	\param parity �������� ��������.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD PortRs485::Settings(DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity)
{
	LOG_Info("PortRs485::Settings\n");

	DCB dcb;
	DWORD err = OPERATION_COMPLETE;
	if(!GetCommState(hPort_, &dcb))
	{
		sysErrorCode_ = GetLastError();
		return SYSTEM_ERROR;
	}
	dcb.BaudRate = baudrate;
	if( parity == NOPARITY )
		dcb.fParity = 0;
	else
		dcb.fParity = 1;
	dcb.Parity = parity;
	dcb.ByteSize = byteSize;

	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;


	if(!SetCommState(hPort_, &dcb))
	{
		sysErrorCode_ = GetLastError();
		return SYSTEM_ERROR;
	}

	return err;
}

/**
 *	������������� ������� ������ (�������) �����.
 *	\param timeout ������� ������ (������) �����.
 *	\return ��� ������ (\see ErrorCode).
 */
DWORD PortRs485::SetTimeout(int timeout)
{
	timeout_ = timeout;
	DWORD err = OPERATION_COMPLETE;

	return err;
}

/**
 *	��������� ����.
 */
void PortRs485::Open()
{
	hPort_ = CreateFile(portName_.c_str(), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if( hPort_ == INVALID_HANDLE_VALUE )
	{
		sysErrorCode_ = GetLastError();

		//LOG_Info("COM Port \"%s\"", GetPortName().c_str());
		LOG_Error(" open error = %d\n", sysErrorCode_);

		throw(sysErrorCode_);
	}
}

/**
 *	��������� ����.
 */
void PortRs485::Close()
{
	//LOG_Info("Close COM port \"%s\"\n", portName_.c_str());
	SetEvent(hCloseEvent_);
	SwitchToThread();
	Sleep(10);
}

/**
 *  ���������, �� ���� �� ������� ����.
 */
bool PortRs485::IsClosed(int timeout)
{
	return (WaitForSingleObject(hCloseEvent_, timeout) == WAIT_OBJECT_0);
}
