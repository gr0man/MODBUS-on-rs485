/*
	Author:		R.Gorohov (K1 SOFT)

	BehaviorMB.cpp
	Определение класса BehaviorMB.
*/

#include "StdAfx.h"
#include "BehaviorMB.h"

/**
 *	Конструктор.
 */
BehaviorMB::BehaviorMB(void)
{
}

/**
 *	Деструктор.
 */
BehaviorMB::~BehaviorMB(void)
{
}

/**
 *	Вычисляет текущее системное время.
 *	\return системное время в формате unsigned 64 bit.
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