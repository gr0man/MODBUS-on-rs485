/*
	Author:		R.Gorohov (K1 SOFT)

	ComportMgr.h
	���������� ������ ComPortMgr.
*/

#pragma once

#include <string>
#include <map>

class PortRs485;

/** ��������� ��������� RS485, �������� ��� COM-�����. */
class ComPortMgr
{
	/** ��������� COM-�����. */
	struct PortSettings
	{
		/** �������� �����. */
		DWORD baudrate;
		/** ������ ����� � �����. */
		BYTE byteSize;
		/** ���������� �������� ���. */
		BYTE stopBits;
		/** ��������� �������� ��������. */
		BYTE parity;
		/** ������� ������ (������) �������. */
		int charTimeout;
	};

	/** ��������� ��-���������. */
	PortSettings defaultSettings_;

public:
	/** �����������. */
	ComPortMgr(void);
	/** ����������. */
	~ComPortMgr(void);

	/** ��������� ��� �������� COM-�����. */
	void CloseAllPorts();

	/** ��������� ��������� COM ����. */
	DWORD Open(LPCTSTR portName);
	/** ��������� ��������� COM ����.*/
	DWORD Close(LPCTSTR portName);
	/** ������ ������ �� COM �����. */
	DWORD Read(LPCTSTR portName, BYTE* data, int len, int* nmbRead);
	/** ���������� ������ � COM ����. */
	DWORD Write(LPCTSTR portName, const BYTE* data, int len, int* nmbWritten);
	/** ������������� ��������� COM �����. */
	DWORD Settings(LPCTSTR portName, DWORD baudrate, BYTE byteSize, BYTE stopBits, BYTE parity);
	/** ������������� ������� �� ������ (������) �������. */
	DWORD SetTimeout(LPCTSTR portName, int timeout);

	/** ���������� ��� ��������� ��������� ������. */
	DWORD GetSystemError(LPCTSTR portName);
	/** ���������, ������ �� �������� COM ����. */
	bool IsExist(LPCTSTR portName){ return (Find(portName)!=0); }
	/** ��������� COM ���� � ������� ��� �� �������. */
	void Remove(LPCTSTR portName);

	/** ������������� ������������� ����������. */
	void Lock();
	/** ������� ������������� ����������. */
	void Unlock();

private:
	/** ������� ������ RS485, �������� ��� COM-�����. */
	std::map<std::wstring, PortRs485*> portTable_;
	/** ������� ��������� ��������� ������. */
	std::map<std::wstring, DWORD> errorTable_;
	/** ������� �������� COM ������. */
	std::map<std::wstring, PortSettings> portSettingsTable_;

	/** ������� ������������� �������������. */
	CRITICAL_SECTION cs_;

	/** ��������� ��������� COM ����� � �������. */
	void AddSettingsInTable(LPCTSTR portName, PortSettings& settings);
	/** ���������� ��������� �� ������� ��� ��������� COM �����. */
	PortSettings GetSettingsFromTable(LPCTSTR portName);

	/** ��������� COM ���� � �������. */
	void Add(LPCTSTR portName, PortRs485* port);
	/** ���� COM ���� � ������� �� �����. */
	PortRs485* Find(LPCTSTR portName);
};
