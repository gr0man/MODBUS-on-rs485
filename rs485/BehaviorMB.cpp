/*
	Author:		R.Gorohov (K1 SOFT)

	BehaviorMB.cpp
	����������� ������ BehaviorMB.
*/

#include "StdAfx.h"
#include "BehaviorMB.h"

/**
 *	�����������.
 */
BehaviorMB::BehaviorMB(void)
{
}

/**
 *	����������.
 */
BehaviorMB::~BehaviorMB(void)
{
}

/**
 *	��������� ������� ��������� �����.
 *	\return ��������� ����� � ������� unsigned 64 bit.
 */
ULONGLONG BehaviorMB::GetCurrentTime()
{
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	FILETIME fileTime;
	SystemTimeToFileTime(&sysTime, &fileTime);

	ULARGE_INTEGER ulTime;
	ulTime.HighPart = fileTime.dwHighDateTime;
	ulTime.LowPart = fileTime.dwLowDateTime;

	return ulTime.QuadPart;
}