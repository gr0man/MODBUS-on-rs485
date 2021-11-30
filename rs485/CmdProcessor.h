/*
	Author:		R.Gorohov (K1 SOFT)

	CmdProcessor.h
	���������� ������ CmdProcessor.
*/

#pragma once

#include <string>

/** ����������� ������� ����� ����������� ������. */
class CmdProcessor
{
public:
	/** �����������. */
	CmdProcessor(void);
	/** ����������. */
	virtual ~CmdProcessor(void);

	/** ������������ ������� � ����� ������. */
	virtual void ProcessError(const std::wstring& portName, BYTE address, DWORD errCode)=0;
	/** ������������ ������� � �������. */
	virtual void ProcessData(const std::wstring& portName, BYTE address, const BYTE* data, int len)=0;
	/** ������������ ������� ��������� ���������. */
	virtual void ProcessSend(const std::wstring& portName, BYTE address)=0;
	/** ������������ ������� � ��������� ��������. */
	virtual void ProcessTimeout(const std::wstring& portName, BYTE address)=0;
};
