/*
	Author:		R.Gorohov (K1 SOFT)

	CommandQueue.cpp
	ќпределение класса CommandQueue.
*/

#include "StdAfx.h"
#include "CommandQueue.h"

void Log(const char* info);

/**
 *	 онструктор.
 *	\param isSync флаг, определ€ющий необходимость синхронизации.
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
 *	ƒеструктор.
 */
CommandQueue::~CommandQueue(void)
{
	DeleteCriticalSection(&_cs);
}

/** 
 *	ƒобавл€ет указатель на команду в конец очереди.
 *	\param data указатель на команду.
 */
void CommandQueue::PushBack(Command* data)
{
	Lock();

	_data.push(data);

	Unlock();
}

/** 
 *	ѕолучает указатель на команду из начала очереди.
 *	\return указатель на команду если очередь не пуста€,
  *			0, если очередь пуста€. 
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
 *	”дал€ет указатель на команду из начала очереди.
 */
void CommandQueue::PopFront()
{
	Lock();

	_data.pop();

	Unlock();
}

/**
 *	ѕровер€ет, €вл€етс€ ли очередь пустой.
 *	\return true, если очередь пуста€,
 *			false, если очередь не пуста€.
 */
bool CommandQueue::IsEmpty()
{
	bool ret;
	
	Lock();

	ret = _data.empty();
	
	Unlock();

	return ret;
}
