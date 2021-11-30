/*
	Author:		R.Gorohov (K1 SOFT)

	CommandQueue.h
	���������� ������ CommandQueue.
*/

#pragma once

#include <queue>
#include <vector>
#include "Command.h"

/** ������������� ������� ������ � ������������ ������������� �������������. */
class CommandQueue
{
	/** ��� ��������� �� ������� ���������� �������. */
	typedef void (CommandQueue::*pLock)();
	/** ��� ��������� �� ������ ������������ ������. */
	typedef void (CommandQueue::*pUnlock)();

public:
	/** �����������. */
	CommandQueue(bool isSync = true);
	/** ����������. */
	~CommandQueue(void);


	/** ��������� ��������� �� ������� � ����� �������. */
	void PushBack(Command* data);
	/** �������� ��������� �� ������� �� ������ �������. */
	Command* GetFront();
	/** ������� ��������� �� ������� �� ������ �������. */
	void PopFront();
	/** ���������, �������� �� ������� ������. */
	bool IsEmpty();

private:
	/** �������������������� ������� ����������. */
	std::queue< Command* > _data;
	/** ������� �������������. */
	CRITICAL_SECTION _cs;

	/** ��������� �� ������� ���������� �������. */
	pLock _lock;
	/** ��������� �� ������� ������������ �������. */
	pUnlock _unlock;

	/** ��������� ������� � ������� ����������� ������. */
	void CriticalSectionLock(){ EnterCriticalSection(&_cs); }
	/** ����������� ������� �� ����������. */
	void CriticalSectionUnlock(){ LeaveCriticalSection(&_cs); }

	/** "��������" ��� �������������������� ������� */
	void EmptyLock(){};
	/** "��������" ��� �������������������� ������� */
	void EmptyUnlock(){};

	// ��� ���������������

	/** �������� ������� ���������� �� ���������. */
	inline void Lock(){ (this->*_lock)(); }
	/** �������� ������� ������������ �� ���������. */
	inline void Unlock(){ (this->*_unlock)(); }
};
