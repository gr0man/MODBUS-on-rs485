/*
	Author:		R.Gorohov (K1 SOFT)

	FrameMB.h
	���������� ������ FrameMB.
*/

#pragma once

/** ������������� ���������� ������ MODBUS. */
class FrameMB
{
public:
	/** �����������. */
	FrameMB(BYTE address, const BYTE* data, int len);
	/** �����������. */
	FrameMB(const BYTE* data, int len);

	/** ����������. */
	virtual ~FrameMB(void);

	/** ���������� ��������� �� ����� � ASCII �������. */
	const BYTE* GetAsciiFrame();
	/** ���������� ������ ASCII ������. */
	unsigned short GetAsciiFrameLen();

	/** ���������� ��������� �� ����� � ��������� ������� ������. */
	const BYTE* GetBinaryData();
	/** ���������� ������ ������ � ��������� �������. */
	unsigned short  GetBinaryLen();

	/** ���������� ����� �� ���� ������ ������. */
	BYTE GetAddress();

	/** ��������� LRC ASCII ������. */
	bool IsLRCValid(){ return isLRCValid_; };

	static bool IsASCIISymbolValid(BYTE ch);

private:
	/** ���� ������������ ASCII ������. */
	bool isLRCValid_;
	/** ������� ��� ����������� �������� ������ � ASCII. */
	static unsigned char s_hexTable[16];

	/** ASCII ������������� ������. */
	BYTE* asciiData_;
	/** ������ ASCII ������. */
	unsigned short asciiDataLen_;

	/** �������� ������. */
	BYTE* binData_;
	/** ������ ������ � ��������� �������. */
	unsigned short binDataLen_;

	/** ����� � �������� ����. */
	BYTE address_;

	/** ���������� ����� � ��������� �������. */
	void FreeBinaryData();
	/** ���������� ����� � ASCII �������������� ������. */
	void FreeAsciiData();
	/** �������� ����� ��� �������� ������. */
	void AllocBinaryData(unsigned short len);
	/** �������� ����� ��� ASCII �����. */
	void AllocAsciiData(unsigned short len);

	/** �������� �������� ������ �� ASCII ������. */
	void SetBinaryDataFromAsciiFrame();
	/** ��������� ASCII ����� �� �������� ������. */
	void SetAsciiFrameFromBinaryData();

	/** ������������ �������� ������ � ASCII �������������. */
	void ConvertBin2Ascii(BYTE* dst, int dstLen, const BYTE* src, int srcLen);
	/** ������������ ASCII ������ � ��������. */
	void ConvertAscii2Bin(BYTE* dst, int dstLen, const BYTE* src, int srcLen);

	/** ��������� LRC ��������� �������. */
	BYTE CalculateLRC(const BYTE* data, int len);

};
