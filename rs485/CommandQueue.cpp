/*
	Author:		R.Gorohov (K1 SOFT)

	CommandQueue.cpp
	����������� ������ CommandQueue.
*/

#include "StdAfx.h"
#include "CommandQueue.h"

void Log(const char* info);

/**
 *	�����������.
 *	\param isSync ����, ������������ ������������� �������������.
 */
CommandQueue::CommandQueue(bool isSync)
: _lock(isSync ? (&CommandQueue::CriticalSectionLock) : (&CommandQueue::EmptyLock))
, _unlock(isSync ? (&CommandQueue::CriticalSectionUnlock) : (&CommandQueue::EmptyUnlock))
{
	InitializeCriticalSection(&_cs);
	while( !_data.empty() )
		_data.pop();
}

/**
 *	����������.
 */
CommandQueue::~CommandQueue(void)
{
	DeleteCriticalSection(&_cs);
}

/** 
 *	��������� ��������� �� ������� � ����� �������.
 *	\param data ��������� �� �������.
 */
void CommandQueue::PushBack(Command* data)
{
	Lock();

	_data.push(data);

	Unlock();
}

/** 
 *	�������� ��������� �� ������� �� ������ �������.
 *	\return ��������� �� ������� ���� ������� �� ������,
  *			0, ���� ������� ������. 
 */
Command* CommandQueue::GetFront()
{
	Lock();

	Command* cmd = 0;
	if( !_data.empty() )
		cmd = _data.front();

	Unlock();

	return cmd;
}

/** 
 *	������� ��������� �� ������� �� ������ �������.
 */
void CommandQueue::PopFront()
{
	Lock();

	_data.pop();

	Unlock();
}

/**
 *	���������, �������� �� ������� ������.
 *	\return true, ���� ������� ������,
 *			false, ���� ������� �� ������.
 */
bool CommandQueue::IsEmpty()
{
	bool ret;
	
	Lock();

	ret = _data.empty();
	
	Unlock();

	return ret;
}
