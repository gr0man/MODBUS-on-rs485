/*
	Author:		R.Gorohov (K1 SOFT)

	CommandQueue.h
	Объявление класса CommandQueue.
*/

#pragma once

#include <queue>
#include <vector>
#include "Command.h"

/** Инкапсулирует очередь команд с возможностью многопоточной синхронизации. */
class CommandQueue
{
	/** Тип указателя на функцию блокировки очереди. */
	typedef void (CommandQueue::*pLock)();
	/** Тип указателя на фнкцию освобождения очерди. */
	typedef void (CommandQueue::*pUnlock)();

public:
	/** Конструктор. */
	CommandQueue(bool isSync = true);
	/** Деструктор. */
	~CommandQueue(void);


	/** Добавляет указатель на команду в конец очереди. */
	void PushBack(Command* data);
	/** Получает указатель на команду из начала очереди. */
	Command* GetFront();
	/** Удаляет указатель на команду из начала очереди. */
	void PopFront();
	/** Проверяет, является ли очередь пустой. */
	bool IsEmpty();

private:
	/** Несинхронизированная очередь указателей. */
	std::queue< Command* > _data;
	/** Элемент синхронизации. */
	CRITICAL_SECTION _cs;

	/** Указатель на функцию блокировки очереди. */
	pLock _lock;
	/** Указатель на функцию освобождения очереди. */
	pUnlock _unlock;

	/** Блокирует очередь с помощью критической секции. */
	void CriticalSectionLock(){ EnterCriticalSection(&_cs); }
	/** Освобождает очередь от блокировки. */
	void CriticalSectionUnlock(){ LeaveCriticalSection(&_cs); }

	/** "Заглушка" для несинхронизированной очереди */
	void EmptyLock(){};
	/** "Заглушка" для несинхронизированной очереди */
	void EmptyUnlock(){};

	// для удобочитаемости

	/** Вызывает функцию блокировки по указателю. */
	inline void Lock(){ (this->*_lock)(); }
	/** Вызывает функцию освобождения по указателю. */
	inline void Unlock(){ (this->*_unlock)(); }
};
