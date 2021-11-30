/*
	Author:		R.Gorohov (K1 SOFT)

	PortRs485.h
	���������� ������ portRs485.
*/

#pragma once

#include <string>

/** �������� ���������� �� ������ � Rs485. */
class PortRs485
{
public:
	/** �����������. */
	PortRs485(LPCTSTR portName);
	/** ����������. */
	virtual ~PortRs485(void);

	/** ���������� ��� ��������� ��������� ������. */
	inline DWORD GetSystemErrorCode(){ return sysErrorCode_; }

	/** ������ ������ �� rs485. */
	DWORD Read(BYTE* data, int len, int* nmbRead);
	/** ���������� ������ � rs485. */
	DWORD Write(const BYTE* data, int len, int* nmbWritten);
	/** ������������� ��������� �����. */
	DWORD Settings(DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity);
	/** ������������� ������� ������ (�������) �����. */
	DWORD SetTimeout(int timeout);
	/** ���������� ��� �����. */
	const std::wstring& GetPortName(){ return portName_; }

	/** ��������� ����. */
	void Close();

	/**  */
	bool IsClosed(int timeout=0);

private:

	/** ��������� ����. */
	void Open();
	/** ��� ��������� ��������� ������. */
	DWORD sysErrorCode_;

	/** ������������� ��������� �����. */
	HANDLE hPort_;
	/** ������� ��������� ������ � COM-����. */
	HANDLE hWriteEvent_;
	/** ������� ��������� ������ �� COM-�����. */
	HANDLE hReadEvent_;
	/** ������� ���������� ��������. */
	HANDLE hCloseEvent_;
	/** ��������� OVERLAPPED ��� ������. */
	OVERLAPPED ovRead_;
	/** ��������� OVERLAPPED ��� ������. */
	OVERLAPPED ovWrite_;
	/** ��� �����. */	
	std::wstring portName_;
	/** ������� �� 1 ������ */
	int timeout_;
};
